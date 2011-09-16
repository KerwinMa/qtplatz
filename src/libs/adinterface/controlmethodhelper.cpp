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

#include "controlmethodhelper.hpp"

using namespace adinterface;

ControlMethodHelper::ControlMethodHelper()
{
}

ControlMethodHelper::ControlMethodHelper( const ControlMethodHelper& t ) : method_(t.method_)
{
}

ControlMethodHelper::ControlMethodHelper( const ControlMethod::Method& m ) : method_(m)
{
}

const wchar_t *
ControlMethodHelper::subject() const 
{
    return method_.subject.in();
}

void
ControlMethodHelper::subject( const std::wstring& text )
{
    method_.subject = CORBA::wstring_dup( text.c_str() );
}

const wchar_t *
ControlMethodHelper::description() const 
{
    return method_.description.in();
}

void
ControlMethodHelper::description( const std::wstring& text )
{
    method_.description = CORBA::wstring_dup( text.c_str() );
}

unsigned int
ControlMethodHelper::findInstrument( const std::wstring& modelname, unsigned long unitnumber )
{
    for ( size_t i = 0; i < method_.iinfo.length(); ++i ) {
        if ( modelname == method_.iinfo[i].modelname.in() && unitnumber == method_.iinfo[i].unit_number )
            return i;
    }
    return (-1); // error
}

::ControlMethod::InstInfo&
ControlMethodHelper::addInstrument( const std::wstring& modelname, unsigned long unitnumber )
{
    unsigned int index;
    if ( ( index = findInstrument( modelname, unitnumber ) ) != unsigned(-1) ) {
        method_.iinfo.length( method_.iinfo.length() + 1 );
        ControlMethod::InstInfo& info = method_.iinfo[ method_.iinfo.length() - 1 ];
        info.index = method_.iinfo.length() - 1;
        return info;
    }
    return method_.iinfo[ index ];
}

::ControlMethod::MethodLine&
ControlMethodHelper::add( const std::wstring& modelname, unsigned long unitnumber )
{
    method_.lines.length( method_.lines.length() + 1 );
    ::ControlMethod::MethodLine&  line = method_.lines[ method_.lines.length() - 1 ];

    line.modelname = CORBA::wstring_dup( modelname.c_str() );
    line.index = findInstrument( modelname, unitnumber );
    line.unitnumber = unitnumber;
    line.isInitialCondition = true;

    return line;
}

/////////////////
unsigned long
ControlMethodLine::index() const
{
    return line_.index;
}

const wchar_t *
ControlMethodLine::modelname() const
{
    return line_.modelname.in();
}

void
ControlMethodLine::modelname( const std::wstring& modelname )
{
    line_.modelname = CORBA::wstring_dup( modelname.c_str() );
}

unsigned long
ControlMethodLine::unitnumber() const
{
    return line_.unitnumber;
}

void
ControlMethodLine::unitnumber( unsigned long unitnumber )
{
    line_.unitnumber = unitnumber;
}

bool
ControlMethodLine::isInitialCondition() const
{
    return line_.isInitialCondition;
}

void
ControlMethodLine::isInitialCondition( bool d )
{
    line_.isInitialCondition = d;
}

//////////////

ControlMethodInstInfo::ControlMethodInstInfo( ControlMethod::InstInfo& t ) : info_( t )
{
}

unsigned long
ControlMethodInstInfo::index() const
{
    return info_.index;
}

unsigned long
ControlMethodInstInfo::unit_number() const // 0..n
{
    return info_.unit_number;
}

::ControlMethod::eDeviceCategory
ControlMethodInstInfo::category() const
{
    return info_.category;
}

const wchar_t *
ControlMethodInstInfo::modelname() const
{
    return info_.modelname.in();
}

const wchar_t *
ControlMethodInstInfo::serial_number() const
{
    return info_.serial_number.in();
}

const wchar_t *
ControlMethodInstInfo::description() const
{
    return info_.description.in();
}