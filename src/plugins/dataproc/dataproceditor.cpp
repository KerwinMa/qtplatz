// -*- C++ -*-
/**************************************************************************
** Copyright (C) 2010-2011 Toshinobu Hondo, Ph.D.
** Science Liaison / Advanced Instrumentation Project
*
** Contact: toshi.hondo@scienceliaison.com
**
** Commercial Usage
**
** Licensees holding valid ScienceLiaison commercial licenses may use this
** file in accordance with the ScienceLiaison Commercial License Agreement
** provided with the Software or, alternatively, in accordance with the terms
** contained in a written agreement between you and ScienceLiaison.
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

#include "dataproceditor.h"
#include "dataprocessorfactory.h"
#include "constants.h"
#include "msprocessingwnd.h"
#include "datafileimpl.h"
#include <coreplugin/uniqueidmanager.h>

using namespace dataproc;

DataprocEditor::DataprocEditor( QWidget * widget
                               , Core::IEditorFactory * factory ) : Core::IEditor( widget )
                                                                  , factory_(factory)
                                                                  , file_(0)
                                                                  , editorWidget_( widget )
{
    Core::UniqueIDManager * uidm = Core::UniqueIDManager::instance();
    context_ << uidm->uniqueIdentifier( Constants::C_DATAPROCESSOR );

    // connect( editorWidget_, SIGNAL( contentModified() ), file_, SLOT( modified() ) );
    connect( editorWidget_, SIGNAL( titleChanged(QString) ), this, SLOT( slotTitleChanged(QString) ) );
    connect( editorWidget_, SIGNAL( contentModified() ), this, SIGNAL( changed() ) );
}

DataprocEditor::~DataprocEditor()
{
}

// implement Core::IEditor
bool
DataprocEditor::createNew( const QString &contents )
{
    Q_UNUSED( contents );
    //editorWidget_->setContent( QByteArray() );
    //file_->setFilename( QString() );
    return true;
}

bool
DataprocEditor::open( const QString &fileName )
{
    if ( factory_ && ( file_ = factory_->open( fileName ) ) )
        return true;
    return false;
    // return file_->open( fileName );
}

Core::IFile *
DataprocEditor::file()
{
    return file_;
}

const char *
DataprocEditor::kind() const
{
    return Constants::C_DATAPROCESSOR;
}

QString
DataprocEditor::displayName() const
{
    return "DataprocEditor";
}

void
DataprocEditor::setDisplayName(const QString &title)
{
}

bool
DataprocEditor::duplicateSupported() const
{
    return false;
}

Core::IEditor *
DataprocEditor::duplicate(QWidget *parent)
{
    return 0;
}

QByteArray
DataprocEditor::saveState() const
{
    return QByteArray();
}

bool
DataprocEditor::restoreState(const QByteArray &state)
{
    return true;
}

//virtual int currentLine() const { return 0; }
//virtual int currentColumn() const { return 0; }

bool
DataprocEditor::isTemporary() const
{
    return false;
}

QWidget *
DataprocEditor::toolBar()
{
    return 0;
}

// Core::IContext
QWidget *
DataprocEditor::widget()
{
    return editorWidget_;
}

QList<int>
DataprocEditor::context() const
{
    return context_;
}

