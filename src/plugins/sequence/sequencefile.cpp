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

#include "sequencefile.hpp"
#include "sequenceeditor.hpp"
#include "constants.hpp"
#include "serializer.hpp"
#include <adcontrols/processmethod.hpp>
#include <adinterface/controlmethodC.h>
#include <adportable/profile.hpp>
#include <adsequence/sequence.hpp>
//
//#include <adportable/portable_binary_oarchive.hpp>
//#include <adportable/portable_binary_iarchive.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/format.hpp>
//
#include <qtwrapper/qstring.hpp>
#include <boost/filesystem/path.hpp>
#include <fstream>
#include <iostream>
#include <qmessagebox.h>

using namespace sequence;

SequenceFile::~SequenceFile()
{
}

SequenceFile::SequenceFile( const SequenceEditor& editor
                            , QObject *parent ) : Core::IFile( parent )
                                                , editor_( editor )
                                                , mimeType_( sequence::Constants::C_SEQUENCE_MIMETYPE )
                                                , modified_( false )
                                                , adsequence_( new adsequence::sequence )
{
    boost::filesystem::path dir( adportable::profile::user_data_dir<char>() );
    dir /= "data";
    defaultPath_ = dir.string().c_str();
    // filename_ = ( dir /= "default.sequ" ).string().c_str();
}

void
SequenceFile::setModified( bool val )
{
    modified_ = val;
    emit changed();
}

bool
SequenceFile::isModified() const
{
    return modified_;
}

QString
SequenceFile::mimeType() const
{
	return mimeType_;
}

bool
SequenceFile::load( const QString& filename )
{
    ctrlmethods_.clear();
    procmethods_.clear();

    boost::filesystem::path path = qtwrapper::wstring::copy( filename );
    std::ifstream inf( path.string().c_str() );
	if ( ! adsequence::sequence::xml_restore( inf, *adsequence_ ) )
		QMessageBox::warning( 0, "SequenceFile", ( boost::format( "FILE %1% OPEN FAILED" ) % path.string() ).str().c_str() );

    using adsequence::sequence;
    do {
        sequence::method_vector_type& ctrlmap = adsequence_->getControlMethod();
        for ( sequence::method_vector_type::const_iterator it = ctrlmap.begin(); it != ctrlmap.end(); ++it )
            serializer::restore( ctrlmethods_[ it->first ], it->second );
    } while(0);

    do {
        sequence::method_vector_type& procmap = adsequence_->getProcessMethod();
        for ( sequence::method_vector_type::const_iterator it = procmap.begin(); it != procmap.end(); ++it )
            serializer::restore( procmethods_[ it->first ], it->second );
    } while(0);

    if ( ! filename.isEmpty() )
        filename_ = filename;

	// editor_.setSequence( *adsequence_ );
	setModified( true );
    return true;
}

bool
SequenceFile::save( const QString& filename )
{
    editor_.getSequence( *adsequence_ );

    if ( ! filename.isEmpty() ) // save as
        filename_ = filename; // replace filename
    
    for ( control_method_map_type::const_iterator it = ctrlmethods_.begin(); it != ctrlmethods_.end(); ++it ) {
        adsequence::sequence::method_vector_type& ctrlmap = adsequence().getControlMethod();
        if ( it->second )
            serializer::archive( ctrlmap[ it->first ], *it->second );
    }

    for ( process_method_map_type::const_iterator it = procmethods_.begin(); it != procmethods_.end(); ++it ) {
        adsequence::sequence::method_vector_type& procmap = adsequence().getProcessMethod();
        if ( it->second )
            serializer::archive( procmap[ it->first ], *it->second );
    }

    //-------
    boost::filesystem::path path( qtwrapper::wstring::copy( filename_ ) );
    std::ofstream outf( path.string().c_str() );
	if ( ! adsequence::sequence::xml_archive( outf, *adsequence_ ) )
		QMessageBox::warning( 0, "SequenceFile", ( boost::format( "FILE %1% SAVE FAILED" ) % path.string() ).str().c_str() );

	return true;
}

QString
SequenceFile::fileName() const
{
    return filename_;
}

QString
SequenceFile::defaultPath() const
{
    return defaultPath_;
}

QString
SequenceFile::suggestedFileName() const
{
    return QString();
}

bool
SequenceFile::isReadOnly() const
{
    return false;
}

bool
SequenceFile::isSaveAsAllowed() const
{
    return true;
}

void
SequenceFile::modified( ReloadBehavior* behavior )
{
    Q_UNUSED(behavior);
}

adsequence::sequence&
SequenceFile::adsequence()
{
    return * adsequence_;
}

const adsequence::sequence&
SequenceFile::adsequence() const
{
    return * adsequence_;
}

void
SequenceFile::removeProcessMethod( const std::wstring& name )
{
	process_method_map_type::iterator it = procmethods_.find ( name );
    if ( it != procmethods_.end() )
		procmethods_.erase( it );
}

void
SequenceFile::removeControlMethod( const std::wstring& name )
{
	control_method_map_type::iterator it = ctrlmethods_.find ( name );
	if ( it != ctrlmethods_.end() )
		ctrlmethods_.erase( it );
}

const adcontrols::ProcessMethod *
SequenceFile::getProcessMethod( const std::wstring& name ) const
{
	process_method_map_type::const_iterator it = procmethods_.find( name );
	if ( it == procmethods_.end() )
		return 0;
	return it->second.get();
}

const ControlMethod::Method *
SequenceFile::getControlMethod( const std::wstring& name ) const
{
	control_method_map_type::const_iterator it = ctrlmethods_.find( name );
	if ( it == ctrlmethods_.end() )
		return 0;
	return it->second.get();
}

void
SequenceFile::setProcessMethod( const std::wstring& name, const adcontrols::ProcessMethod& m )
{
	using adcontrols::ProcessMethod;
	procmethods_[ name ] = boost::shared_ptr< ProcessMethod >( new ProcessMethod( m ) );
}

void
SequenceFile::setControlMethod( const std::wstring& name, const ControlMethod::Method& m )
{
	using ControlMethod::Method;
	ctrlmethods_[ name ] = boost::shared_ptr< Method >( new Method( m ) );
}

