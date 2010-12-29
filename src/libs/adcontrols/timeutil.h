// This is a -*- C++ -*- header.
//////////////////////////////////////////
// Copyright (C) 2010 Toshinobu Hondo, Ph.D.
// Science Liaison / Advanced Instrumentation Project
//////////////////////////////////////////

#pragma once

#include "adcontrols_global.h"
#include <utility>
#include <boost/serialization/nvp.hpp>

namespace adcontrols {

    struct ADCONTROLSSHARED_EXPORT seconds_t { 
        seconds_t( double t = 0 ) : seconds(t) {} 
        double seconds; 
        operator double () const { return seconds; }
    private:
        friend class boost::serialization::access;
        template<class Archive> void serialize(Archive& ar, const unsigned int /* version */) {
            ar & BOOST_SERIALIZATION_NVP( seconds );
        }
    };

    struct ADCONTROLSSHARED_EXPORT minutes_t {
        minutes_t( double t = 0 ) : minutes(t) {}
        double minutes; operator double () const { return minutes; }
    };

    struct ADCONTROLSSHARED_EXPORT timeutil {
        static minutes_t toMinutes( const seconds_t& );
        static seconds_t toSeconds( const minutes_t& );
        std::pair<double, double> toMinutes( const std::pair<seconds_t, seconds_t>& pair );
    };

}

