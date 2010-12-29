// This is a -*- C++ -*- header.
/**************************************************************************
** Copyright (C) 2010-2011 Toshinobu Hondo, Ph.D.
** Science Liaison / Advanced Instrumentation Project
*
** Contact: toshi.hondo@scienceliaison.com
**
** Commercial Usage
**
** Licensees holding valid ScienceLiaison commercial licenses may use this file in
** accordance with the ScienceLiaison Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and ScienceLiaison.
**
** GNU Lesser General Public License Usage
**
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.TXT included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
**************************************************************************/

#pragma once

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <boost/smart_ptr.hpp>

namespace qtwidgets {

    class TreeItem;
    typedef boost::shared_ptr<TreeItem> TreeItemPtr;
    
    class SequenceModel : public QAbstractItemModel {
        Q_OBJECT
        
    public:
        SequenceModel( QObject *parent = 0 );
        ~SequenceModel();
        
        QVariant data(const QModelIndex &index, int role) const;
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
        
        QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
        QModelIndex parent(const QModelIndex &index) const;
        
        int rowCount(const QModelIndex &parent = QModelIndex()) const;
        int columnCount(const QModelIndex &parent = QModelIndex()) const;
        
        Qt::ItemFlags flags(const QModelIndex &index) const;
        bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
        bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole);
        
        bool insertColumns(int position, int columns, const QModelIndex &parent = QModelIndex());
        bool removeColumns(int position, int columns, const QModelIndex &parent = QModelIndex());
        bool insertRows(int position, int rows, const QModelIndex &parent = QModelIndex());
        bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex());
        
        ///////////////////////////////////////////////////////////////////////////
        int findParent( const QString& addrString ) const;
        
        ///////////////////////////////////////////////////////////////////////////
    private:
        TreeItem * getItem(const QModelIndex &index) const;
        TreeItemPtr rootItem_;
    };

}
