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

#include "adcontrols_global.h"
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/version.hpp>
#include <string>

namespace adcontrols {

    class ADCONTROLSSHARED_EXPORT annotation {
    public:
        enum dataType {
            dataText
            , dataSvg
        };
        annotation();
        annotation( const std::wstring&, double x = 0, double y = 0, int id = (-1), int priority = 0, dataType typ = dataText );
        annotation( const annotation& );

        const std::wstring& text() const;
        void text( const std::wstring& text );

        int index() const;
        void index( int );

        dataType type() const;
        void dataType( dataType );

        int priority() const;
        void priority( int );

        double x() const;
        double y() const;
        double width() const;
        double height() const;
        void rect( double x, double y, double width = 0, double height = 0 );
        void x( double );
        void y( double );

    private:
        enum dataType type_;
        int index_;
        int priority_;
#if defined _MSC_VER
# pragma warning( disable: 4251 )
#endif
        std::wstring text_;

        double x_, y_;
        double w_, h_;
        
        friend class boost::serialization::access;
        template<class Archive>
            void serialize( Archive& ar, const unsigned int /* version */) {
            ar & BOOST_SERIALIZATION_NVP( type_ )
                & BOOST_SERIALIZATION_NVP( index_ )
                & BOOST_SERIALIZATION_NVP( priority_ )
                & BOOST_SERIALIZATION_NVP( text_ )
                & BOOST_SERIALIZATION_NVP( x_ )
                & BOOST_SERIALIZATION_NVP( y_ )
                & BOOST_SERIALIZATION_NVP( w_ )
                & BOOST_SERIALIZATION_NVP( h_ )
                ;
        }
    };

}


