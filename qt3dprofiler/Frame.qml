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
    property alias threadsModel: threadRepeater.model
    property int threadCount
    property int frameId
    property int totalDuration // in nsec

    width: childrenRect.width
    height: childrenRect.height

    // Duration bars
    Repeater {
        id: durationBarRepeater
        model: Math.floor(totalDuration * 0.000001) // -> to ms
        Rectangle {
            width: 1
            height: root.height
            color: "#338b8b8b"
            x: (model.index + 1) * Singleton.msecToPixelScale
        }
    }

    Rectangle {
        id: rect
        width: Math.max(parent.width, frameHeader.implicitWidth + 30)
        height: frameView.barHeight
        color: "#d35400"
        gradient: Gradient {
            GradientStop { color: Qt.lighter(rect.color, 1.25); position: 0.0}
            GradientStop { color: rect.color; position: 1.0}
        }
        Text {
            id: frameHeader
            anchors.left: parent.left
            anchors.leftMargin: 15
            color: "white"
            text: frameId
            lineHeight: 2
            font {
                pointSize: 25
                italic: true
                bold: true

            }
            style: Text.Outline
            styleColor: "black"
        }
    }

    Repeater {
        id: threadRepeater
        Thread {
            jobModel: model.JobModel
            threadId: model.Id
        }
    }

    Rectangle {
        anchors.left: parent.left
        height: (Singleton.threadCount + 1) * (frameView.barHeight + 5)
        width: 2
        z: -1
    }
}
