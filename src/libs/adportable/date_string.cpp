/**************************************************************************
** Copyright (C) 2010-2013 Toshinobu Hondo, Ph.D.
** Copyright (C) 2013 MS-Cheminformatics LLC
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

#include "date_string.hpp"
#include "debug.hpp"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/local_time_adjustor.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>
#include <boost/format.hpp>

using namespace adportable;

std::string
date_string::string( const boost::gregorian::date& dt, const char * fmt )
{
    std::locale loc( std::locale::classic(), new boost::gregorian::date_facet( fmt ) );
    std::ostringstream os;
    os.imbue( loc );
    os << dt;
    return os.str();
}

// static
std::string
date_string::utc_to_localtime_string( time_t utc, unsigned usec )
{
	try {
		boost::posix_time::ptime putc = boost::posix_time::from_time_t( utc );
		boost::posix_time::ptime lt = boost::date_time::c_local_adjustor< boost::posix_time::ptime>::utc_to_local( putc );
		std::ostringstream o;
		o << boost::posix_time::to_simple_string( lt );
		o << " " << std::fixed << std::setw(7) << std::setfill('0') << std::setprecision(3) << double( usec ) / 1000.0;
		return o.str();
	} catch ( std::exception& ex ) {
		adportable::debug(__FILE__, __LINE__) << "exception: " << ex.what();
		assert(0);
	}
	return "date_string::utc_to_localtime_string - conversion error";
}
