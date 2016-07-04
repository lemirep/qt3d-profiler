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

Item {
    id: frameView
    implicitWidth: mainRoot.width
    height: childrenRect.height

    readonly property int barHeight: 50
    readonly property real nsecToMSec: 0.000001

    property int threadCount
    property real frameTotalDuration
    property alias workerJobStatsModel: workerJobListView.model
    property alias submissionJobStatsModel: submissionJobListView.model
    property alias frameTitle: titleText.text

    Item {
        id: topBar
        anchors {
            left: parent.left
            leftMargin: 25
            right: parent.right
            rightMargin: 25
        }
        height: 55

        Rectangle {
            id: rect
            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
                bottom: parent.bottom
                bottomMargin: 10
            }

            color: "#3498db"

            gradient: Gradient {
                GradientStop { color: Qt.lighter(rect.color, 1); position: 0.0}
                GradientStop { color: Qt.darker(rect.color, 1.25); position: 1.0}
            }
            TextInput {
                id: titleText
                anchors {
                    left: parent.left
                    leftMargin: 15
                    verticalCenter: parent.verticalCenter
                }
                font.family: robotoFont.name
                width: parent.width - 30
                wrapMode: Text.WrapAnywhere
                color: "white"
                font.pointSize: 15
                onAccepted: focus = false
            }

            Row {
                id: bottomBar
                anchors.top: titleText.baseline
                anchors.left: titleText.left
                anchors.leftMargin: -5
                visible: width > 2
                readonly property color barColor: "#f39c12"
                Rectangle {
                    height: 10
                    width: 1
                    color: bottomBar.barColor
                }
                Rectangle {
                    anchors.top: parent.bottom
                    width: titleText.focus ? titleText.implicitWidth + 10 : 0
                    Behavior on width { NumberAnimation { duration: 1500; easing.type: Easing.OutQuad} }
                    height: 1
                    color: bottomBar.barColor
                }
                Rectangle {
                    height: 10
                    width: 1
                    color: bottomBar.barColor
                }
            }

            AddButton {
                id: removeButton
                color: "#e0c438"
                width: 30
                anchors {
                    right: parent.right
                    rightMargin: 15
                    verticalCenter: parent.verticalCenter
                }

                text: "-"
                onClicked: Singleton.removeTrace(model.index);
            }
        }
    }

    Column {
        id: sideBar
        anchors {
            left: parent.left
            top: topBar.bottom
            leftMargin: 25
        }

        StyledRectText {
            text: "Frame"
            rectColor: "#F89406"
            visible: threadCount > 0
        }
        spacing: 5
        Repeater {
            model: threadCount
            StyledRectText {
                text: "Thread " + (model.index + 1)
                rectColor: "grey"
            }
        }
        z: 1
    }

    Column {
        id: backgroundBandColumn
        spacing: 5
        anchors {
            left: sideBar.right
            leftMargin: 5
            right: parent.right
            rightMargin: 25
            top: sideBar.top
            topMargin: barHeight + 5
        }

        Repeater {
            model: threadCount
            Rectangle {
                color: model.index % 2 ? "#074354" : "#053542"
                height: frameView.barHeight
                width: backgroundBandColumn.width
            }
        }
    }

    property JobHighlight jobHighLight: JobHighlight {
        parent: pageListView
        visible: false
        z: 5
    }

    Column {
        anchors {
            left: backgroundBandColumn.left
            right: backgroundBandColumn.right
            top: sideBar.top
        }


        Item {
            anchors {
                left: parent.left
                right: parent.right
            }
            height: sideBar.height
            clip: true
            ListView {
                id: submissionJobListView
                anchors.fill: parent
                orientation: ListView.Horizontal
                preferredHighlightBegin: 0
                preferredHighlightEnd: width
                highlightRangeMode: ListView.ApplyRange
                highlightMoveDuration: -1
                highlightMoveVelocity: -1
                spacing: 10
                delegate:  SubmissionFrame {
                    threadsModel: model.ThreadsModel
                    threadCount: model.ThreadCount
                    frameId: model.Id
                    totalDuration: model.TotalDurationMS
                    startTime: model.StartTimeMS
                    timeSinceLastFrame: model.TimeSinceLastFrameMS
                }
                contentX: workerJobListView.contentX
                currentIndex: workerJobListView.currentIndex
            }
            ListView {
                id: workerJobListView
                anchors.fill: parent
                orientation: ListView.Horizontal
                preferredHighlightBegin: 0
                preferredHighlightEnd: width
                highlightRangeMode: ListView.ApplyRange
                highlightMoveDuration: -1
                highlightMoveVelocity: -1
                spacing: 10
                delegate:  JobFrame {
                    threadsModel: model.ThreadsModel
                    threadCount: model.ThreadCount
                    frameId: model.Id
                    totalDuration: model.TotalDurationMS
                    startTime: model.StartTimeMS
                    timeSinceLastFrame: model.TimeSinceLastFrameMS
                }
            }
        }

        ListView {
            z: -1
            height: 35
            anchors {
                left: parent.left
                right: parent.right
            }
            clip: true
            model: workerJobListView.model.rowCount()
            orientation: ListView.Horizontal
            currentIndex: workerJobListView.currentIndex
            delegate: Rectangle {
                id: smallDel
                height: 35
                width: 50
                gradient: Gradient {
                    GradientStop { color: Qt.lighter(smallDel.color, 1); position: 0.0}
                    GradientStop { color: Qt.darker(smallDel.color, 1.25); position: 1.0}
                }

                Text {
                    anchors.centerIn: parent
                    text: model.index
                    color: parent.ListView.isCurrentItem ? "red" : "black"
                    font.family: robotoFont.name
                }
                Rectangle {
                    anchors {
                        top: parent.top
                        left: parent.left
                    }
                    height: parent.height * 0.5
                    width: 1
                    color: "black"
                }
                Rectangle {
                    anchors {
                        top: parent.top
                        horizontalCenter: parent.horizontalCenter
                    }
                    height: parent.height * 0.25
                    width: 1
                    color: parent.ListView.isCurrentItem ? "red" : "black"
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: workerJobListView.currentIndex = model.index
                }
            }

            highlightRangeMode: ListView.ApplyRange
            preferredHighlightBegin: 0
            preferredHighlightEnd: width
            highlightMoveDuration: -1
            highlightMoveVelocity: -1
        }
    }
}
