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

#include "portfolio_global.h"
#include <vector>
#include "node.hpp"

namespace pugi { class xml_node; }

namespace portfolio {

    class Folium;

    namespace internal {
        PortfolioImpl;
    }

    // folder can be directory, or data (folio)

    class PORTFOLIOSHARED_EXPORT Folder : public internal::Node {
    public:
        ~Folder();
        Folder();
        // Folder( const xmlNode&, internal::PortfolioImpl * );
        Folder( const pugi::xml_node&, internal::PortfolioImpl * );
        Folder( const Folder& );

        std::vector< Folder > folders();
        const std::vector< Folder > folders() const;
        std::vector< Folium > folio();
        const std::vector< Folium > folio() const;
        Folium selectSingleFolium( const std::wstring& );

        // --- add/modify features
        Folium addFolium( const std::wstring& name );
    };

}
