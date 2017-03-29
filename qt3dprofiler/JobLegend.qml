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
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import Profiler 1.0


Flow {
    spacing: 250
    width: childrenRect.width
    height: childrenRect.height

    Repeater {
        model: Singleton.aspectInfoModel

        Column {
            readonly property QtObject typeInfoModel: model.JobTypeInfoModel
            id: innerColumn
            width: childrenRect.width + 10
            height: childrenRect.height
            spacing: 10


            Text {
                color: "white"
                text: model.Name
                font {
                    capitalization: Font.Capitalize
                    pointSize: 20
                }
                font.family: robotoFont.name
            }

            Grid {
                id: g
                spacing: 20
                rows: 5

                Repeater {
                    model: typeInfoModel
                    Row {
                        spacing: 5
                        Rectangle {
                            id: colorIndicator
                            color: model.Color
                            width: 15
                            height: 15
                            gradient: Gradient {
                                GradientStop { color: colorIndicator.color; position: 0.0}
                                GradientStop { color: Qt.darker(colorIndicator.color, 1.25); position: 1.0}
                            }
                        }
                        Text {
                            anchors.verticalCenter: colorIndicator.verticalCenter
                            text: model.Name
                            color: "white"
                            font.family: robotoFont.name
                        }
                    }
                }
            }
        }
    }
}
