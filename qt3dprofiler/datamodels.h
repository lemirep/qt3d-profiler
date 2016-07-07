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
#include <QRectF>
#include <QSize>

//////////// Structs for Binary Format of trace files ///////////////

struct FrameHeader
{
    FrameHeader()
        : frameId(0)
        , jobCount(0)
        , frameType(WorkerJob)
    {
    }

    enum FrameType {
        WorkerJob = 0,
        Submission
    };

    quint32 frameId;
    quint16 jobCount;
    quint16 frameType; // Submission or worker job
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
    qint64 m_relativeStart;
    qint64 m_relativeEnd;
    float m_x;
    int m_threadId;
};


class JobTraces
{
public:
    QVariant data(int role) const;

    std::unique_ptr<JobModel> m_jobModel;
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
        Color,
        RelativeStart,
        RelativeEnd,
        X,
        ThreadId
    };
    Q_ENUM(Roles)
};

class JobTracesModel : public ListModel<JobTraces>
{
    Q_OBJECT
public:
    enum Roles {
        JobsModel,
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

class RenderCommandModel;
class ShaderParamaterPackModel;
class ShaderBlockModel;
class TexturePackModel;

struct RenderViewInfo
{
    QVariant data(int role) const;

    QRectF m_viewport;
    QSize m_surfaceSize;
    float m_devicePixelRatio;
    bool m_noDraw;
    bool m_compute;
    bool m_frustumCulling;
    float m_clearDepthValue;
    float m_clearStencilValue;

    std::unique_ptr<RenderCommandModel> m_renderCommandModel;
};

struct RenderCommandInfo
{
    QVariant data(int role) const;

    int m_instanceCount;
    int m_shader;
    int m_vao;
    int m_geometry;
    int m_geometryRenderer;

    std::unique_ptr<ShaderParamaterPackModel> m_parameters;
    std::unique_ptr<ShaderBlockModel> m_ubos;
    std::unique_ptr<ShaderBlockModel> m_ssbos;
    std::unique_ptr<TexturePackModel> m_textures;
};

struct ParameterInfo
{
    QVariant data(int role) const;

    QString m_name;
    QVariant m_value;
    int m_uniformType; // Value or Texture
};

struct ShaderBlockInfo
{
    QVariant data(int role) const;

    int m_index;
    int m_bufferId;
};

struct ShaderTextureInfo
{
    QVariant data(int role) const;

    QString m_name;
    int m_id;
};

class RenderViewModel : public ListModel<RenderViewInfo>
{
    Q_OBJECT
public:
    enum Roles {
        Viewport = Qt::UserRole + 1,
        SurfaceSize,
        DevicePixelRatio,
        IsNoDraw,
        IsCompute,
        HasFrustumCulling,
        ClearDepthValue,
        ClearStencilValue,
        RenderCommandModel
    };
    Q_ENUM(Roles)
};

class RenderCommandModel : public ListModel<RenderCommandInfo>
{
    Q_OBJECT
public:
    enum Roles {
        InstanceCount = Qt::UserRole + 1,
        Shader,
        Vao,
        Geometry,
        GeometryRenderer,
        Parameters,
        UBOs,
        SSBOs,
        Textures
    };
    Q_ENUM(Roles)
};

class ShaderParamaterPackModel : public ListModel<ParameterInfo>
{
    Q_OBJECT
public:
    enum Roles {
        Name = Qt::UserRole + 1,
        Value,
        UniformType,
        ValueType
    };
    Q_ENUM(Roles)
};

class ShaderBlockModel : public ListModel<ShaderBlockInfo>
{
    Q_OBJECT
public:
    enum Roles {
        Index = Qt::UserRole + 1,
        BufferID
    };
    Q_ENUM(Roles)
};

class TexturePackModel : public ListModel<ShaderTextureInfo>
{
    Q_OBJECT
public:
    enum Roles {
        Name = Qt::UserRole + 1,
        ID
    };
    Q_ENUM(Roles)
};

#endif // DATAMODELS_H
