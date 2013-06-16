/**************************************************************************
** Copyright (C) 2013 MS-Cheminformatics LLC
** Copyright (C) 2010-2013 Toshinobu Hondo, Ph.D.
*
** Contact: info@ms-cheminfo.com
**
** Commercial Usage
**
** Licensees holding valid MS-Cheminformatics commercial licenses may use this file in
** accordance with the MS-Cheminformatics Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and MS-Cheminformatics.
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

#include "attributes.hpp"
#include "attribute.hpp"

using namespace adchem;

attributes::~attributes()
{
}

attributes::attributes()
{
}

attributes::attributes( const attributes& t ) : vec_( t.vec_ )
{
}

void
attributes::operator << ( const attribute& t )
{
    vec_.push_back( t );
}

size_t 
attributes::size() const
{
    return vec_.size();
}

const attribute&
attributes::operator [] ( size_t idx ) const
{
    return vec_[ idx ];
}