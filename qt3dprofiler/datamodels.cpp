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
    case JobModel::RelativeEnd:
        return m_relativeEnd;
    case JobModel::RelativeStart:
        return m_relativeStart;
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
    case FrameModel::StartTimeMS:
        return m_startTime / 1000000;
    case FrameModel::TotalDurationMS:
        return m_totalDuration / 1000000;
    case FrameModel::FrameType:
        return m_frameType;
    case FrameModel::TimeSinceLastFrameMS:
        return m_timeSinceEndOfLastFrame / 1000000;
    default:
        return QVariant();
    }
}

QVariant JobTraces::data(int role) const
{
    switch (role) {
    case JobTracesModel::WorkerJobFramesModel:
        return QVariant::fromValue(m_workJobStatsModel.get());
    case JobTracesModel::SubmissionJobFramesModel:
        return QVariant::fromValue(m_submissionJobStatsModel.get());
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

QVariant RenderCommandInfo::data(int role) const
{
    switch (role) {
    case RenderCommandModel::InstanceCount:
        return m_instanceCount;
    case RenderCommandModel::Shader:
        return m_shader;
    case RenderCommandModel::Vao:
        return m_vao;
    case RenderCommandModel::Geometry:
        return m_geometry;
    case RenderCommandModel::GeometryRenderer:
        return m_geometryRenderer;
    case RenderCommandModel::Parameters:
        return QVariant::fromValue(m_parameters.get());
    case RenderCommandModel::UBOs:
        return QVariant::fromValue(m_ubos.get());
    case RenderCommandModel::SSBOs:
        return QVariant::fromValue(m_ssbos.get());
    case RenderCommandModel::Textures:
        return QVariant::fromValue(m_textures.get());
    default:
        return QVariant();
    }
}

QVariant RenderViewInfo::data(int role) const
{
    switch (role) {
    case RenderViewModel::Viewport:
        return m_viewport;
    case RenderViewModel::SurfaceSize:
        return m_surfaceSize;
    case RenderViewModel::DevicePixelRatio:
        return m_devicePixelRatio;
    case RenderViewModel::IsNoDraw:
        return m_noDraw;
    case RenderViewModel::IsCompute:
        return m_compute;
    case RenderViewModel::HasFrustumCulling:
        return m_frustumCulling;
    case RenderViewModel::ClearDepthValue:
        return m_clearDepthValue;
    case RenderViewModel::ClearStencilValue:
        return m_clearStencilValue;
    case RenderViewModel::RenderCommandModel:
        return QVariant::fromValue(m_renderCommandModel.get());
    default:
        return QVariant();
    }
}

QVariant ParameterInfo::data(int role) const
{
    switch (role) {
    case ShaderParamaterPackModel::Name:
        return m_name;
    case ShaderParamaterPackModel::Value:
        return m_value;
    case ShaderParamaterPackModel::UniformType:
        return m_uniformType;
    default:
        return QVariant();
    }
}

QVariant ShaderTextureInfo::data(int role) const
{
    switch (role) {
    case TexturePackModel::Name:
        return m_name;
    case TexturePackModel::ID:
        return m_id;
    default:
        return QVariant();
    }
}

QVariant ShaderBlockInfo::data(int role) const
{
    switch (role) {
    case ShaderBlockModel::Index:
        return m_index;
    case ShaderBlockModel::BufferID:
        return m_bufferId;
    default:
        return QVariant();
    }
}
