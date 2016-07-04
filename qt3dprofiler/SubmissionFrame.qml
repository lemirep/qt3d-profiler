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
import Profiler 1.0

Item {
    id: root
    property alias threadsModel: threadRepeater.model
    property int threadCount
    property int frameId
    property int frameType
    property real totalDuration // in ms
    property real startTime // in ms
    property real timeSinceLastFrame // in ms

    readonly property int totalFrameTime: timeSinceLastFrame + totalDuration

    // Do not specify a size for submission as these can be in between 2 frames
    width: Math.max(childrenRect.width, totalFrameTime * Singleton.msecToPixelScale)
    height: childrenRect.height

    Repeater {
        id: threadRepeater
        Thread {
            jobModel: model.JobModel
            threadId: model.Id
            x: timeSinceLastFrame * Singleton.msecToPixelScale
        }
    }
}
