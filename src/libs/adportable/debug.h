// This is a -*- C++ -*- header.
//////////////////////////////////////////
// Copyright (C) 2010 Toshinobu Hondo, Ph.D.
// Science Liaison / Advanced Instrumentation Project
//////////////////////////////////////////

#pragma once

#include <sstream>

namespace adportable {

    class debug {
        std::ostringstream o_;
    public:
        debug(void);
        ~debug(void);
        static void initialize( const std::string& filename );

        debug& operator << ( const char * );
        debug& operator << ( const std::string& );
        debug& operator << ( const std::wstring& );
        debug& operator << ( bool );
        debug& operator << ( int );
        debug& operator << ( double );
		debug& operator << ( unsigned long );
    };

}