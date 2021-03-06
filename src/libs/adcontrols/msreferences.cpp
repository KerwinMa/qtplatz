// -*- C++ -*-
/**************************************************************************
** Copyright (C) 2010-2013 Toshinobu Hondo, Ph.D.
** Copyright (C) 2013 MS-Cheminformatics LLC
*
** Contact: info@ms-cheminfo.com
**
** Commercial Usage
**
** Licensees holding valid MS-Cheminformatics commercial licenses may use this
** file in accordance with the MS-Cheminformatics Commercial License Agreement
** provided with the Software or, alternatively, in accordance with the terms
** contained in a written agreement between you and MS-Cheminformatics.
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

#include "msreferences.hpp"
#include "msreference.hpp"

using namespace adcontrols;

MSReferences::MSReferences()
{
}

MSReferences::MSReferences( const MSReferences& t ) : vec_(t.vec_)
                                                    , name_(t.name_)
{
}

MSReferences::vector_type::iterator
MSReferences::begin()
{
    return vec_.begin();
}

MSReferences::vector_type::iterator
MSReferences::end()
{
    return vec_.end();
}

MSReferences::vector_type::const_iterator
MSReferences::begin() const
{
    return vec_.begin();
}

MSReferences::vector_type::const_iterator
MSReferences::end() const
{
    return vec_.end();
}

const std::wstring&
MSReferences::name() const
{
    return name_;
}

void
MSReferences::name( const std::wstring& name )
{
    name_ = name;
}

size_t
MSReferences::size() const
{
    return vec_.size();
}

const MSReference&
MSReferences::operator [] ( int idx ) const
{
    return vec_[ idx ];
}

MSReference&
MSReferences::operator [] ( int idx )
{
    return vec_[ idx ];
}

MSReferences&
MSReferences::operator << ( const MSReference& t )
{
    vec_.push_back( t );
    return *this;
}
