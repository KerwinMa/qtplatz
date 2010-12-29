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

#include "portfolio.h"
#include "portfolioimpl.h"
#include "folder.h"
#include "folium.h"

# if defined _DEBUG
#    pragma comment(lib, "xmlwrapperd.lib")
#    pragma comment(lib, "adportabled.lib")
# else
#    pragma comment(lib, "xmlwrapper.lib")
#    pragma comment(lib, "adportable.lib")
# endif

using namespace portfolio;

Portfolio::~Portfolio()
{
}

Portfolio::Portfolio() : impl_( new internal::PortfolioImpl() )
{
}

Portfolio::Portfolio( const Portfolio& t ) : impl_(t.impl_)
{
}

Portfolio::Portfolio( const std::wstring& xml ) : impl_( new internal::PortfolioImpl( xml ) )
{
}

std::vector< Folder >
Portfolio::folders()
{
    return impl_->selectFolders( L"./folder[@folderType='directory']" );
}