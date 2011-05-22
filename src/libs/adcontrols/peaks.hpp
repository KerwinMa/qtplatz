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

#include <boost/smart_ptr.hpp>
#include <vector>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>

namespace adcontrols {

    class Peak;
    class Baseline;

    class ADCONTROLSSHARED_EXPORT Peaks {
    public:
        ~Peaks();
        Peaks();
        Peaks( const Peaks& );

        typedef Peak value_type;
        typedef std::vector< Peak > vector_type;

        void add( const Peak& );

        inline operator const vector_type& () const      { return peaks_;	      }
        inline operator vector_type& ()                  { return peaks_;	      }
        inline vector_type::const_iterator begin() const { return peaks_.begin(); }
        inline vector_type::iterator begin()             { return peaks_.begin(); }
        inline vector_type::const_iterator end() const   { return peaks_.end();   }
        inline vector_type::iterator end()               { return peaks_.end();   }
        inline vector_type::reverse_iterator rbegin()    { return peaks_.rbegin();}
        inline vector_type::reverse_iterator rend()      { return peaks_.rend();  }
        inline size_t size() const                       { return peaks_.size();  }
        inline vector_type::iterator erase( vector_type::iterator pos )   { return peaks_.erase( pos ); }
        inline vector_type::iterator erase( vector_type::iterator beg, vector_type::iterator end )   { return peaks_.erase( beg, end ); }

        // algorithms
        vector_type::iterator find_first_peak( const Baseline& );
        vector_type::const_iterator find_first_peak( const Baseline& ) const;

    private:
#pragma warning(disable : 4251)
        vector_type peaks_;
//#pragma warning( default : 4251)

        double areaTotal_;
        double heightTotal_;
        double noiseLevel_;

        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive& ar, const unsigned int version) {
            if ( version >= 0 ) {
                ar & BOOST_SERIALIZATION_NVP( peaks_ );
                ar & BOOST_SERIALIZATION_NVP( areaTotal_ );
                ar & BOOST_SERIALIZATION_NVP( heightTotal_ );
                ar & BOOST_SERIALIZATION_NVP( noiseLevel_ );
            }
        }

    };

}
