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
import QtQml.Models 2.2
import Profiler 1.0

Window {
    id: mainRoot
    visible: true
    width: Screen.desktopAvailableWidth
    height: Screen.desktopAvailableHeight
    color: "#22313f"

    FontLoader {
        id: robotoFont
        source: "Roboto-Regular.ttf"
    }


    Component.onCompleted: {
        Singleton.debuggerConnection.host = "127.0.0.1"
    }

    Text {
        anchors {
            right: parent.right
            bottom: parent.bottom
            margins: 50
        }
        font.italic: true
        font.pointSize: 25
        color: "white"
        text: "Qt3D Profiler"
        font.family: robotoFont.name
    }

    FileDialog {
        id: fileDialog
        title: "Open a Trace File"
        width: 600
        height: 400
        nameFilters: [ "Qt3D trace file (*.qt3d)" ]
        onAccepted: Singleton.addTraceFile(fileDialog.fileUrl)
    }

    ListView {
        id: pageListView
        model: VisualItemModel {
            JobTraceView { height: ListView.view.height; width: ListView.view.width }
            RenderViewInspector { height: ListView.view.height; width: ListView.view.width }
            Rectangle { color: "blue"; height: ListView.view.height; width: ListView.view.width }
        }
        anchors {
            top: menu.bottom
            left: parent.left
            right: parent.right
            bottom: bottomBar.top
            topMargin: 25
        }
        snapMode: ListView.SnapOneItem
        orientation: ListView.Horizontal
        preferredHighlightBegin: 0
        preferredHighlightEnd: width
        highlightRangeMode: ListView.StrictlyEnforceRange
        highlightMoveDuration: -1
        highlightMoveVelocity: -1
    }

    BottomBar {
        id: bottomBar
        anchors {
            left: parent.left
            right: parent.right
        }
    }

    ProfilerMenu {
        id: menu
        anchors {
            top: parent.top
            topMargin: 15
            left: parent.left
            right: parent.right
            rightMargin: 25
            leftMargin: 25
        }
        onClicked: pageListView.currentIndex = idx
        currentIndex: pageListView.currentIndex
    }
}
