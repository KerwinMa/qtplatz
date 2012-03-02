/**************************************************************************
** Copyright (C) 2010-2012 Toshinobu Hondo, Ph.D.
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

#include <QtCore/QCoreApplication>
#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include <adportable/combination.hpp>
#include <adcontrols/isotopecluster.hpp>
#include <adcontrols/massspectrum.hpp>
#include <adportable/array_wrapper.hpp>
#include <numeric>
#include <boost/utility.hpp>

#  if defined NDEBUG
#     pragma comment(lib, "adcontrols.lib")
#     pragma comment(lib, "adportable.lib")
#  else
#     pragma comment(lib, "../../lib/qtplatz/adcontrolsd.lib")
#  endif

typedef std::vector< size_t >::iterator iterator_t;

void
init_combination( iterator_t it, iterator_t& end, size_t nmols )  // nCr  n:=num of molecules, r:= number of isotopes
{
	*it++ = nmols;
	while ( it != end )
		*it++ = 0;
}

bool
next_combination( iterator_t it, iterator_t& end, size_t nmols )  // nCr  n:=num of molecules, r:= number of isotopes
{
    if ( it == end )
		return false;

	size_t nfree = nmols - *it;
	if ( nfree == 0 || *(end - 1) == nfree ) {
		if ( ( *it ) == 0 )
			return false;
		(*it)--;     // decriment
		init_combination( ++it, end, nfree + 1 );
		return true;
	}
	return next_combination( boost::next( it ), end, nfree );
}

int
main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
	//const size_t nmols = 5, r = 3;   // O5
//	std::vector< size_t > isotopes( r, 0 );
/*
	init_combination( isotopes.begin(), isotopes.end(), nmols );
	do {
		for ( int i = 0; i < r; ++i ) std::cout << isotopes[i] << ", ";	std::cout << std::endl;
	} while ( next_combination( isotopes.begin(), isotopes.end(), nmols ) );
*/

    const int r = 6;
    const int n = 2;

	std::vector< size_t > isotopes;
	isotopes.push_back( 0 );
	isotopes.push_back( 1 );
/*
	std::vector< std::vector< size_t >::const_iterator > v( r, isotopes.begin() );
	do {
        int n = 0; 
		for ( int i = 0; i < r; ++i ) {
			if ( *v[i] == 1 )
				++n;
			std::cout << *v[i] << ", ";
		}
		std::cout << "  --> " << n << std::endl;
	// } while ( boost::next_partial_permutation( isotopes.begin(), isotopes.begin() + 2, isotopes.end() ) );
	// } while ( boost::next_combination( isotopes.begin(), isotopes.begin() + 2, isotopes.end() ) );
   } while ( boost::next_mapping( v.begin(), v.end(), isotopes.begin(), isotopes.end() ) );
*/
	//
	adcontrols::MassSpectrum ms;
	adcontrols::IsotopeCluster::isotopeDistribution( ms, L"C6H5Cl" );
    
    return a.exec();
}
