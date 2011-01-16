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

#pragma once

#include <boost/any.hpp>
#include <map>
#include <xmlwrapper/msxml.h>
#include "node.h"

namespace portfolio {

    class Folium;
    class Folder;

    namespace internal {

        // Portfolio is a root folder

        class PortfolioImpl : public Node {
        public:
            PortfolioImpl();
            PortfolioImpl( const PortfolioImpl& );
            PortfolioImpl( const std::wstring& xml );
            operator bool () const { return isXMLLoaded_; }
            const std::wstring fullpath() const;
            std::vector<Folder> selectFolders( const std::wstring& );
            Folium selectFolium( const std::wstring& query );

            boost::any& find( const std::wstring& id );
            void assign( const std::wstring& id, boost::any& );

            ///////////////  creation ///////////////
            bool create_with_fullpath( const std::wstring& );
            Folder addFolder( const std::wstring&, bool uniq );
            xmlDocument& getDocument() { return doc_; }

            static std::wstring newGuid();
     
        private:
            bool isXMLLoaded_;
            std::map< std::wstring, boost::any > db_;
            xmlDocument doc_;
        };
    }
}


