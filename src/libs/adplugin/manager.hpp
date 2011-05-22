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

#include "adplugin_global.h"
#include "ifactory.hpp"
#include <string>

class QString;
class QObject;
class QWidget;

namespace adportable {
	class Configuration;
	class Component;
}

namespace adplugin {

    class orbLoader;

	class ADPLUGINSHARED_EXPORT manager {
	protected:
            manager();
            ~manager();
	public:
            static manager * instance();
            static void dispose();
            static std::string ior( const char * name ); // return broker::manager's ior
            static std::string iorBroker();
            
            virtual bool loadConfig( adportable::Configuration&, const std::wstring&, const wchar_t * query ) = 0;

            virtual adplugin::ifactory * loadFactory( const std::wstring& ) = 0;
            virtual bool unloadFactory( const std::wstring& ) = 0;

            virtual orbLoader& orbLoader( const std::wstring& name ) = 0;
			virtual void register_ior( const std::string& name, const std::string& ior ) = 0;
            virtual const char * lookup_ior( const std::string& name ) = 0;

            static QWidget * widget_factory( const adportable::Configuration&, const wchar_t * path, QWidget * parent = 0 );
            
	private:

	};
}