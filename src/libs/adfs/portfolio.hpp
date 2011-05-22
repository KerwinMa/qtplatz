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

#pragma once

#include <boost/smart_ptr.hpp>
#include <vector>
#include <adfs/attributes.hpp>

namespace adfs {

    class folium;
    class folder;
    class sqlite;

    class portfolio {
    public:
        ~portfolio();
        portfolio();
        portfolio( const portfolio& );

        std::vector<folder> folders();
        folium findFolium( const std::wstring& id );
        folder findFolder( const std::wstring& name ) const;

        bool create( const wchar_t * filename, size_t alloc = 0, size_t page_size = 8192 );
        bool mount( const wchar_t * filename );
        bool close();

        folder addFolder( const std::wstring& name, bool uniq = true );

        inline sqlite& db() const { return *db_; }

    private:
        boost::shared_ptr< adfs::sqlite > db_;
  };

}