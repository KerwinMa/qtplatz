/**************************************************************************
** Copyright (C) 2010-2013 Toshinobu Hondo, Ph.D.
** Copyright (C) 2013 MS-Cheminformatics LLC, Toin, Mie Japan
*
** Contact: toshi.hondo@qtplatz.com
**
** Commercial Usage
**
** Licensees holding valid ScienceLiaison commercial licenses may use this file in
** accordance with the MS-Cheminformatics Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and MS-Cheminformatics LLC.
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

#ifndef MSPEAKINFO_HPP
#define MSPEAKINFO_HPP

#pragma once

#include <vector>
#include "adcontrols_global.h"
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/vector.hpp>

#include <compiler/disable_dll_interface.h>

namespace adcontrols {

    class MSPeakInfoItem;

    class ADCONTROLSSHARED_EXPORT MSPeakInfo {
    public:
        MSPeakInfo();
        MSPeakInfo( const MSPeakInfo& );

        static const wchar_t * dataClass() { return L"adcontrols::MSPeakInfo"; }

        typedef std::vector< MSPeakInfoItem >::iterator iterator;
        typedef std::vector< MSPeakInfoItem >::const_iterator const_iterator;
        
        inline iterator begin() { return vec_.begin(); }
        inline iterator end() { return vec_.end(); }
        inline const_iterator begin() const { return vec_.begin(); }
        inline const_iterator end() const { return vec_.end(); }
        size_t size() const;
        void clear();

        MSPeakInfo& operator << ( const MSPeakInfoItem& );

        void addSegment( const MSPeakInfo& );
        MSPeakInfo& getSegment( size_t fcn );
        const MSPeakInfo& getSegment( size_t fcn ) const;
        size_t numSegments() const;

        static bool archive( std::ostream&, const MSPeakInfo& );
        static bool restore( std::istream&, MSPeakInfo& );

    private:
        std::vector< MSPeakInfoItem > vec_;
        std::vector< MSPeakInfo > siblings_;

        friend class boost::serialization::access;
        template<class Archive> void serialize(Archive& ar, const unsigned int ) {
            ar  & vec_
                & siblings_
                ;
        }

    };

}

#endif // MSPEAKINFO_HPP
