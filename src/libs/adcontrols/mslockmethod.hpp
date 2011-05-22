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
//////////////////////////////////////////
// Copyright (C) 2010 Toshinobu Hondo, Ph.D.
// Science Liaison / Advanced Instrumentation Project
//////////////////////////////////////////

#ifndef MSLOCKMETHOD_H
#define MSLOCKMETHOD_H

#include "adcontrols_global.h"
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/version.hpp>
#include <string>

namespace adcontrols {

    class ADCONTROLSSHARED_EXPORT MSLockMethod {
    public:
		~MSLockMethod(void);
		MSLockMethod(void);
        MSLockMethod(const MSLockMethod &);
        MSLockMethod & operator = ( const MSLockMethod & rhs );

		bool operator == ( const MSLockMethod & rhs ) const;
		bool operator != ( const MSLockMethod & rhs ) const;

        enum eToleranceMethod {
           eToleranceMethodDa
           , eToleranceMethodPpm
        };

    private:
        eToleranceMethod toleranceMethod_;
        double massToleranceDa_;
        double massTolerancePpm_;

        double minimumPeakHeight_;

        std::wstring refMassDefnsFullyQualifiedName_;
        std::wstring refMassDefnsXML_;

        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive& ar, const unsigned int version) {
            using namespace boost::serialization;
            if ( version >= 0 ) {
                ar & BOOST_SERIALIZATION_NVP(toleranceMethod_);
                ar & BOOST_SERIALIZATION_NVP(massToleranceDa_);
                ar & BOOST_SERIALIZATION_NVP(massTolerancePpm_);
                ar & BOOST_SERIALIZATION_NVP(minimumPeakHeight_);
                ar & BOOST_SERIALIZATION_NVP(refMassDefnsFullyQualifiedName_);
                ar & BOOST_SERIALIZATION_NVP(refMassDefnsXML_);
            }
        }

    };

}

#endif // MSLOCKMETHOD_H