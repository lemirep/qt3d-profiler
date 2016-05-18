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
    height: Screen.desktopAvailableWidth
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

    function colorForType(type)
    {
        switch (type) {
        case Singleton.LoadBuffer:
            return "#1abc9c"
        case Singleton.FrameCleanup:
            return "#2ecc71"
        case Singleton.FramePreparation:
            return "#3498db"
        case Singleton.CalcBoundingVolume:
            return "#9b59b6"
        case Singleton.CalcTriangleVolume:
            return "#34495e"
        case Singleton.LoadGeometry:
            return "#f1c40f"
        case Singleton.LoadScene:
            return "#e67e22"
        case Singleton.LoadTextureData:
            return "#e74c3c"
        case Singleton.PickBoundingVolume:
            return "#bdc3c7"
        case Singleton.RenderView:
            return "#95a5a6"
        case Singleton.UpdateTransform:
            return "#f39c12"
        case Singleton.UpdateBoundingVolume:
            return "#d35400"
        case Singleton.FrameSubmission:
            return "#1f3a96"
        case Singleton.LayerFiltering:
            return "#453E4A"
        case Singleton.EntityComponentTypeFiltering:
            return "#E6567A"
        case Singleton.MaterialParameterGathering:
            return "#816432"
        case Singleton.RenderViewBuilder:
            return "#025159"
        case Singleton.GenericLambda:
            return "#F7E999"
        case Singleton.FrustumCulling:
            return "#1DABB8"
        case Singleton.LightGathering:
            return "#F9690E"
        default:
            return "red"
        }
    }

    function jobNameForType(type)
    {
        switch (type) {
        case Singleton.LoadBuffer:
            return "LoadBuffer"
        case Singleton.FrameCleanup:
            return "FrameCleanup"
        case Singleton.FramePreparation:
            return "FramePreparation"
        case Singleton.CalcBoundingVolume:
            return "CalcBoundingVolume"
        case Singleton.CalcTriangleVolume:
            return "CalcTriangleVolume"
        case Singleton.LoadGeometry:
            return "LoadGeometry"
        case Singleton.LoadScene:
            return "LoadScene"
        case Singleton.LoadTextureData:
            return "LoadTextureData"
        case Singleton.PickBoundingVolume:
            return "PickBoundingVolume"
        case Singleton.RenderView:
            return "RenderView"
        case Singleton.UpdateTransform:
            return "UpdateTransform"
        case Singleton.UpdateBoundingVolume:
            return "UpdateBoundingVolume"
        case Singleton.FrameSubmission:
            return "FrameSubmission"
        case Singleton.LayerFiltering:
            return "LayerFiltering"
        case Singleton.EntityComponentTypeFiltering:
            return "EntityComponentTypeFiltering"
        case Singleton.MaterialParameterGathering:
            return "MaterialParameterGathering"
        case Singleton.RenderViewBuilder:
            return "RenderViewBuilder"
        case Singleton.GenericLambda:
            return "GenericLambdaSync"
        case Singleton.FrustumCulling:
            return "FrustumCulling"
        case Singleton.LightGathering:
            return "LightGathering"
        default:
            return "Unknown"
        }
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

    FrameJobView {
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            top: traceSelector.bottom
        }
    }

    Column {
        anchors {
            left: parent.left
            bottom: parent.bottom
            bottomMargin: 25
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

    Grid {
        id: g
        spacing: 20
        rows: 2
        anchors {
            horizontalCenter: parent.horizontalCenter
            bottom: parent.bottom
            bottomMargin: 25
        }

        Repeater {
            model: Singleton.MaxType
            Row {
                spacing: 5
                Rectangle {
                    id: colorIndicator
                    color: colorForType(model.index)
                    width: 15
                    height: 15
                    gradient: Gradient {
                        GradientStop { color: colorIndicator.color; position: 0.0}
                        GradientStop { color: Qt.darker(colorIndicator.color, 1.25); position: 1.0}
                    }
                }
                Text {
                    anchors.verticalCenter: colorIndicator.verticalCenter
                    text: jobNameForType(model.index)
                    color: "white"
                }
            }
        }
    }
}
