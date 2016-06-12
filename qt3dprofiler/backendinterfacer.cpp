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

#include "backendinterfacer.h"
#include "datamodels.h"
#include "jobstatsreader.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>

BackendInterfacer::BackendInterfacer(QObject *parent)
    : QObject(parent)
    , m_msecToPixelScale(50.0f)
    , m_aspectInfoModel(new AspectInfoModel)
    , m_jobTracesModel(new JobTracesModel)
{
    parseConfigFile(QLatin1Literal(":/config.json"));
}

BackendInterfacer::~BackendInterfacer()
{
}

QAbstractListModel *BackendInterfacer::aspectInfoModel() const
{
    return m_aspectInfoModel.data();
}

QAbstractListModel *BackendInterfacer::jobTracesModel() const
{
    return m_jobTracesModel.data();
}

void BackendInterfacer::addTraceFile(const QUrl &fileUrl)
{
    m_jobTracesModel->insertRow(std::move(JobStatsReader::readTraceFile(fileUrl)));
}

void BackendInterfacer::removeTrace(int idx)
{
    qDebug() << Q_FUNC_INFO;
    m_jobTracesModel->removeRows(idx);
}

float BackendInterfacer::msecToPixelScale() const
{
    return m_msecToPixelScale;
}

void BackendInterfacer::setMsecToPixelScale(float scale)
{
    if (m_msecToPixelScale != scale) {
        m_msecToPixelScale = scale;
        emit msecToPixelScaleChanged();
    }
}

void BackendInterfacer::parseConfigFile(const QString &filePath)
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
    const QJsonArray aspects = root.value(QLatin1String("aspects")).toArray();

    std::vector<AspectJobInfo> aspectsInfo;
    aspectsInfo.reserve(aspects.size());

    for (const QJsonValue &aspectValue : aspects) {
        const QJsonObject aspectObj = aspectValue.toObject();

        AspectJobInfo aspectJobInfo;
        aspectJobInfo.m_name = aspectObj.value(QLatin1String("name")).toString();
        aspectJobInfo.m_jobTypeInfoModel.reset(new JobTypeInfoModel);

        const QJsonArray jobs = aspectObj.value(QLatin1String("jobs")).toArray();
        std::vector<JobInfo> jobInfo;
        jobInfo.reserve(jobs.size());

        // Build JobTypeModel based on the above
        for (const QJsonValue &jobValue : jobs) {
            const QJsonObject job = jobValue.toObject();
            const int typeId = job.value(QLatin1String("type")).toInt();
            const QColor jobColor(job.value(QLatin1String("color")).toString());
            const QString jobName = job.value(QLatin1String("name")).toString();
            JobStatsReader::jobTypeToColorTable.insert(typeId, jobColor);
            JobStatsReader::jobTypeToNameTable.insert(typeId, jobName);
            jobInfo.push_back({typeId, jobName, jobColor});
        }

        aspectJobInfo.m_jobTypeInfoModel->insertRows(jobInfo);
        aspectsInfo.push_back(std::move(aspectJobInfo));
    }

    // Build AspectModel based on the above
    m_aspectInfoModel->insertRows(std::move(aspectsInfo));
}
