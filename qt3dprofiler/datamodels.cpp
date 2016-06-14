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

#include "datamodels.h"

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
    case FrameModel::StartTime:
        return m_startTime;
    default:
        return QVariant();
    }
}

QVariant JobTraces::data(int role) const
{
    switch (role) {
    case JobTracesModel::JobFramesModel:
        return QVariant::fromValue(m_jobStatsModel.get());
    case JobTracesModel::ThreadCount:
        return m_threadCount;
    case JobTracesModel::TotalDuration:
        return m_totalDuration;
    case JobTracesModel::Title:
        return m_title;
    default:
        return QVariant();
    }
}

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

QVariant AspectJobInfo::data(int role) const
{
    switch (role) {
    case AspectInfoModel::Name:
        return m_name;
    case AspectInfoModel::JobTypeInfoModel:
        return QVariant::fromValue(m_jobTypeInfoModel.get());
    default:
        return QVariant();
    }
}

QVariant Command::data(int role) const
{
    switch (role) {
    case CommandDisplayModel::Content:
        return m_content;
    default:
        return QVariant();
    }
}
