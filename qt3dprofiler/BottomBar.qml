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

import QtQuick 2.5
import Profiler 1.0

Item {
    id: root
    property bool expanded: false
    property int lastHeight: 350
    readonly property int closedHeight: 35
    height: !expanded ? closedHeight : closedHeight + lastHeight
    y: !expanded ? mainRoot.height - closedHeight : mainRoot.height - (closedHeight + lastHeight)

    MouseArea {
        enabled: expanded
        drag.target: root
        drag.axis: Drag.YAxis
        anchors.fill: parent
        drag.minimumY: 75
        drag.maximumY: mainRoot.height
        onPositionChanged: {
            if (drag.active)
                lastHeight = mainRoot.height - root.y - closedHeight
        }
    }

    Behavior on height { NumberAnimation { duration: 750; easing.type: Easing.InOutQuad } }

    Column {
        id: col
        width: parent.width

        Rectangle {
            id: top
            color: "#333333"
            anchors {
                left: parent.left
                right: parent.right
            }
            height: closedHeight

            Text {
                text: "V"
                font.pointSize: 18
                color: "grey"
                anchors {
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                    rightMargin: 15
                }
                rotation: expanded ? 0 : 180
                Behavior on rotation { NumberAnimation { duration: 500 } }
                MouseArea {
                    anchors.fill: parent
                    onClicked: expanded = !expanded
                }
            }

            Row {
                anchors {
                    left: parent.left
                    right: parent.right
                    leftMargin: 15
                    verticalCenter: parent.verticalCenter
                }
                spacing: 10
                Rectangle {
                    id: connectionIndicator
                    color: Singleton.debuggerConnection.connected ? "green" : "red"
                    Behavior on color { ColorAnimation { duration: 500 } }
                    width: 15
                    height: width
                    radius: width * 0.5
                    gradient: Gradient {
                        GradientStop { color: connectionIndicator.color; position: 0.0}
                        GradientStop { color: Qt.darker(connectionIndicator.color, 1.25); position: 1.0}
                    }
                }
                Text {
                    font.family: robotoFont.name
                    color: "white"
                    text: Singleton.debuggerConnection.connected ? "Connected" : "Disconnected"
                }
            }
        }

        CommandTerminal {
            id: terminal
            anchors {
                left: parent.left
                right: parent.right
            }
            height: expanded ? lastHeight : 0
            entryEnabled: Singleton.debuggerConnection.connected
            visible: height > 0
            onCommandEntered: {
                Singleton.executeCommand(command);
            }
        }
    }
}
