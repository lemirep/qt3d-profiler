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

Item {
    id: frameView
    readonly property int barHeight: 50
    implicitWidth: mainRoot.width
    height: childrenRect.height

    Column {
        id: sideBar
        anchors {
            left: parent.left
            leftMargin: 25
        }

        StyledRectText {
            text: "Frame"
            rectColor: "#F89406"
            visible: Singleton.threadCount > 0
        }
        spacing: 5
        Repeater {
            model: Singleton.threadCount
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
            model: Singleton.threadCount
            Rectangle {
                color: model.index % 2 ? "#074354" : "#053542"
                height: frameView.barHeight
                width: backgroundBandColumn.width
            }
        }
    }

    property JobHighlight jobHighLight: JobHighlight {
        parent: frameView
        visible: false
    }


    ScrollView {
        id: scrollView
        anchors {
            left: backgroundBandColumn.left
            right: backgroundBandColumn.right
            top: sideBar.top
        }
        height: sideBar.height + 50

        ListView {
            id: listView
            orientation: ListView.Horizontal
            model: Singleton.jobStatsModel

            highlightMoveDuration: -1
            highlightMoveVelocity: -1
            spacing: 10
            delegate:
                Frame {
                id: delRoot
                threadsModel: model.ThreadsModel
                threadCount: model.ThreadCount
                frameId: model.Id
                totalDuration: model.TotalDuration
            }
            focus: true
        }
        flickableItem.interactive: true
        style: ScrollViewStyle {
            frame: Item {}
            corner: Item {}
            decrementControl: Item {}
            incrementControl: Item {}
            handle:Item {
                implicitWidth: 14
                implicitHeight: 26
                Rectangle {
                    color: "#424246"
                    radius: 10
                    width: 40
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                }
            }
            scrollBarBackground: Item {
                implicitWidth: 14
                implicitHeight: 26
            }
        }
    }

    ComboBox {
        id: frameComboBox
        model: Singleton.jobStatsModel
        visible: Singleton.threadCount > 0
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: scrollView.bottom
        }
        textRole: "Id"
        onCurrentIndexChanged: listView.currentIndex = frameComboBox.currentIndex
    }
//    ListView {
//        height: frameView.barHeight
//        spacing: 5
//        model: Singleton.jobStatsModel
//        orientation: ListView.Horizontal
//        delegate: StyledRectText {
//            text: "Frame " + (model.Id)
//            rectColor: "#E91818"

//        }
//    }

}
