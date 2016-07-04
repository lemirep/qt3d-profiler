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
import QtQuick.Dialogs 1.0
import QtQuick.Window 2.0
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4
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
            spacing: 20
            width: mainRoot.width

            Column {
                // TO DO: Allow to load multiple traces to perform comparison
                id: frameJobViews
                width: mainRoot.width
                spacing: 25

                add: Transition {
                    NumberAnimation { property: "opacity"; from: 0; to: 1.0; duration: 400 }
                    NumberAnimation { property: "scale"; from: 0; to: 1.0; duration: 400 }
                }

                move: Transition {
                    NumberAnimation { property: "opacity"; from: 0; to: 1.0; duration: 400 }
                    NumberAnimation { property: "scale"; from: 0; to: 1.0; duration: 400 }
                }

                Repeater {
                    id: jobTracesRepeater
                    model: Singleton.jobTracesModel
                    FrameJobView {
                        threadCount: model.ThreadCount
                        workerJobStatsModel: model.WorkerJobFramesModel
                        submissionJobStatsModel: model.SubmissionJobFramesModel
                        frameTotalDuration: model.TotalDuration
                        frameTitle: model.Title
                    }
                }
            }

            Item {
                height: addTraceButton.height
                anchors {
                    left: parent.left
                    right: parent.right
                    leftMargin: 25
                    rightMargin: 25
                }
                Item {
                    width: frameJobViews.width
                    height: childrenRect.height + 15
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    visible: jobTracesRepeater.count > 0
                    Column {

                        Text {
                            text: "1 ms === " + zoomSlider.value + "px"
                            color: "white"
                            anchors.horizontalCenter: zoomSlider.horizontalCenter
                        }
                        Slider {
                            id: zoomSlider
                            from: 50
                            to: 500
                            width: 400
                            value: Singleton.msecToPixelScale
                            onValueChanged: Singleton.msecToPixelScale = value
                        }
                    }
                }

                AddButton {
                    id: addTraceButton
                    onClicked: fileDialog.visible = true
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
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
