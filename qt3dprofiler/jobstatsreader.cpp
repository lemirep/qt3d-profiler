/****************************************************************************
**
** Copyright (C) 2016 Paul Lemire <paul.lemire350@gmail.com>
**
** This file is part of the Qt3D Profiler
**
** $QT_BEGIN_LICENSE:GPL-QTAS$
** Commercial License Usage
** Licensees holding valid commercial Qt Automotive Suite licenses may use
** this file in accordance with the commercial license agreement provided
** with the Software or, alternatively, in accordance with the terms
** contained in a written agreement between you and The Qt Company.  For
** licensing terms and conditions see https://www.qt.io/terms-conditions.
** For further information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
** SPDX-License-Identifier: GPL-3.0
**
****************************************************************************/

#include "jobstatsreader.h"
#include "listmodel.h"
#include <QFile>
#include <QUrl>
#include <QSharedPointer>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QColor>
#include <QDebug>

//////////// Structs for Binary Format ///////////////

struct FrameHeader
{
    quint32 frameId;
    quint32 jobCount;
};

union JobId
{
    quint32 typeAndInstance[2];
    quint64 id;
};

struct JobRunStats
{
    JobRunStats()
    {
        jobId.id = 0;
    }

    qint64 startTime;
    qint64 endTime;
    JobId jobId;
    quint64 threadId;
};

////////////// Models ///////////////

class JobModel;
class ThreadModel;

class Job
{
public:
    QVariant data(int role) const;

    JobRunStats m_jobStats;
    qint64 m_frameStart;
    qint64 m_frameEnd;
    QString m_name;
    QColor m_color;
};

class Thread
{
public:
    QVariant data(int role) const;

    QSharedPointer<JobModel> m_jobModel;
    int m_threadId;
};

class Frame
{
public:
    QVariant data(int role) const;

    FrameHeader m_header;
    QSharedPointer<ThreadModel> m_threadModel;
    quint64 m_totalDuration;
    quint64 m_startTime;
};

class JobModel : public ListModel<Job>
{
    Q_OBJECT
public:
    enum Roles {
        InstanceId = Qt::UserRole + 1,
        Type,
        Start,
        End,
        Duration,
        FrameStart, // Relative to the total duration of a frame
        FrameEnd,
        Name,
        Color
    };
    Q_ENUM(Roles)
};

class ThreadModel : public ListModel<Thread>
{
    Q_OBJECT
public:
    enum Roles {
        Id = Qt::UserRole + 1,
        JobCount,
        JobModel
    };
    Q_ENUM(Roles)
};

class FrameModel : public ListModel<Frame>
{
    Q_OBJECT
public:
    enum Roles {
        Id = Qt::UserRole + 1,
        ThreadCount,
        ThreadsModel,
        TotalDuration
    };
    Q_ENUM(Roles)
};


QVariant Job::data(int role) const
{
    switch (role) {
    case JobModel::InstanceId:
        return m_jobStats.jobId.typeAndInstance[1];
    case JobModel::Type:
        return m_jobStats.jobId.typeAndInstance[0];
    case JobModel::Start:
        return m_jobStats.startTime;
    case JobModel::End:
        return m_jobStats.endTime;
    case JobModel::Duration:
        return m_jobStats.endTime - m_jobStats.startTime;
    case JobModel::FrameStart:
        return m_frameStart;
    case JobModel::FrameEnd:
        return m_frameEnd;
    case JobModel::Name:
        return m_name;
    case JobModel::Color:
        return m_color;
    default:
        return QVariant();
    }
}

QVariant Thread::data(int role) const
{
    switch (role) {
    case ThreadModel::Id:
        return m_threadId;
    case ThreadModel::JobCount:
        return m_jobModel->rowCount();
    case ThreadModel::JobModel:
        return QVariant::fromValue(m_jobModel.data());
    default:
        return QVariant();
    }
}

