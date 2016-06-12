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

Rectangle {
    id: root
    property int start
    property int end
    property int type
    property int instance
    readonly property real nsecToMSec: 0.000001
    readonly property real duration: (end - start) * nsecToMSec
    property bool collapsed: false
    property string name

    height: frameView.barHeight * 0.5 - 1
    y: model.index % 2 ? frameView.barHeight * 0.5 : 1
    readonly property int maxWidth: Math.max(Singleton.msecToPixelScale * duration, 2)
    width: collapsed ? Math.min(maxWidth, 200) : maxWidth
    x: start * nsecToMSec * Singleton.msecToPixelScale

    gradient: Gradient {
        GradientStop { color: Qt.lighter(root.color, ma.containsMouse ? 1.25 : 1); position: 0.0}
        GradientStop { color: Qt.darker(root.color, 1.25); position: 1.0}
    }

    Text {
        id: txt
        color: "white"
        text: duration.toFixed(2) + " ms"
        anchors.centerIn: parent
        font.bold: true
        width: parent.width - 25
        horizontalAlignment: Text.AlignHCenter
        elide: Text.ElideRight
        font.family: robotoFont.name
    }
    MouseArea {
        id: ma
        anchors.fill: parent
        hoverEnabled: true
        onClicked: {
            collapsed = !collapsed
        }
        onEntered: {
            var pos = pageListView.mapFromItem(ma, ma.mouseX, ma.mouseY)
            frameView.jobHighLight.x = pos.x + 15
            frameView.jobHighLight.y = pos.y + 15
            frameView.jobHighLight.visible = true
            frameView.jobHighLight.color = root.color
            frameView.jobHighLight.start = root.start
            frameView.jobHighLight.end = root.end
            frameView.jobHighLight.type = root.type
            frameView.jobHighLight.instance = root.instance
            frameView.jobHighLight.duration = root.duration
            frameView.jobHighLight.name = root.name
        }
        onExited: {
            frameView.jobHighLight.visible = false
        }
    }
}
