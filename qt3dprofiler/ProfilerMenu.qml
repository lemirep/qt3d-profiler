import QtQuick 2.5

Rectangle {
    id: root
    height: childrenRect.height
    radius: 2

    property int currentIndex: 0
    color: "#3498db"
    gradient: Gradient {
        GradientStop { color: Qt.lighter(root.color, 1.0); position: 0.0}
        GradientStop { color: Qt.darker(root.color, 1.25); position: 1.0}
    }

    signal clicked(int idx, string name)

    readonly property ListModel model: ListModel {
        ListElement { text: "Jobs" }
        ListElement { text: "Rendering" }
        ListElement { text: "FrameGraph" }
    }

    Row {
        anchors.left: parent.left
        Repeater {
            model: root.model
            Item {
                width: title.implicitWidth + 100
                height: title.implicitHeight + 20
                Text {
                    id: title
                    text: model.text
                    color: "white"
                    font.pointSize: 18
                    anchors.centerIn: parent
                    font.family: robotoFont.name
                }
                Rectangle {
                    color: "#BBA900"
                    anchors {
                        left: parent.left
                        right: parent.right
                        bottom: parent.bottom
                    }
                    height: 5
                    visible: model.index === currentIndex
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        root.clicked(model.index, model.text)
                    }
                }
                Rectangle {
                    gradient: Gradient {
                        GradientStop { color: Qt.darker(root.color, 1.25); position: 0.0}
                        GradientStop { color: Qt.darker(root.color, 1.5); position: 1.0}
                    }
                    anchors {
                        right: parent.right
                        top: parent.top
                        bottom: parent.bottom
                    }
                    width: 1
                    visible: model.index + 1 < root.model.count
                }
            }
        }
    }
}
