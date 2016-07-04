import QtQuick 2.7

Item {
    id: root
    property alias viewport: viewportText.text
    property alias surfaceSize: surfaceSizeText.text
    property alias devicePixelRatio: devicePixelRatioText.text
    property bool noDraw;
    property bool isCompute;
    property bool frustumCulling;
    property real clearDepthValue;
    property real clearStencilValue;
    property alias renderCommands: renderCommandsListView.model

    width: background.width + renderCommandsFlickable.implicitWidth
    height: background.height

    Rectangle {
        id: background
        anchors.fill: col
        anchors.margins: -15
        color: "#1D628B"
        gradient: Gradient {
            GradientStop { color: background.color; position: 0.0}
            GradientStop { color: Qt.darker(background.color, 1.25); position: 1.0}
        }
    }

    Column {
        id: col
        anchors.centerIn: parent
        spacing: 15
        Row {
            spacing: 25
            Text { text: "Viewport"; color: "white"; font.family: robotoFont.name }
            Text { id: viewportText; color: "white"; font.family: robotoFont.name }
        }
        Row {
            spacing: 25
            Text { text: "SurfaceSize"; color: "white"; font.family: robotoFont.name }
            Text { id: surfaceSizeText; color: "white"; font.family: robotoFont.name  }
        }
        Row {
            spacing: 25
            Text { text: "DevicePixelRatio"; color: "white"; font.family: robotoFont.name  }
            Text { id: devicePixelRatioText; color: "white"; font.family: robotoFont.name  }
        }
        Row {
            spacing: 25
            Text { text: "NoDraw"; color: "white"; font.family: robotoFont.name  }
            Text { text: noDraw; color: "white"; font.family: robotoFont.name  }
        }
        Row {
            spacing: 25
            Text { text: "Compute"; color: "white"; font.family: robotoFont.name  }
            Text { text: isCompute; color: "white"; font.family: robotoFont.name  }
        }
        Row {
            spacing: 25
            Text { text: "FrustumCulling"; color: "white"; font.family: robotoFont.name  }
            Text { text: frustumCulling; color: "white"; font.family: robotoFont.name  }
        }
        Row {
            spacing: 25
            Text { text: "ClearDepthValue"; color: "white"; font.family: robotoFont.name  }
            Text { text: clearDepthValue; color: "white"; font.family: robotoFont.name  }
        }
        Row {
            spacing: 25
            Text { text: "ClearStencilValue"; color: "white"; font.family: robotoFont.name  }
            Text { text: clearStencilValue; color: "white"; font.family: robotoFont.name }
        }
    }

    Flickable {
        id: renderCommandsFlickable
        anchors {
            left: col.right
            leftMargin: 15
            top: root.top
            bottom: root.bottom
            right: root.right
        }

        contentWidth: rowContent.width
        contentHeight: rowContent.height

        Row {
            id: rowContent
            Repeater {
                id: renderCommandsListView
                Rectangle {
                    id: renderCommandDel
                    width: commandCol.implicitWidth + 20
                    height: root.height
                    color: model.index % 2 ? "#1c93d7" : "#1cc2d7"
                    gradient: Gradient {
                        GradientStop { color: renderCommandDel.color; position: 0.0}
                        GradientStop { color: Qt.darker(renderCommandDel.color, 1.25); position: 1.0}
                    }

                    Column {
                        id: commandCol
                        anchors.centerIn: parent
                        spacing: 15
                        Row {
                            spacing: 25
                            Text { text: "InstanceCount"; color: "white"; font.family: robotoFont.name }
                            Text { text: model.InstanceCount; color: "white"; font.family: robotoFont.name }
                        }
                        Row {
                            spacing: 25
                            Text { text: "Shader"; color: "white"; font.family: robotoFont.name }
                            Text { text: model.Shader; color: "white"; font.family: robotoFont.name  }
                        }
                        Row {
                            spacing: 25
                            Text { text: "VAO"; color: "white"; font.family: robotoFont.name  }
                            Text { text: model.Vao; color: "white"; font.family: robotoFont.name  }
                        }
                        Row {
                            spacing: 25
                            Text { text: "Geometry"; color: "white"; font.family: robotoFont.name  }
                            Text { text: model.Geometry; color: "white"; font.family: robotoFont.name  }
                        }
                        Row {
                            spacing: 25
                            Text { text: "GeometryRenderer"; color: "white"; font.family: robotoFont.name  }
                            Text { text: model.GeometryRenderer; color: "white"; font.family: robotoFont.name  }
                        }
                    }
                }
            }
        }
    }
}
