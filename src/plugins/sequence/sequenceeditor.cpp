/**************************************************************************
** Copyright (C) 2010-2013 Toshinobu Hondo, Ph.D.
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

#include "sequenceeditor.hpp"
#include "sequencefile.hpp"
#include "constants.hpp"
#include "sequencewidget.hpp"
#include <adsequence/sequence.hpp>
#include <adsequence/schema.hpp>
#include <coreplugin/uniqueidmanager.h>
#include <coreplugin/filemanager.h>
#include <coreplugin/icore.h>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem.hpp>
#include <qtwrapper/qstring.hpp>

using namespace sequence;

SequenceEditor::~SequenceEditor()
{
    delete file_;
    delete widget_;
}

SequenceEditor::SequenceEditor(QObject *parent) : Core::IEditor(parent)
                                                , displayName_( "Sequence Editor" )
                                                , file_( new SequenceFile( *this ) )
                                                , widget_( new SequenceWidget( file_->adsequence().schema()
                                                                               , *this, 0 ) )
{
    Core::UniqueIDManager* uidm = Core::UniqueIDManager::instance();
    if ( uidm )
        context_ << uidm->uniqueIdentifier( Constants::C_SEQUENCE );
    widget_->OnInitialUpdate( file_->adsequence().schema() );
}

// Core::IEditor
bool
SequenceEditor::createNew(const QString &contents )
{
    Q_UNUSED( contents );
    return true;
}

const char *
SequenceEditor::uniqueModeName() const 
{
    return sequence::Constants::C_SEQUENCE_MODE;
}

bool
SequenceEditor::open( const QString &fileName )
{
    boost::filesystem::path path( qtwrapper::wstring::copy( fileName ) );
    
    if ( boost::filesystem::exists( path ) && file_->load( fileName ) ) {

        widget_->setSequenceName( fileName );

        Core::FileManager * filemgr = Core::ICore::instance()->fileManager();
        if ( filemgr && filemgr->addFile( file_ ) )
            filemgr->addToRecentFiles( fileName );

        return true;
    }

    return false;
}

Core::IFile *
SequenceEditor::file()
{
    return static_cast<Core::IFile *>( file_ );
}

const char *
SequenceEditor::kind() const
{
    return Constants::C_SEQUENCE;
}

QString
SequenceEditor::displayName() const
{
    // displayName shows on "Open Documents" pane in Navigator
    if ( file_ )
        return file_->fileName();
    return "SequenceEditor";
}

void
SequenceEditor::setDisplayName(const QString &title)
{
    displayName_ = title;
}

bool
SequenceEditor::duplicateSupported() const
{
    return false;
}

Core::IEditor *
SequenceEditor::duplicate(QWidget *parent)
{
    Q_UNUSED( parent );
    return 0;
}

QByteArray
SequenceEditor::saveState() const
{
    return QByteArray();
}

bool
SequenceEditor::restoreState(const QByteArray &state)
{
  Q_UNUSED( state );
  return false;
}

bool
SequenceEditor::isTemporary() const
{
  return false;
}

QWidget *
SequenceEditor::toolBar()
{
  return 0;
}
// end Core::IEditor

// implement IContext
QList<int>
SequenceEditor::context() const
{ 
    return context_;
}

QWidget *
SequenceEditor::widget()
{ 
    return widget_;
}

void
SequenceEditor::slotTitleChanged( const QString& title )
{
    setDisplayName( title );
}

////

void
SequenceEditor::getSequence( adsequence::sequence& sequence ) const
{
    widget_->getSequence( sequence );
}

void
SequenceEditor::setModified( bool modified )
{
    file_->setModified( modified );
}
