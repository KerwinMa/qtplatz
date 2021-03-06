// -*- C++ -*-
/**************************************************************************
** Copyright (C) 2010-2013 Toshinobu Hondo, Ph.D.
** Copyright (C) 2013 MS-Cheminformatics LLC
*
** Contact: info@ms-cheminfo.com
**
** Commercial Usage
**
** Licensees holding valid MS-Cheminformatics commercial licenses may use this
** file in accordance with the MS-Cheminformatics Commercial License Agreement
** provided with the Software or, alternatively, in accordance with the terms
** contained in a written agreement between you and MS-Cheminformatics.
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

#ifndef MSCALIBSUMMARYDELEGATE_H
#define MSCALIBSUMMARYDELEGATE_H

#include <QItemDelegate>

namespace qtwidgets2 {

    class MSCalibSummaryDelegate : public QItemDelegate {
        Q_OBJECT
    public:
        explicit MSCalibSummaryDelegate(QObject *parent = 0);

        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
		void setEditorData(QWidget *editor, const QModelIndex &index) const override;
        void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
        bool editorEvent( QEvent * event, QAbstractItemModel *
                          , const QStyleOptionViewItem&, const QModelIndex& ) override;

        static void to_print_text( std::string&, const QModelIndex& index );

    signals:
        void valueChanged( const QModelIndex& ) const;

    public slots:

    };

}

#endif // MSCALIBSUMMARYDELEGATE_H
