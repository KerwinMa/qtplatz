// This is a -*- C++ -*- header.
//////////////////////////////////////////
// Copyright (C) 2010 Toshinobu Hondo, Ph.D.
// Science Liaison / Advanced Instrumentation Project
//////////////////////////////////////////

#pragma once

#include <ace/CDR_Stream.h>
#include <string>
#include <boost/noncopyable.hpp>

namespace acewrapper {

	class InputCDR : boost::noncopyable {
	public:
		InputCDR();
		InputCDR( ACE_Message_Block * mb );

		operator ACE_InputCDR& () { return impl_; }
		operator const ACE_InputCDR& () const { return impl_; }

		InputCDR& operator >> (bool&);
		InputCDR& operator >> (char&);
		InputCDR& operator >> (unsigned char&);
		InputCDR& operator >> (short&);
		InputCDR& operator >> (unsigned short&);
		InputCDR& operator >> (long&);
		InputCDR& operator >> (unsigned long&);
		InputCDR& operator >> (long long&);
		InputCDR& operator >> (unsigned long long&);
		InputCDR& operator >> (float&);
		InputCDR& operator >> (double&);
		InputCDR& operator >> (std::string& );
		InputCDR& operator >> (std::wstring& );
		bool read( bool *, size_t );
		bool read( char *, size_t );
		bool read( unsigned char *, size_t );
		bool read( short *, size_t );
		bool read( unsigned short *, size_t );
		bool read( long *, size_t );
		bool read( unsigned long *, size_t );
		bool read( long long *, size_t );
		bool read( unsigned long long *, size_t );
		bool read( float *, size_t );
		bool read( double *, size_t );
		bool read( long double *, size_t );
	private:
		ACE_InputCDR impl_;
	};

}
