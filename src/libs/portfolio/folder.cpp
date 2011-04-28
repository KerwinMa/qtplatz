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

#include "folder.h"
#include "folium.h"

using namespace portfolio;

Folder::~Folder()
{
}

Folder::Folder()
{
}

#if defined USE_MSXML
Folder::Folder( const xmlNode& n, internal::PortfolioImpl * impl ) : Node( n, impl )
{
}
#else
Folder::Folder( const pugi::xml_node& n, internal::PortfolioImpl * impl ) : Node( n, impl )
{
}
#endif

Folder::Folder( const Folder& t ) : Node( t )
{
}

std::vector< Folder >
Folder::folders()
{
#if defined USE_MSXML
    xmlNodeList list = Node::selectNodes( L"./folder[@folderType='directory']" );
    std::vector< Folder > folders;
    for ( size_t i = 0; i < list.size(); ++i )
        folders.push_back( Folder( list[i], impl_ ) );
#else
    std::vector< Folder > folders;
    pugi::xpath_node_set list = node_.select_nodes( "./folder[@folderType='directory']" );
    for ( pugi::xpath_node_set::const_iterator it = list.begin(); it != list.end(); ++it )
        folders.push_back( Folder( it->node(), impl_ ) );
#endif
    return folders;
}

const std::vector< Folder >
Folder::folders() const
{
    return const_cast< Folder * >(this)->folders();
}


Folio
Folder::folio()
{
    Folio folio;
#if defined USE_MSXML
    xmlNodeList list = Node::selectNodes( L"./folder[@folderType='file']|./folium" );
    for ( size_t i = 0; i < list.size(); ++i )
        folio.push_back( Folium( list[i], impl_ ) );
#else
    pugi::xpath_node_set list = node_.select_nodes( "./folder[@folderType='file']|./folium" );
    for ( pugi::xpath_node_set::const_iterator it = list.begin(); it != list.end(); ++it )
        folio.push_back( Folium( it->node(), impl_ ) );
#endif
    return folio;
}

const Folio
Folder::folio() const
{
    return const_cast< Folder * >(this)->folio();
}

Folium
Folder::selectSingleFolium( const std::wstring& )
{
    return Folium();
}

/////////////////////////
Folium
Folder::addFolium( const std::wstring& name )
{
    return Folium( Node::addFolium( name ), impl_ );
}
