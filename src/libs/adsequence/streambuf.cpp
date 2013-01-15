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

#include "streambuf.hpp"

using namespace adsequence;

std::streamsize
streambuf::xsputn( const char * s, std::streamsize num )
{
    for ( int i = 0; i < num; ++i )
        vec_.push_back( *s++ );
    return num;
}

std::basic_streambuf<char>::int_type
streambuf::overflow ( int_type c )
{
    vec_.push_back( c );
    return c;
}

std::basic_streambuf<char>::int_type
streambuf::underflow()
{
    return ( gptr() == egptr() ) ? traits_type::eof() : traits_type::to_int_type( *gptr() );
}