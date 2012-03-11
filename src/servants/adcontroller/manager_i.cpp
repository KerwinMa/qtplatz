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

#include "manager_i.hpp"
#include "session_i.hpp"
#include "taskmanager.hpp"
// #include <adinterface/brokerC.h>

using namespace adcontroller;

namespace adcontroller {
    namespace singleton {
        typedef ACE_Singleton< acewrapper::ORBServant< ::adcontroller::manager_i >, ACE_Recursive_Thread_Mutex > manager_i;
    }
}

manager_i::manager_i(void) 
{
}

manager_i::~manager_i(void)
{
}

acewrapper::ORBServant< manager_i > *
manager_i::instance()
{
    return singleton::manager_i::instance();
}

void
manager_i::shutdown()
{
    // PortableServer::POA_var poa = singleton::manager::instance()->poa();
}

ControlServer::Session_ptr
manager_i::getSession( const CORBA::WChar * token )
{
    PortableServer::POA_var poa = manager_i::instance()->poa(); // getServantManager()->root_poa();

    if ( CORBA::is_nil( poa ) )
        return 0;

    if ( session_list_.empty() )
        adcontroller::iTaskManager::instance()->manager_initialize();

    session_map_type::iterator it = session_list_.find( token );
    if ( it == session_list_.end() ) 
        session_list_[ token ].reset( new adcontroller::session_i() );

    CORBA::Object_ptr obj = poa->servant_to_reference( session_list_[ token ].get() );
    return ControlServer::Session::_narrow( obj );
}

Broker::Logger_ptr
manager_i::getLogger()
{
    if ( CORBA::is_nil( broker_manager_ ) ) {
        std::string ior = manager_i::instance()->broker_manager_ior();
        if ( ior.empty() )
            return 0;
        CORBA::ORB_var orb = manager_i::instance()->orb();
        CORBA::Object_var obj = orb->string_to_object( ior.c_str() );
        broker_manager_ = Broker::Manager::_narrow( obj );
        if ( ! CORBA::is_nil( broker_manager_ ) )
            logger_ = broker_manager_->getLogger();
    }
    if ( ! CORBA::is_nil( logger_ ) )
        return Broker::Logger::_duplicate( logger_ );
    return 0;
}
