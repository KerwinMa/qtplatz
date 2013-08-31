/**************************************************************************
** Copyright (C) 2010-2013 Toshinobu Hondo, Ph.D.
** Copyright (C) 2013 MS-Cheminformatics LLC
*
** Contact: info@ms-cheminfo.com
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

#pragma once

#include <string>
#include <boost/any.hpp>
#include <adinterface/controlmethodC.h>
#include <adinterface/controlmethodhelper.hpp>

namespace adinterface {

    class ControlMethodAccessor {
    public:
        static bool isPointer( boost::any& a ) {
#if defined __GNUC__ 
            // See issue on boost.  https://svn.boost.org/trac/boost/ticket/754
            if ( std::strcmp( a.type().name(), typeid( ::ControlMethod::Method * ).name() ) == 0 )
                return true;
#else
            if ( a.type() == typeid( ::ControlMethod::Method * ) )
                return true;
#endif
            return false;
        }
        static bool isReference( boost::any& a ) {
#if defined __GNUC__ 
            // See issue on boost.  https://svn.boost.org/trac/boost/ticket/754
            if ( std::strcmp( a.type().name(), typeid( ::ControlMethod::Method * ).name() ) == 0 )
                return true;
#else
            if ( a.type() == typeid( ::ControlMethod::Method * ) )
                return true;
#endif
            return false;
        }

        static ::ControlMethod::Method * out( boost::any& a ) {
            if ( isPointer( a ) ) {
                ::ControlMethod::Method * m = boost::any_cast< ::ControlMethod::Method * >( a );
                return m;
            }
            return 0;
        }
            
        static const ::ControlMethod::Method * in( boost::any& a ) {
            if ( isReference( a ) ) {
                const ::ControlMethod::Method& m = boost::any_cast< const ::ControlMethod::Method& >( a );
                return &m;
            }
            return 0;
        }
    };

    template<class IM, class Serializer>
    class ControlMethodAccessorT {
        std::wstring instId_;
        unsigned int unitNumber_;
    public:
        ControlMethodAccessorT( const std::wstring& instId
                                , unsigned int unitnumber = 0 ) : instId_( instId )
                                                                , unitNumber_( unitnumber ) {
        }

        bool setMethod( boost::any& a, const IM& im ) const {
            if ( ::ControlMethod::Method * out = ControlMethodAccessor::out( a ) ) {
                std::string device;
                if ( Serializer::serialize( im, device ) ) {
                    ::ControlMethod::MethodLine * line = ControlMethodHelper::findFirst( *out, instId_, unitNumber_ );
                    if ( line == 0 ) {
						::ControlMethod::MethodLine& t = ControlMethodHelper::add( *out, instId_, unitNumber_ );
						line = &t;
					}
                    line->xdata.length( device.size() );
                    std::copy( device.begin(), device.end(), reinterpret_cast< char *>(line->xdata.get_buffer()) );
                    return true;
                }
            }
            return false;
        }

        bool getMethod( IM& im, boost::any& a ) {
            if ( const ::ControlMethod::Method * in = ControlMethodAccessor::in( a ) ) {
                const ::ControlMethod::Method& m = boost::any_cast< const ::ControlMethod::Method& >( a );
                const ::ControlMethod::MethodLine * line = ControlMethodHelper::findFirst( m, instId_, unitNumber_ );
                if ( line && line->xdata.length() > 0 )
                    return Serializer::deserialize( im, reinterpret_cast< const char * >(line->xdata.get_buffer()), line->xdata.length() );
            }
            return false;
        }
    };

}

