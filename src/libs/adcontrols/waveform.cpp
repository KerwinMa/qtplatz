/**************************************************************************
** Copyright (C) 2010-2013 Toshinobu Hondo, Ph.D.
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

#include "waveform.hpp"
#include "massspectrum.hpp"
#include <adportable/array_wrapper.hpp>
#include <adportable/fft.hpp>
#include <vector>
#include <complex>

using namespace adcontrols;

waveform::waveform()
{
}

bool
waveform::fft::reduceNoise( adcontrols::MassSpectrum& ms )
{
    size_t totalSize = ms.size();
	(void)totalSize;
	size_t N = 32;
    while ( N < ms.size() )
		N *= 2;
	const size_t NN = ms.size();

	// const double * pMass = ms.getMassArray();
	adportable::array_wrapper<const double> pIntens( ms.getIntensityArray(), N );

	std::vector< std::complex<double> > spc( N );
	std::vector< std::complex<double> > fft( N );
	size_t n;
	for ( n = 0; n < N && n < NN; ++n )
		spc[ n ] = std::complex<double>( pIntens[ n ] );
	while ( n < N )
		spc[ n++ ] = pIntens[ NN - 1 ];

	adportable::fft::fourier_transform( fft, spc, false );
	adportable::fft::apodization( N/2 - N/16, N / 16, fft );
	adportable::fft::fourier_transform( spc, fft, true );

	std::vector<double> data( N );
	for ( size_t i = 0; i < NN; ++i )
		data[ i ] = spc[i].real();

	ms.setIntensityArray( &data[0] );

	return true;
}

