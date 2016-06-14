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

#ifndef DATAMODELS_H
#define DATAMODELS_H

#include "listmodel.h"
#include <memory>
#include <QSharedPointer>
#include <QColor>

//////////// Structs for Binary Format of trace files ///////////////

struct FrameHeader
{
    quint32 frameId;
    quint32 jobCount;
};

union JobId
{
    quint32 typeAndInstance[2];
    quint64 id = 0;
};

struct JobRunStats
{
    qint64 startTime;
    qint64 endTime;
    JobId jobId;
    quint64 threadId;
};

////////////// Job Models ///////////////

class JobModel;
class ThreadModel;
class FrameModel;

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

class JobTraces
{
public:
    QVariant data(int role) const;

    std::unique_ptr<FrameModel> m_jobStatsModel;
    int m_threadCount = 0;
    quint64 m_totalDuration = 0;
    QString m_title;
};
Q_DECLARE_TYPEINFO(JobTraces, Q_MOVABLE_TYPE);

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
        StartTime,
        TotalDuration
    };
    Q_ENUM(Roles)
};

class JobTracesModel : public ListModel<JobTraces>
{
    Q_OBJECT
public:
    enum Roles {
        JobFramesModel = Qt::UserRole + 1,
        ThreadCount,
        TotalDuration,
        Title
    };
    Q_ENUM(Roles)
};

//// MODELS FOR DISPLAY PURPOSES ONLY

struct JobInfo
{
    QVariant data(int role) const;

    int m_typeId;
    QString m_name;
    QColor m_color;
};

struct CommandInfo
{
    QString m_name;
    int m_returnType;
};

struct Command
{
    QVariant data(int role) const;

    QString m_content;
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

struct AspectJobInfo
{
    QVariant data(int role) const;

    QString m_name;
    std::unique_ptr<JobTypeInfoModel> m_jobTypeInfoModel;
};
Q_DECLARE_TYPEINFO(AspectJobInfo, Q_MOVABLE_TYPE);

class AspectInfoModel : public ListModel<AspectJobInfo>
{
    Q_OBJECT
public:
    enum Roles {
        Name = Qt::UserRole + 1,
        JobTypeInfoModel
    };
    Q_ENUM(Roles)
};

class CommandDisplayModel : public ListModel<Command>
{
    Q_OBJECT
public:
    enum Roles {
        Content = Qt::UserRole + 1
    };
    Q_ENUM(Roles)

    Q_INVOKABLE bool clear()
    {
        return removeRows(0, rowCount() - 1);
    }
};

#endif // DATAMODELS_H
