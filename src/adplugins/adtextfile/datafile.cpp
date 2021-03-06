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

// #if __APPLE__ && (__GNUC_LIBSTD__ <= 4) && (__GNUC_LIBSTD_MINOR__ <= 2)
// #  define BOOST_NO_CXX11_RVALUE_REFERENCES
// #endif

#if defined __GNUC__
# pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

#include "datafile.hpp"
#include "txtspectrum.hpp"
#include <adcontrols/datafile.hpp>
#include <adcontrols/datapublisher.hpp>
#include <adcontrols/datasubscriber.hpp>
#include <adcontrols/massspectrum.hpp>
#include <adcontrols/processeddataset.hpp>
#include <portfolio/portfolio.hpp>
#include <portfolio/folder.hpp>
#include <portfolio/folium.hpp>
#include <adportable/debug.hpp>
#include <boost/any.hpp>
#include <boost/format.hpp>

using namespace adtextfile;

datafile::~datafile()
{
}

datafile::datafile()
{
}

void
datafile::accept( adcontrols::dataSubscriber& sub )
{
    // subscribe acquired dataset <LCMSDataset>
    sub.subscribe( *this );

    // subscribe processed dataset
    if ( processedDataset_ )
        sub.subscribe( *processedDataset_ );
}

bool
datafile::open( const std::wstring& filename, bool /* readonly */ )
{
    TXTSpectrum txt;

    if ( ! txt.load( filename ) ) {
        adportable::debug(__FILE__, __LINE__) 
            << "datafile '" << filename << "' open failed -- check file access permission";
        return false;
    }

    portfolio::Portfolio portfolio;

    portfolio.create_with_fullpath( filename );
    portfolio::Folder spectra = portfolio.addFolder( L"Spectra" );

    int idx = 0;
    for ( auto it: txt.spectra_ ) {
        std::wstring name( ( boost::wformat( L"Spectrum(%1%)" ) % idx++ ).str() );
        portfolio::Folium folium = spectra.addFolium( name );
        folium.setAttribute( L"dataType", adcontrols::MassSpectrum::dataClass() );
		data_[ folium.id() ] = it;
    }

    if ( txt.spectra_.size() > 1 ) {
        do {
            auto it = txt.spectra_.begin();
            adcontrols::MassSpectrumPtr ptr( new adcontrols::MassSpectrum( *it->get() ) );

            std::for_each( it + 1, txt.spectra_.end(), [&ptr]( adcontrols::MassSpectrumPtr& sub ){
                    ptr->addSegment( *sub );
                });
            
            portfolio::Folium folium = spectra.addFolium( L"A Spectrum" );
            folium.setAttribute( L"dataType", adcontrols::MassSpectrum::dataClass() );        
            data_[ folium.id() ] = ptr;
            
        } while(0);
    }
    
    processedDataset_.reset( new adcontrols::ProcessedDataset );
    processedDataset_->xml( portfolio.xml() );

    return true;
}

boost::any
datafile::fetch( const std::wstring& path, const std::wstring& dataType ) const
{
    (void)dataType;

	auto it = data_.find( path );
	if ( it != data_.end() )
		return it->second;

	return 0;
}

size_t
datafile::getSpectrumCount( int /* fcn */ ) const
{
    return 1;
}

bool
datafile::getSpectrum( int /* fcn */, int /* idx */, adcontrols::MassSpectrum&, uint32_t ) const
{
    return true;
}

bool
datafile::getTIC( int /* fcn */, adcontrols::Chromatogram& ) const
{
    return false;
}

size_t
datafile::getChromatogramCount() const
{
    return 0;
}

size_t
datafile::getFunctionCount() const
{
    return 1;
}

size_t
datafile::posFromTime( double ) const
{
	return 0;
}

double
datafile::timeFromPos( size_t ) const
{
	return 0;
}