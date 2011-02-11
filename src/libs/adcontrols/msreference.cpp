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

#include "msreference.h"
#include "chemicalformula.h"
#include <cmath>

using namespace adcontrols;

MSReference::MSReference() : enable_( true )
                           , polarityPositive_( true )
                           , exactMass_(0)  
{
}

MSReference::MSReference( const MSReference& t ) : formula_( t.formula_ )
                                                 , enable_( t.enable_ )
                                                 , polarityPositive_( t.polarityPositive_ )
                                                 , adduct_or_loss_( t.adduct_or_loss_ )
                                                 , exactMass_( t.exactMass_ ) 
                                                 , description_( t.description_ )    
{
    if ( exactMass_ <= DBL_EPSILON ) {
        ChemicalFormula formula;
        exactMass_ = formula.getMonoIsotopicMass( formula_ );
        if ( ! adduct_or_loss_.empty() ) {
            double adduct = formula.getMonoIsotopicMass( adduct_or_loss_ );
            if ( polarityPositive_ )
                exactMass_ += adduct;
            else
                exactMass_ -= adduct;
        }
    }
}

MSReference::MSReference( const std::wstring& formula
                         , bool polarityPositive
                         , const std::wstring& adduct_or_loss
                         , bool enable
                         , double exactMass
                         , const std::wstring& description ) : formula_( formula )
                                                             , enable_( enable )
                                                             , polarityPositive_( polarityPositive )
                                                             , adduct_or_loss_( adduct_or_loss )
                                                             , exactMass_( exactMass ) 
                                                             , description_( description )    
{
}

bool
MSReference::enable() const
{
    return enable_;
}

bool
MSReference::operator < ( const MSReference& t ) const
{
    return exactMass_ < t.exactMass_;
}

double
MSReference::exactMass() const
{
    return exactMass_;
}

bool
MSReference::polarityPositive() const
{
    return polarityPositive_;
}

const std::wstring&
MSReference::formula() const
{
    return formula_;
}

const std::wstring&
MSReference::adduct_or_loss() const
{
    return adduct_or_loss_;
}

const std::wstring&
MSReference::description() const
{
    return description_;
}


void
MSReference::enable( bool value )
{
    enable_ = value;
}

void
MSReference::exactMass( double value )
{
    exactMass_ = value;
}

void
MSReference::polarityPositive( bool value )
{
    polarityPositive_ = value;
}

void
MSReference::formula( const std::wstring& value )
{
    formula_ = value;
}

void
MSReference::adduct_or_loss( const std::wstring& value )
{
    adduct_or_loss_ = value;
}

void
MSReference::description( const std::wstring& value )
{
    description_ = value;
}
