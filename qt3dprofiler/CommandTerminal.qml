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

import QtQuick 2.7
import QtQuick.Controls 2.0
import Profiler 1.0

FocusScope {
    id: root
    signal commandEntered(string command)
    property alias entryEnabled: commandInput.enabled

    Rectangle {
        color: "#1e1e27"
        border {
            width: 1
            color: "grey"
        }
        anchors.fill: parent

        Flickable {
            anchors.fill: parent
            contentWidth: col.width
            contentHeight: col.height
            clip: true

            onContentHeightChanged: {
                console.log(originY, contentX, contentHeight)
                flick(0, -2.0 * contentHeight)
            }

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
                anchors.left: parent.left
                anchors.leftMargin: 10

                Repeater {
                    id: historyCommands
                    model: Singleton.commandDisplayModel
                    Text {
                        font.pointSize: 15
                        color: "grey"
                        text: model.Content
                        font.family: robotoFont.name
                    }
                }

                Row {
                    Text {
                        text: "qt3d:> "
                        color: "grey"
                        font.family: robotoFont.name
                        font.pointSize: 15
                    }
                    TextInput {
                        id: commandInput
                        color: "grey"
                        font.pointSize: 15
                        width: root.width - 100
                        focus: true
                        font.family: robotoFont.name
                        cursorDelegate: Item {
                            width: 2
                            Rectangle {
                                height: 5
                                width: 10
                                anchors.bottom: parent.bottom
                                SequentialAnimation on opacity {
                                    OpacityAnimator { duration: 500; to: 0; }
                                    OpacityAnimator { duration: 500; to: 1; }
                                    loops: Animation.Infinite
                                }
                            }
                        }

                        property int lastCommandIdx: 0

                        Keys.onReleased: {
                            if (event.key === Qt.Key_L && (event.modifiers & Qt.ControlModifier))
                                Singleton.commandDisplayModel.clear()
                            if (event.key === Qt.Key_Escape && expanded)
                                expanded = false
                            if (event.key === Qt.Key_Up && historyCommands.count > 0) {
                                lastCommandIdx -= 1
                                if (lastCommandIdx < 0)
                                    lastCommandIdx = historyCommands.count - 1
                                text = historyCommands.itemAt(lastCommandIdx).text.replace("qt3d:>", "").replace("No such command:", "")
                            }
                        }

                        onAccepted: {
                            // Send command to be processed
                            root.commandEntered(text)
                            text = ""
                        }
                    }
                }
            }
        }

        MouseArea {
            anchors.fill: parent
            enabled: !commandInput.activeFocus
            onClicked: {
                commandInput.forceActiveFocus()
            }
        }
    }
}

