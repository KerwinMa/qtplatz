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

#include "posix_path.h"
#include <algorithm>

namespace adportable { namespace detail {

    template<class char_type> struct forslash {
        char_type operator () ( char_type c ) {
            return c == L'\\' ? L'/' : c;
        }
    };
}
}

using namespace adportable;

path::path()
{
}

path::path( const std::wstring& path ) : boost::filesystem::path( path )
{
}

path::path( const std::string& path ) : boost::filesystem::path( path )
{
}

path::path( const path& t ) : boost::filesystem::path( t )
{
}

std::wstring
path::posix( const boost::filesystem::path& path )
{
    std::wstring temp = path.wstring();
    std::transform( temp.begin(), temp.end(), temp.begin(), detail::forslash<wchar_t>() );
    return temp;
}
