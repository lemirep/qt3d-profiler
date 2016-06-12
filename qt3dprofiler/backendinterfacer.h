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

#ifndef BACKENDINTERFACER_H
#define BACKENDINTERFACER_H

#include <QObject>
#include <QAbstractListModel>

class AspectInfoModel;
class JobTracesModel;

class BackendInterfacer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(float msecToPixelScale READ msecToPixelScale WRITE setMsecToPixelScale NOTIFY msecToPixelScaleChanged)
    Q_PROPERTY(QAbstractListModel *aspectInfoModel READ aspectInfoModel CONSTANT)
    Q_PROPERTY(QAbstractListModel *jobTracesModel READ jobTracesModel CONSTANT)

public:
    explicit BackendInterfacer(QObject *parent = nullptr);
    ~BackendInterfacer();

    void setMsecToPixelScale(float scale);
    float msecToPixelScale() const;

    QAbstractListModel *aspectInfoModel() const;
    QAbstractListModel *jobTracesModel() const;

    Q_INVOKABLE void addTraceFile(const QUrl &fileUrl);
    Q_INVOKABLE void removeTrace(int idx);

Q_SIGNALS:
    void msecToPixelScaleChanged();

private:
    void parseConfigFile(const QString &filePath);

    float m_msecToPixelScale;
    QScopedPointer<AspectInfoModel> m_aspectInfoModel;
    QScopedPointer<JobTracesModel> m_jobTracesModel;
};

#endif // BACKENDINTERFACER_H
