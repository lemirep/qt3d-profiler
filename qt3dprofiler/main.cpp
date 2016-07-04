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

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "profiler.h"
#include "debuggerconnection.h"

QObject *singletonProvider(QQmlEngine *, QJSEngine *)
{
    return new Profiler();
}

int main(int ac, char **av)
{
    QGuiApplication app(ac, av);
    QQmlApplicationEngine engine;
    qmlRegisterSingletonType<Profiler>("Profiler", 1, 0, "Singleton", &singletonProvider);
    qmlRegisterUncreatableType<DebuggerConnection>("Profiler", 1, 0, "DebuggerConnection", QLatin1String("Uncreatable type"));

    engine.load(QUrl("qrc:/main.qml"));

    return app.exec();
}

