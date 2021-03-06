/**************************************************************************
** Copyright (C) 2010-2012 Toshinobu Hondo, Ph.D.
** Copyright (C) MS-Cheminformatics LLC / Advanced Instrumentation Project
*
** Contact: toshi.hondo@scienceliaison.com
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

#include "mzxml.hpp"

#include "datafile_factory.hpp"

#if defined WIN32
#  if defined _DEBUG
#     pragma comment(lib, "adcontrolsd.lib")
#     pragma comment(lib, "portfoliod.lib")
#     pragma comment(lib, "xmlparserd.lib") //-- can't work since 4GB file size limitation by fopen() on windows
#  else
#     pragma comment(lib, "adcontrols.lib")
#     pragma comment(lib, "portfolio.lib")
// #     pragma comment(lib, "xmlparser.lib")
#  endif
#endif


mzXML::mzXML()
{
}

namespace adcontrols {
    class datafile_factory;
}

extern "C" {
    MZXMLSHARED_EXPORT adcontrols::datafile_factory * datafile_factory();
}

adcontrols::datafile_factory *
datafile_factory()
{
	return new mzxml::datafile_factory();
}
