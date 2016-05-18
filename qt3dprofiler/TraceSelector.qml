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
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

Item {
    id: root
    property color color: "#4d4dff"
    property alias text: text.text
    signal clicked()

    Row {
        id: bottomBar
        Rectangle {
            height: 10
            width: 1
            color: root.color
        }
        Rectangle {
            anchors.bottom: parent.bottom
            width: root.width
            height: 1
            color: root.color
        }
        Rectangle {
            height: 10
            width: 1
            color: root.color
        }
    }
    Text {
        id: text
        color: "white"
        width: parent.width - 25
        anchors {
            baseline: bottomBar.top
            left: bottomBar.left
            leftMargin: 12
        }
        elide: Text.ElideRight
    }

    Button {
        anchors {
            left: bottomBar.right
            bottom: bottomBar.bottom
            leftMargin: 10
        }
        onClicked: root.clicked();
        text: "Open trace"
        style: ButtonStyle {
            background: Rectangle {
                implicitWidth: 100
                implicitHeight: 25
                border.width: control.activeFocus ? 2 : 1
                border.color: "#888"
                radius: 4
                gradient: Gradient {
                    GradientStop { position: 0 ; color: control.pressed ? "#ccc" : "#eee" }
                    GradientStop { position: 1 ; color: control.pressed ? "#aaa" : "#ccc" }
                }
            }
        }
    }
}
