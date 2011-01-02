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
#include "description.h"
#include <boost/smart_ptr.hpp>

namespace boost {
  namespace serialization {
	class access;
  }
}

namespace adcontrols {

   namespace internal {
       class DescriptionsImpl;
   }

   class ADCONTROLSSHARED_EXPORT Descriptions {
   public:
	   ~Descriptions();
	   Descriptions();
	   Descriptions( const Descriptions& );
	   Descriptions( const std::wstring& text, const std::wstring& key );

	   void operator = ( const Descriptions& );

	   void append( const Description&, bool uniq = false );
	   size_t size() const;
	   const Description& operator [](int idx) const;

	   std::wstring saveXml() const;
	   void loadXml( const std::wstring& xml );

   private:
	   friend class boost::serialization::access;
	   template<class Archiver> void serialize(Archiver& ar, const unsigned int version);
	   internal::DescriptionsImpl* pImpl_;
   };

}