QVariant Frame::data(int role) const
{
    switch (role) {
    case FrameModel::Id:
        return m_header.frameId;
    case FrameModel::ThreadCount:
        return m_threadModel->rowCount();
    case FrameModel::ThreadsModel:
        return QVariant::fromValue(m_threadModel.data());
    case FrameModel::TotalDuration:
        return m_totalDuration;
    default:
        return QVariant();
    }
}

//// MODELS FOR DISPLAY PURPOSES ONLY

struct JobInfo
{
    QVariant data(int role) const;

    int m_typeId;
    QString m_name;
    QColor m_color;
};

class JobTypeInfoModel : public ListModel<JobInfo>
{
    Q_OBJECT
public:
    enum Roles {
        TypeId = Qt::UserRole + 1,
        Name,
        Color
    };
    Q_ENUM(Roles)
};


QVariant JobInfo::data(int role) const
{
    switch (role) {
    case JobTypeInfoModel::TypeId:
        return m_typeId;
    case JobTypeInfoModel::Name:
        return m_name;
    case JobTypeInfoModel::Color:
        return m_color;
    default:
        return QVariant();
    }
}


//// MAIN CLASS

JobStatsReader::JobStatsReader()
    : QObject()
    , m_jobStatsModel(new FrameModel)
    , m_jobTypeInfoModel(new JobTypeInfoModel)
    , m_msecToPixelScale(1000.0f)
    , m_threadCount(0)
{
    parseConfigFile(QLatin1Literal(":/config.json"));
}

JobStatsReader::~JobStatsReader()
{}

void JobStatsReader::parseConfigFile(const QString &filePath)
{
    QFile configFile(filePath);
    if (!configFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open configuration file";
        return;
    }

    QJsonDocument jsonDoc = QJsonDocument::fromJson(configFile.readAll());
    if (jsonDoc.isNull() || !jsonDoc.isObject()) {
        qWarning() << "Malformed configuration file";
        return;
    }

    QJsonObject root = jsonDoc.object();
    std::vector<JobInfo> jobInfo;
    const QJsonArray aspects = root.value(QLatin1String("aspects")).toArray();
    for (const QJsonValue &aspectValue : aspects) {
        const QJsonObject aspectObj = aspectValue.toObject();
        const QJsonArray jobs = aspectObj.value(QLatin1String("jobs")).toArray();
        for (const QJsonValue &jobValue : jobs) {
            const QJsonObject job = jobValue.toObject();
            const int typeId = job.value(QLatin1String("type")).toInt();
            const QColor jobColor(job.value(QLatin1String("color")).toString());
            const QString jobName = job.value(QLatin1String("name")).toString();
            m_jobTypeToColorTable.insert(typeId, jobColor);
            m_jobTypeToNameTable.insert(typeId, jobName);
            jobInfo.push_back({typeId, jobName, jobColor});
        }
    }
    // Build JobTypeModel based on the above
    m_jobTypeInfoModel->insertRows(jobInfo);
}

