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
#include "constants.hpp"
#include <adportable/profile.hpp>
#include <adsequence/sequence.hpp>
#include <boost/filesystem/path.hpp>

using namespace sequence::internal;

SequenceFile::~SequenceFile()
{
}

SequenceFile::SequenceFile(QObject *parent) : Core::IFile( parent )
                                    , modified_( false )
                                    , adsequence_( new adsequence::sequence )
                                    , mimeType_( sequence::Constants::C_SEQUENCE_MIMETYPE )
{
    boost::filesystem::path dir( adportable::profile::user_data_dir<char>() );
    dir /= "data";
    defaultPath_ = dir.string().c_str();
    // filename_ = ( dir /= "default.sequ" ).string().c_str();
}

SequenceFile::SequenceFile( const QString& path )
{
    filename_ = path;
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
SequenceFile::save( const QString& filename )
{
    if ( filename.isEmpty() ) {
        // save filename_;
    } else {
        // save as filename
    }
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
