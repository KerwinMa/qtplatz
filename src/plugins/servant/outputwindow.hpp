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
//////////////////////////////////////////
// Copyright (C) 2010 Toshinobu Hondo, Ph.D.
// Science Liaison / Advanced Instrumentation Project
//////////////////////////////////////////

#pragma once

#include <coreplugin/ioutputpane.h>

class QStackedWidget;
class QPlainTextEdit;

namespace servant {

    class OutputWindow : public Core::IOutputPane {

        Q_OBJECT

    public:
        OutputWindow(void);
        ~OutputWindow(void);

        void appendLog( const std::wstring& );
        void appendLog( const QString& );

    // IOutputPane
        QWidget * outputWidget( QWidget * );
        QList<QWidget *> toolBarWidgets() const;
        QString name() const { return tr("Servant Log"); }
        int priorityInStatusBar() const;
        void visibilityChanged(bool visible);
        //bool isEmpty() const;
        //int numberOfResults() const;
        bool hasFocus();
        bool canFocus();
        void setFocus();

        bool canNext();
        bool canPrevious();
        void goToNext();
        void goToPrev();
        bool canNavigate();

        public slots:
            void clearContents();

    private:
        QStackedWidget * widget_;
        QPlainTextEdit * textWidget_;
    };

}