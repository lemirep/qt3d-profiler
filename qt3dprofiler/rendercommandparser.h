#ifndef RENDERCOMMANDPARSER_H
#define RENDERCOMMANDPARSER_H

class RenderCommandModel;
class RenderViewModel;
class QJsonObject;

struct RenderCommandParser
{
    static RenderViewModel *parseRenderViews(const QJsonObject &renderViewsObject);
};

#endif // RENDERCOMMANDPARSER_H
