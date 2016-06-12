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

#ifndef DEBUGGERCONNECTION_H
#define DEBUGGERCONNECTION_H

#include <QObject>

class QTcpSocket;
class QTimer;

class DebuggerConnection : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString host READ host WRITE setHost NOTIFY hostChanged)
    Q_PROPERTY(bool connected READ isConnected NOTIFY connectedChanged)
public:
    explicit DebuggerConnection(QObject *parent = nullptr);

    Q_INVOKABLE void executeCommand(const QString &command);

    inline bool isConnected() const { return m_connected; }

    inline QString host() const { return m_host; }
    void setHost(const QString &host);

Q_SIGNALS:
    void hostChanged();
    void connectedChanged();

private Q_SLOTS:
    void connectToHost();
    void onReplyReceived();

private:
    QTcpSocket *m_socket;
    QString m_host;
    bool m_connected;

    struct ReadBuffer {
        ReadBuffer();

        QByteArray buffer;
        int startIdx;
        int endIdx;
    };
    ReadBuffer m_readBuffer;
    QTimer *m_disconnectedTimer;
};

#endif // DEBUGGERCONNECTION_H
