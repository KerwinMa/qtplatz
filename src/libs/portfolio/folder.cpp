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

Folder::Folder( const xmlNode& n, internal::PortfolioImpl * impl ) : Node( n, impl )
{
}

Folder::Folder( const Folder& t ) : Node( t )
{
}

std::vector< Folder >
Folder::folders()
{
    xmlNodeList list = Node::selectNodes( L"./folder[@folderType='directory']" );
    std::vector< Folder > folders;
    for ( size_t i = 0; i < list.size(); ++i )
        folders.push_back( Folder( list[i], impl_ ) );
    return folders;
}

Folio
Folder::folio()
{
    xmlNodeList list = Node::selectNodes( L"./folder[@folderType='file']" );
    Folio folio;
    for ( size_t i = 0; i < list.size(); ++i )
        folio.push_back( Folium( list[i], impl_ ) );
    return folio;
}

Folium
Folder::selectSingleFolium( const std::wstring& )
{
    return Folium();
}