void JobStatsReader::readTraceFile(const QUrl &fileUrl)
{
    QFile file(fileUrl.toLocalFile());
    m_jobStatsModel->clear();
    if (file.open(QFile::ReadOnly)) {
        const int sizeOfHeader = sizeof(FrameHeader);
        const int sizeOfJob = sizeof(JobRunStats);
        QByteArray headerBuffer(sizeOfHeader, '\0');
        QByteArray jobBuffer(sizeOfJob, '\0');

        QVector<quint64> threadIds;
        // Read header
        uint readSize = 0;
        // Check file is at least as long as what header says
        while ((readSize = file.read(headerBuffer.data(), sizeOfHeader)) == sizeOfHeader) {
            FrameHeader *header = reinterpret_cast<FrameHeader *>(headerBuffer.data());

            // Read commands
            uint c = 0;
            std::vector<Job> jobs;
            while (c < header->jobCount && (readSize = file.read(jobBuffer.data(), sizeOfJob)) == sizeOfJob) {
                JobRunStats *jobStat = reinterpret_cast<JobRunStats *>(jobBuffer.data());
                Job job;
                job.m_jobStats = *jobStat;
                job.m_color = m_jobTypeToColorTable.value(jobStat->jobId.typeAndInstance[0], QColor(Qt::red));
                job.m_name = m_jobTypeToNameTable.value(jobStat->jobId.typeAndInstance[0], QLatin1String("Unknown"));
                jobs.push_back(job);
                ++c;
            }

            // Sort Jobs by thread id and start time
            std::sort(jobs.begin(), jobs.end(), [] (const Job &a, const Job &b) {
                return a.m_jobStats.threadId < b.m_jobStats.threadId;
            });

            for (int i = 1, m = jobs.size(); i < m; ++i) {
                const int prec = i - 1;
                while (i < m && jobs[prec].m_jobStats.threadId == jobs[i].m_jobStats.threadId)
                    ++i;
                if (i - prec > 1) {
                    std::sort(jobs.begin() + prec, jobs.begin() + i, [] (const Job &a, const Job &b) {
                        return a.m_jobStats.startTime < b.m_jobStats.startTime;
                    });
                }
            }

            qint64 startTime = std::numeric_limits<qint64>::max();
            qint64 endTime = std::numeric_limits<qint64>::min();

            // Get minimum start and maximum end time
            for (const Job &job : jobs) {
                endTime = std::max(job.m_jobStats.endTime, endTime);
                startTime = std::min(job.m_jobStats.startTime, startTime);
            }

            for (int i = 0, m = jobs.size(); i < m; ++i) {
                Job &job = jobs[i];
                job.m_frameStart = job.m_jobStats.startTime - startTime;
                job.m_frameEnd = job.m_jobStats.endTime - startTime;
            }

            Frame f;
            f.m_header = std::move(*header);
            f.m_threadModel.reset(new ThreadModel);
            f.m_startTime = startTime;
            f.m_totalDuration = endTime - startTime;

            for (int i = 0, m = jobs.size(); i < m;) {
                const int j = i;
                while (i < m && jobs[j].m_jobStats.threadId == jobs[i].m_jobStats.threadId)
                    ++i;
                // Create a Thread Model for each unique thread
                Thread t;

                if (!threadIds.contains(jobs[j].m_jobStats.threadId))
                    threadIds.push_back(jobs[j].m_jobStats.threadId);

                t.m_threadId = threadIds.indexOf(jobs[j].m_jobStats.threadId) + 1;
                t.m_jobModel.reset(new JobModel);
                auto first = std::make_move_iterator(jobs.begin() + j);
                auto last = std::make_move_iterator(jobs.begin() + i);
                t.m_jobModel->insertRows(std::vector<Job>(first, last));
                f.m_threadModel->insertRow(t);
            }

            m_jobStatsModel->insertRow(f);
        } // Repeat
        m_threadCount = threadIds.size();
        emit threadCountChanged();
        emit jobStatsModelChanged();
        qDebug() << Q_FUNC_INFO << "Done";
    } else {
        qDebug() << Q_FUNC_INFO << "Failure to open";
    }
}

QAbstractListModel *JobStatsReader::jobStatsModel() const
{
    return m_jobStatsModel.data();
}

QAbstractListModel *JobStatsReader::jobTypeInfoModel() const
{
    return m_jobTypeInfoModel.data();
}

float JobStatsReader::msecToPixelScale() const
{
    return m_msecToPixelScale;
}

int JobStatsReader::threadCount() const
{
    return m_threadCount;
}

void JobStatsReader::setMsecToPixelScale(float scale)
{
    if (m_msecToPixelScale != scale) {
        m_msecToPixelScale = scale;
        emit msecToPixelScaleChanged();
    }
}

#include "jobstatsreader.moc"
