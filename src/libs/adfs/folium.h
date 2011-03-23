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

#include <string>
#include <vector>
#include <boost/any.hpp>
#include "node.h"

namespace adfs {

    class folder;

    class folium : public internal::Node {
    public:
        ~folium();
        folium();
        folium( const folium& );
        // Folium( xmlNode&, internal::PortfolioImpl * impl );
    public:

        std::wstring path() const;
        bool empty() const;
        void operator = ( boost::any& );
        operator boost::any& ();

        std::vector< folium > attachments();
        folder getParentFolder();

        typedef std::vector< folium > vector_type;

        template<class T> static vector_type::iterator find_first_of( vector_type::iterator it, vector_type::iterator ite ) {
            while ( it != ite ) {
                boost::any& data = (*it);
                if ( data.type() == typeid(T) )
                    return it;
                ++it;
            }
            return ite;
        }

        template<class T> static bool get( T& t, folium& folium ) {
            boost::any& data = folium;
            if ( data.type() == typeid(T) ) {
                t = boost::any_cast<T>(data);
                return true;
            }
            return false;
        }

        // --- create/modify
        folium addAttachment( const std::wstring& name );
    };

    typedef std::vector< folium > folio;

}
