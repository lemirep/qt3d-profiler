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
import QtQuick.Dialogs 1.0
import QtQuick.Window 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import Profiler 1.0

Window {
    id: mainRoot
    visible: true
    width: Screen.desktopAvailableWidth
    height: Screen.desktopAvailableHeight
    color: "#22313f"

    Text {
        anchors {
            right: parent.right
            bottom: parent.bottom
            margins: 25
        }
        font.italic: true
        font.pointSize: 25
        color: "white"
        text: "Qt3D Profiler"
    }



    FileDialog {
        id: fileDialog
        title: "Open a Trace File"
        width: 600
        height: 400
        nameFilters: [ "Qt3D trace file (*.qt3d)" ]
        onAccepted: Singleton.readTraceFile(fileDialog.fileUrl)
    }

    TraceSelector {
        id: traceSelector
        width: parent.width * 0.4
        height: 50
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: parent.top
            topMargin: 50
        }
        text: fileDialog.fileUrl
        onClicked: fileDialog.visible = true
    }


    ScrollView {
        anchors {
            top: traceSelector.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }

        Column {
            spacing: 20

            Column {
                // TO DO: Allow to load multiple traces to perform comparison
                FrameJobView {
                    id: frameJobView
                }
            }

            Item {
                width: frameJobView.width
                height: childrenRect.height + 15
                Column {
                    anchors {
                        left: parent.left
                        leftMargin: 25
                    }
                    Text {
                        text: "1 ms === " + zoomSlider.value + "px"
                        color: "white"
                        anchors.horizontalCenter: zoomSlider.horizontalCenter
                    }
                    Slider {
                        id: zoomSlider
                        maximumValue: 5000
                        minimumValue: 50
                        width: 400
                        value: Singleton.msecToPixelScale
                        onValueChanged: Singleton.msecToPixelScale = value
                    }
                }
            }

            JobLegend {
                spacing: 10
                anchors {
                    horizontalCenter: parent.horizontalCenter
                }
                height: 400
            }
        }
    }
}
