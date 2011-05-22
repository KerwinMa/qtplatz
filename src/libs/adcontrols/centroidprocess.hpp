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

#include "adcontrols_global.h"
#include <boost/serialization/nvp.hpp>
#include <boost/smart_ptr.hpp>

namespace adcontrols {

    class CentroidMethod;
    class MassSpectrum;

    namespace internal {
        class CentroidProcessImpl;
    }

	class ADCONTROLSSHARED_EXPORT CentroidProcess {
	public:
		~CentroidProcess(void);
		CentroidProcess(void);
		CentroidProcess( const CentroidMethod& );
        bool operator()( const CentroidMethod&, const MassSpectrum& );
		bool operator()( const MassSpectrum& );
        bool getCentroidSpectrum( MassSpectrum& );

	private:
        internal::CentroidProcessImpl* pImpl_;
        //static void setup( SACONTROLSLib::ISAMSPeakDetect*, const CentroidMethod& );
	};

}