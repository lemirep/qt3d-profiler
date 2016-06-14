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

#include "debuggerconnection.h"
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QTimer>

namespace {

const qint32 MagicNumber = 0x454;

struct CommandHeader
{
    qint32 magic;
    qint32 size;
};

} // anonymous

DebuggerConnection::ReadBuffer::ReadBuffer()
    : buffer(4096, 0)
    , startIdx(0)
    , endIdx(0)
{
}

DebuggerConnection::DebuggerConnection(QObject *parent)
    : QObject(parent)
    , m_socket(new QTcpSocket(this))
    , m_host(QLatin1String("127.0.0.1"))
    , m_connected(false)
    , m_disconnectedTimer(new QTimer(this))
{
    QObject::connect(m_socket, &QTcpSocket::connected, [this] {
        m_connected = true;
        emit connectedChanged();
        m_disconnectedTimer->stop();
    });
    QObject::connect(m_socket, &QTcpSocket::disconnected, [this] {
        m_connected = false;
        emit connectedChanged();
        m_disconnectedTimer->start();
    });
    QObject::connect(m_socket, &QTcpSocket::readyRead,
                     this, &DebuggerConnection::onReplyReceived);

    m_disconnectedTimer->setInterval(5000);
    QObject::connect(m_disconnectedTimer, &QTimer::timeout,
                     this,  &DebuggerConnection::connectToHost);
    m_disconnectedTimer->start();
}

void DebuggerConnection::executeCommand(const QString &command)
{
    if (m_connected) {
        CommandHeader replyHeader;

        QJsonObject commandObj;
        commandObj.insert(QLatin1String("command"), QJsonValue(command));

        const QByteArray payload = QJsonDocument(commandObj).toJson();

        replyHeader.magic = MagicNumber;
        replyHeader.size = payload.size();
        // Write header
        m_socket->write(reinterpret_cast<const char *>(&replyHeader), sizeof(CommandHeader));
        // Write payload
        m_socket->write(payload.constData(), payload.size());
    }
}

void DebuggerConnection::connectToHost()
{
    if (m_connected)
        m_socket->disconnectFromHost();
    m_socket->connectToHost(m_host, 8883, QIODevice::ReadWrite);
}

void DebuggerConnection::setHost(const QString &host)
{
    if (host != m_host) {
        m_host = host;
        emit hostChanged();
        connectToHost();
    }
}

void DebuggerConnection::onReplyReceived()
{
    const QByteArray newData = m_socket->readAll();
    m_readBuffer.buffer.insert(m_readBuffer.endIdx, newData);
    m_readBuffer.endIdx += newData.size();

    const int commandPacketSize = sizeof(CommandHeader);
    while ((m_readBuffer.endIdx - m_readBuffer.startIdx) >= commandPacketSize) {
        CommandHeader *header = reinterpret_cast<CommandHeader *>(m_readBuffer.buffer.data() + m_readBuffer.startIdx);
        if (header->magic == MagicNumber &&
                (m_readBuffer.endIdx - (m_readBuffer.startIdx + commandPacketSize)) >= header->size) {
            // We have a valid command
            // We expect command to be a CommandHeader + some json text
            const QJsonDocument doc = QJsonDocument::fromJson(
                        QByteArray(m_readBuffer.buffer.data() + m_readBuffer.startIdx + commandPacketSize,
                                   header->size));

            if (!doc.isNull()) {
                // Send command to the aspectEngine
                QJsonObject commandObj = doc.object();
                const QJsonValue commandNameValue = commandObj.value(QLatin1String("command"));
                emit replyReceived(doc);
            }

            m_readBuffer.startIdx += commandPacketSize + header->size;
        }
    }
    if (m_readBuffer.startIdx != m_readBuffer.endIdx && m_readBuffer.startIdx != 0) {
        // Copy remaining length of buffer at begininning
        memcpy(m_readBuffer.buffer.data(),
               m_readBuffer.buffer.constData() + m_readBuffer.startIdx,
               m_readBuffer.endIdx - m_readBuffer.startIdx);
    }
    m_readBuffer.endIdx -= m_readBuffer.startIdx;
    m_readBuffer.startIdx = 0;
}
