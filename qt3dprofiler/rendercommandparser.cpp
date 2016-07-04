#include "rendercommandparser.h"
#include "datamodels.h"
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>
#include <QVector3D>
#include <QVector4D>
#include <QRectF>
#include <QSize>
#include <QMatrix4x4>

namespace {

template<typename Type>
Type jsonValueToType(const QJsonValue &value)
{
    return Type();
}

template<>
QVector3D jsonValueToType<QVector3D>(const QJsonValue &value)
{
    QJsonArray array = value.toArray();
    Q_ASSERT(array.size() > 2);
    return QVector3D(array.at(0).toDouble(),
                     array.at(1).toDouble(),
                     array.at(2).toDouble());
}

template<>
QVector4D jsonValueToType<QVector4D>(const QJsonValue &value)
{
    QJsonArray array = value.toArray();
    Q_ASSERT(array.size() > 3);
    return QVector4D(array.at(0).toDouble(),
                     array.at(1).toDouble(),
                     array.at(2).toDouble(),
                     array.at(3).toDouble());
}

template<>
QRectF jsonValueToType<QRectF>(const QJsonValue &value)
{
    QJsonArray array = value.toArray();
    Q_ASSERT(array.size() > 3);
    return QRectF(array.at(0).toDouble(),
                  array.at(1).toDouble(),
                  array.at(2).toDouble(),
                  array.at(3).toDouble());
}

template<>
QSize jsonValueToType<QSize>(const QJsonValue &value)
{
    QJsonArray array = value.toArray();
    Q_ASSERT(array.size() > 1);
    return QSize(array.at(0).toInt(),
                 array.at(1).toInt());
}

template<>
QMatrix4x4 jsonValueToType<QMatrix4x4>(const QJsonValue &value)
{
    QJsonArray array = value.toArray();

    Q_ASSERT(array.size() > 3);

    float values[16];


    for (int i = 0; i < 4; ++i) {
        const QVector4D row = jsonValueToType<QVector4D>(array.at(i));
        memcpy(values + i * 4, &row, sizeof(QVector4D));
    }

    return QMatrix4x4(values);
}

RenderCommandInfo parseRenderCommand(const QJsonObject &renderCommand)
{
    RenderCommandInfo command;

    command.m_shader = renderCommand.value(QLatin1String("shader")).toInt();
    command.m_geometry = renderCommand.value(QLatin1String("geometry")).toInt();
    command.m_geometryRenderer = renderCommand.value(QLatin1String("geometryRenderer")).toInt();
    command.m_vao = renderCommand.value(QLatin1String("vao")).toInt();
    command.m_instanceCount = renderCommand.value(QLatin1String("instanceCount")).toInt();

    const QJsonObject parameterPackObj = renderCommand.value(QLatin1String("shaderParameterPack")).toObject();

    command.m_parameters.reset(new ShaderParamaterPackModel);
    const QJsonArray parametersArray = parameterPackObj.value(QLatin1String("uniforms")).toArray();
    std::vector<ParameterInfo> parameters;
    parameters.reserve(parametersArray.size());

    return std::move(command);
}

RenderViewInfo parseRenderView(const QJsonObject &renderViews)
{
    RenderViewInfo view;

    view.m_viewport = jsonValueToType<QRectF>(renderViews.value(QLatin1String("viewport")));
    view.m_surfaceSize = jsonValueToType<QSize>(renderViews.value(QLatin1String("surfaceSize")));
    view.m_devicePixelRatio = renderViews.value(QLatin1String("devicePixelRatio")).toDouble();
    view.m_noDraw = renderViews.value(QLatin1String("noDraw")).toBool();
    view.m_frustumCulling = renderViews.value(QLatin1String("frustumCulling")).toBool();
    view.m_compute = renderViews.value(QLatin1String("compute")).toBool();
    view.m_clearDepthValue = renderViews.value(QLatin1String("clearDepthValue")).toDouble();
    view.m_clearStencilValue = renderViews.value(QLatin1String("clearStencilValue")).toDouble();

    std::vector<RenderCommandInfo> commandInfos;
    const QJsonArray renderCommands = renderViews.value("commands").toArray();
    commandInfos.reserve(renderCommands.size());

    for (const QJsonValue &v : renderCommands)
        commandInfos.push_back(parseRenderCommand(v.toObject()));

    view.m_renderCommandModel.reset(new RenderCommandModel);
    view.m_renderCommandModel->insertRows(std::move(commandInfos));

    qDebug() << view.m_renderCommandModel->rowCount();

    return std::move(view);
}

} // anonymous


RenderViewModel *RenderCommandParser::parseRenderViews(const QJsonObject &renderViewObject)
{
    RenderViewModel *model = new RenderViewModel;

    const QJsonArray renderViewsArray = renderViewObject.value(QLatin1String("renderViews")).toArray();
    std::vector<RenderViewInfo> renderViewInfos;
    renderViewInfos.reserve(renderViewsArray.size());
    for (const QJsonValue &v : renderViewsArray)
        renderViewInfos.push_back(parseRenderView(v.toObject()));
    model->insertRows(std::move(renderViewInfos));

    return model;
}
