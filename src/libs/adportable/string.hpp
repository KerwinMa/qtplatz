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

#include <string>

namespace adportable {

    typedef unsigned long	u32char_t;	/* at least 32 bits */
    typedef unsigned short	u16char_t;	/* at least 16 bits */
    typedef unsigned char	u8char_t;	/* typically 8 bits */

    class u32string : public std::basic_string<u32char_t> {
    public:
        u32string();
        u32string( const std::basic_string<u32char_t>& );
        u32string( const std::wstring& );
    };

    class u16string : public std::basic_string<u16char_t> {
    public:
        u16string();
        u16string( const std::basic_string<u16char_t>& );
        u16string( const std::wstring& );
    };

    class u8string : public std::basic_string<u8char_t> {
    public:
        u8string();
        u8string( const std::basic_string<u8char_t>& );
        u8string( const std::wstring& );
        u8string( const std::basic_string<u16char_t>& );
        u8string( const std::basic_string<u32char_t>& );
    };

    class string {
    public:
        string(void);
        ~string(void);

        static std::wstring convert( const std::string& );
        static std::string convert( const std::wstring& );

        static u8string utf8( const wchar_t * );
        static u8string utf8( const u16char_t * );
        static u8string utf8( const u32char_t * );
		static u16string utf16( const wchar_t * );
		static u32string utf32( const wchar_t * );
    };
}