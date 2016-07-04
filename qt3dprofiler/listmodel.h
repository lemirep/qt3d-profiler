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

#ifndef LISTMODEL_H
#define LISTMODEL_H

#include <QMetaEnum>
#include <QAbstractListModel>

template <class T>
class ListModel : public QAbstractListModel
{
public:
    ListModel()
        : QAbstractListModel()
    {}

    ~ListModel()
    {}

    enum Roles {};
    Q_ENUM(Roles)

    QHash<int, QByteArray> roleNames() const Q_DECL_FINAL
    {
        // Builds the roles names from the Roles enum
        QHash<int, QByteArray> roles;
        const int enumIdx = metaObject()->indexOfEnumerator("Roles");
        const QMetaEnum metaEnum = metaObject()->enumerator(enumIdx);
        const int keysCount = metaEnum.keyCount();

        for (int i = 0; i < keysCount; ++i)
            roles.insert(metaEnum.value(i), metaEnum.key(i));

        return roles;
    }

    QVariant data(const QModelIndex &index, int role) const Q_DECL_FINAL
    {
        if (index.row() < int(m_items.size())) {
            return m_items.at(index.row()).data(role);
        }
        return QVariant();
    }

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_FINAL
    {
        Q_UNUSED(parent);
        return int(m_items.size());
    }

    bool insertRows(const std::vector<T> &data)
    {
        if (data.size() == 0)
            return false;
        beginInsertRows(QModelIndex(), m_items.size(), m_items.size() + data.size() - 1);
        m_items.insert(m_items.end(), data.begin(), data.end());
        endInsertRows();
        return true;
    }

    bool insertRows(std::vector<T> &&data)
    {
        if (data.size() == 0)
            return false;
        beginInsertRows(QModelIndex(), m_items.size(), m_items.size() + data.size() - 1);
        std::move(data.begin(), data.end(), std::back_inserter(m_items));
        endInsertRows();
        return true;
    }

    bool insertRow(const T &data)
    {
        beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
        m_items.push_back(data);
        endInsertRows();
        return true;
    }

    bool insertRow(T &&data)
    {
        beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
        m_items.push_back(std::move(data));
        endInsertRows();
        return true;
    }

    Q_INVOKABLE bool removeRows(int row, int count = 0)
    {
        beginRemoveRows(QModelIndex(), row, row + count);
        m_items.erase(m_items.begin() + row, m_items.begin() + row + count + 1);
        endRemoveRows();
        return true;
    }

    bool clear()
    {
        return removeRows(0, rowCount() - 1);
    }

    std::vector<T> &items()
    {
        return m_items;
    }

private:
    std::vector<T> m_items;
};

#endif // LISTMODEL_H
