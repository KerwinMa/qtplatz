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

#include "fticr.hpp"
#include "datafile_factory.hpp"

#if defined WIN32
#  if defined _DEBUG
#     pragma comment(lib, "adcontrolsd.lib")
#     pragma comment(lib, "portfoliod.lib")
#  else
#     pragma comment(lib, "adcontrols.lib")
#     pragma comment(lib, "portfolio.lib")
#  endif
#endif

FTICR::FTICR()
{
}

namespace adcontrols {
    class datafile_factory;
}

extern "C" {
    FTICRSHARED_EXPORT adcontrols::datafile_factory * datafile_factory();
}

adcontrols::datafile_factory *
datafile_factory()
{
	return new fticr::datafile_factory();
}