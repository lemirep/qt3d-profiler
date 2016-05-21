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

#ifndef JOBSTATSREADER_H
#define JOBSTATSREADER_H

#include <QObject>
#include <QScopedPointer>
#include <QAbstractListModel>

class FrameModel;
class JobTypeInfoModel;

class JobStatsReader : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QAbstractListModel *jobStatsModel READ jobStatsModel NOTIFY jobStatsModelChanged)
    Q_PROPERTY(QAbstractListModel *jobTypeInfoModel READ jobTypeInfoModel CONSTANT)
    Q_PROPERTY(float msecToPixelScale READ msecToPixelScale WRITE setMsecToPixelScale NOTIFY msecToPixelScaleChanged)
    Q_PROPERTY(int threadCount READ threadCount NOTIFY threadCountChanged)
public:
    JobStatsReader();
    ~JobStatsReader();

    Q_INVOKABLE void readTraceFile(const QUrl &fileUrl);

    QAbstractListModel *jobStatsModel() const;
    QAbstractListModel *jobTypeInfoModel() const;
    float msecToPixelScale() const;
    int threadCount() const;
    void setMsecToPixelScale(float scale);


Q_SIGNALS:
    void jobStatsModelChanged();
    void msecToPixelScaleChanged();
    void threadCountChanged();

private:
    QScopedPointer<FrameModel> m_jobStatsModel;
    QScopedPointer<JobTypeInfoModel> m_jobTypeInfoModel;
    float m_msecToPixelScale;
    int m_threadCount;

    void parseConfigFile(const QString &filePath);
    QHash<int, QString> m_jobTypeToNameTable;
    QHash<int, QColor> m_jobTypeToColorTable;
};
#endif // JOBSTATSREADER_H
