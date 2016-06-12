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

Item {
    property bool expanded: false
    readonly property int closedHeight: 35
    height: expanded ? 350 : closedHeight

    Behavior on height { NumberAnimation { duration: 750; easing.type: Easing.InOutQuad } }

    Rectangle {
        color: "#333333"
        anchors {
            top: parent.top
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
    }
}
