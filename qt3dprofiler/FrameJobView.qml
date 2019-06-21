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

    readonly property int barHeight: 35
    readonly property real nsecToMSec: 0.000001

    property int threadCount
    property real frameTotalDuration
    property real frameStartTime
    property alias jobsModel: jobTraceView.sourceModel
    property alias frameTitle: titleText.text

    onFrameStartTimeChanged: console.log("FrameStartTime " + frameStartTime)

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
                id: measurerButton
                color: "#387ee0"
                width: 100
                height: 30
                anchors {
                    right: removeButton.left
                    rightMargin: 15
                    verticalCenter: parent.verticalCenter
                }

                text: ":-:-:"
                onClicked: measurer.visible = !measurer.visible
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

    property alias jobHighLight: _jobHightLight

    JobHighlight {
        id: _jobHightLight
        parent: root
        visible: false
        z: 100
    }

    // Show time bar
    Item {
        id: timeBar
        anchors {
            left: backgroundBandColumn.left
            right: backgroundBandColumn.right
            top: sideBar.top
        }
        height: frameView.barHeight
        readonly property real startTime: ((jobsFlickable.contentX / Singleton.msecToPixelScale) + frameStartTime * nsecToMSec).toFixed(1)
        readonly property real endTime: (((jobsFlickable.contentX + parent.width) / Singleton.msecToPixelScale) + frameStartTime * nsecToMSec).toFixed(1)

        Rectangle {
            id: timeBackground
            color: "white"
            anchors.fill: parent
            gradient: Gradient {
                GradientStop { color: Qt.lighter(timeBackground.color, 1); position: 0.0}
                GradientStop { color: Qt.darker(timeBackground.color, 1.25); position: 1.0}
            }

            Repeater {
                id: timeGraduationsRepeater
                model: Math.floor(timeBar.endTime - timeBar.startTime) * 2.0
                readonly property real offset: (Math.floor(timeBar.startTime) - timeBar.startTime) * Singleton.msecToPixelScale
                readonly property real spacing: (timeBackground.width / timeGraduationsRepeater.model)
                Rectangle {
                    color: "black"
                    x: timeGraduationsRepeater.offset + model.index * timeGraduationsRepeater.spacing
                    height: model.index % 2 === 0 ? timeBackground.height * 0.4 : timeBackground.height * 0.2
                    anchors.bottom: parent.bottom
                    width: model.index % 2 === 0 ? 2 : 1
                }
            }

            // Duration bars (overdrawn outside of this element
            Repeater {
                id: durationBarRepeater
                model: timeGraduationsRepeater.model * 0.5
                Rectangle {
                    width: 1
                    height: jobsFlickable.height
                    color: "#338b8b8b"
                    x: timeGraduationsRepeater.offset + model.index * timeGraduationsRepeater.spacing * 2.0
                }
            }
        }

        Text {
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.leftMargin: 20
            text: timeBar.startTime + " ms"
            color: "white"
            font.bold: true
            font.pointSize: 16
            font.italic: true
            font.family: robotoFont.name
            style: Text.Outline
            styleColor: "black"
        }
        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            text: ((timeBar.startTime + timeBar.endTime) * 0.5).toFixed(1) + " ms"
            color: "white"
            font.bold: true
            font.pointSize: 16
            font.italic: true
            font.family: robotoFont.name
            style: Text.Outline
            styleColor: "black"
        }
        Text {
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.rightMargin: 20
            text: timeBar.endTime + " ms"
            color: "white"
            font.bold: true
            font.pointSize: 16
            font.italic: true
            font.family: robotoFont.name
            style: Text.Outline
            styleColor: "black"
        }
    }

    Flickable {
        id: jobsFlickable
        anchors {
            left: backgroundBandColumn.left
            right: backgroundBandColumn.right
            top: sideBar.top
        }
        height: sideBar.height + 10 // Offset for scrollbar
        contentWidth: jobTraceView.width
        contentHeight: height
        clip: true

        ScrollBar.horizontal: ScrollBar {
            id: control
            orientation: Qt.Horizontal
            contentItem: Rectangle {
                implicitHeight: 6
                implicitWidth: 100
                radius: height / 2
                color: control.pressed ? "#81e889" : "#c2f4c6"
            }
            background: Rectangle {
                color: "#333333"
            }
        }

        // Show the jobs
        JobTraceView {
            id: jobTraceView
            anchors {
                top: parent.top
                bottom: parent.bottom
            }

            frameStartTime: frameView.frameStartTime
            msecToPixelScale: Singleton.msecToPixelScale
            frameTotalDuration: frameView.frameTotalDuration
            viewWidth: col.width
            viewContentX: jobsFlickable.contentX

            Repeater {
                id: workerJobListView
                model: jobTraceView.visibleJobsModel
                anchors.fill: parent

                delegate: Job {
                    xPos: model.X
                    start: model.FrameStart
                    end: model.FrameEnd
                    type: model.Type
                    instance: model.InstanceId
                    color: model.Color
                    name: model.Name
                    relativeStart: model.RelativeStart
                    relativeEnd: model.RelativeEnd
                    threadId: model.ThreadId
                }
            }
        }
    }

    // Time measurement
    Item {
        id: measurer
        visible: false
        readonly property real startTime: timeBar.startTime + ((timeBar.endTime - timeBar.startTime) * ((leftHandle.x + leftHandle.width) / measurer.width))
        readonly property real endTime: timeBar.startTime + ((timeBar.endTime - timeBar.startTime) * ((rightHandle.x) / measurer.width))

        anchors {
            left: backgroundBandColumn.left
            right: backgroundBandColumn.right
            top: sideBar.top
            bottom: sideBar.bottom
        }

        Rectangle {
            id: leftHandle
            width: 8
            x: parent.width * 0.5 - width
            color: leftHandleMa.containsMouse ? Qt.lighter("#464650", 1.5) :  "#464650"
            anchors {
                top: parent.top
                bottom: parent.bottom
            }
            MouseArea {
                id: leftHandleMa
                drag.target: parent
                drag.axis: Drag.XAxis
                anchors.fill: parent
                drag.maximumX: rightHandle.x - width
                drag.minimumX: -width
                hoverEnabled: true
            }
            Rectangle {
                color: "#442144"
                anchors {
                    top: parent.top
                    bottom: parent.bottom
                    right: parent.right
                }
                width: 1
            }
            Text {
                z: 1
                anchors.centerIn: parent
                anchors.verticalCenterOffset: -30
                color: "white"
                font.bold: true
                font.pointSize: 16
                font.italic: true
                font.family: robotoFont.name
                style: Text.Outline
                styleColor: "black"
                text: measurer.startTime.toFixed(1) + " ms"
            }
        }

        Rectangle {
            color: "#55454549"
            anchors {
                left: leftHandle.right
                right: rightHandle.left
                top: leftHandle.top
                bottom: leftHandle.bottom
            }
            Text {
                anchors.centerIn: parent
                color: "white"
                font.bold: true
                font.pointSize: 16
                font.italic: true
                font.family: robotoFont.name
                style: Text.Outline
                styleColor: "black"
                text: (measurer.endTime - measurer.startTime).toFixed(3) + " ms"
            }
        }

        Rectangle {
            id: rightHandle
            x: parent.width * 0.5
            width: 8
            color: rightHandleMa.containsMouse ? Qt.lighter("#464650", 1.5) :  "#464650"
            anchors {
                top: parent.top
                bottom: parent.bottom
            }
            MouseArea {
                id: rightHandleMa
                drag.target: parent
                drag.axis: Drag.XAxis
                anchors.fill: parent
                drag.maximumX: measurer.width
                drag.minimumX: leftHandle.x + width
                hoverEnabled: true
            }
            Rectangle {
                color: "#442144"
                anchors {
                    top: parent.top
                    bottom: parent.bottom
                    left: parent.left
                }
                width: 1
            }
            Text {
                anchors.centerIn: parent
                anchors.verticalCenterOffset: +30
                color: "white"
                font.bold: true
                font.pointSize: 16
                font.italic: true
                font.family: robotoFont.name
                style: Text.Outline
                styleColor: "black"
                text: measurer.endTime.toFixed(1) + " ms"
                z: 1
            }
        }

    }

}
