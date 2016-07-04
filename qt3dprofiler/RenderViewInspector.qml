import QtQuick 2.7
import QtQuick.Controls 2.0
import Profiler 1.0

Item {
    Flickable {
        anchors.fill: parent
        contentWidth: col.width
        contentHeight: col.height

        ScrollBar.vertical: ScrollBar {
            id: control
            orientation: Qt.Vertical
            contentItem: Rectangle {
                implicitWidth: 6
                implicitHeight: 100
                radius: width / 2
                color: control.pressed ? "#81e889" : "#c2f4c6"
            }
            background: Rectangle {
                color: "#333333"
            }
        }

        Column {
            id: col
            width: mainRoot.width
            height: childrenRect.height

            Column {
                x: 25
                spacing: 10
                Repeater {
                    model: Singleton.renderViewModel
                    RenderView {
                        viewport: "" + model.Viewport
                        surfaceSize: "" + model.SurfaceSize
                        devicePixelRatio: model.DevicePixelRatio
                        noDraw: model.IsNoDraw
                        isCompute: model.IsCompute
                        frustumCulling: model.HasFrustumCulling
                        clearDepthValue: model.ClearDepthValue.toFixed(6)
                        clearStencilValue: model.ClearStencilValue.toFixed(6)
                        renderCommands: model.RenderCommandModel
                    }
                }
            }
        }
    }
}
