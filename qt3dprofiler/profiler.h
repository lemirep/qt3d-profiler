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

#ifndef PROFILER_H
#define PROFILER_H

#include <QObject>
#include <QAbstractListModel>

class AspectInfoModel;
class JobTracesModel;
class CommandDisplayModel;
class DebuggerConnection;
class RenderViewModel;

class Profiler : public QObject
{
    Q_OBJECT
    Q_PROPERTY(float msecToPixelScale READ msecToPixelScale WRITE setMsecToPixelScale NOTIFY msecToPixelScaleChanged)
    Q_PROPERTY(QAbstractListModel *aspectInfoModel READ aspectInfoModel CONSTANT)
    Q_PROPERTY(QAbstractListModel *jobTracesModel READ jobTracesModel CONSTANT)
    Q_PROPERTY(QAbstractListModel *commandDisplayModel READ commandDisplayModel CONSTANT)
    Q_PROPERTY(QAbstractListModel *renderViewModel READ renderViewModel NOTIFY renderViewModelChanged)
    Q_PROPERTY(DebuggerConnection *debuggerConnection READ debuggerConnection CONSTANT)

public:

    enum FrameType {
        Worker = 0,
        Submission
    };
    Q_ENUM(FrameType)

    explicit Profiler(QObject *parent = nullptr);
    ~Profiler();

    void setMsecToPixelScale(float scale);
    float msecToPixelScale() const;

    QAbstractListModel *aspectInfoModel() const;
    QAbstractListModel *jobTracesModel() const;
    QAbstractListModel *commandDisplayModel() const;
    QAbstractListModel *renderViewModel() const;
    DebuggerConnection *debuggerConnection() const;

    Q_INVOKABLE void addTraceFile(const QUrl &fileUrl);
    Q_INVOKABLE void removeTrace(int idx);
    Q_INVOKABLE void executeCommand(const QString &command);

Q_SIGNALS:
    void msecToPixelScaleChanged();
    void renderViewModelChanged();

private Q_SLOTS:
    void commandReplyReceived(const QJsonDocument &reply);

private:
    void parseConfigFile(const QString &filePath);

    float m_msecToPixelScale;
    QScopedPointer<AspectInfoModel> m_aspectInfoModel;
    QScopedPointer<JobTracesModel> m_jobTracesModel;
    QScopedPointer<CommandDisplayModel> m_commandDisplayModel;
    QScopedPointer<DebuggerConnection> m_debuggerConnection;
    QScopedPointer<RenderViewModel> m_renderViewModel;
};

#endif // PROFILER_H
