// -*- C++ -*-
/**************************************************************************
** Copyright (C) 2010-2011 Toshinobu Hondo, Ph.D.
** Copyright (C) 2013 MS-Cheminformatics LLC
*
** Contact: info@ms-cheminfo.com
**
** Commercial Usage
**
** Licensees holding valid MS-Cheminformatics commercial licenses may use this
** file in accordance with the MS-Cheminformatics Commercial License Agreement
** provided with the Software or, alternatively, in accordance with the terms
** contained in a written agreement between you and MS-Cheminformatics.
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

#include "adbroker.hpp"
#include <adplugin/visitor.hpp>
#include <adportable/debug.hpp>
#include <boost/thread/mutex.hpp>
#include <typeinfo>

#if defined WIN32 && _MSC_VER
#  if defined _DEBUG || defined DEBUG
#     pragma comment(lib, "TAO_Utilsd.lib")
#     pragma comment(lib, "TAO_PId.lib")
#     pragma comment(lib, "TAO_PortableServerd.lib")
#     pragma comment(lib, "TAO_AnyTypeCoded.lib")
#     pragma comment(lib, "TAOd.lib")
#     pragma comment(lib, "ACEd.lib")
#     pragma comment(lib, "adinterfaced.lib")
#     pragma comment(lib, "adplugind.lib")
#     pragma comment(lib, "adportabled.lib")
#     pragma comment(lib, "acewrapperd.lib")
#     pragma comment(lib, "adcontrolsd.lib")
#     pragma comment(lib, "portfoliod.lib")
#  else
#     pragma comment(lib, "TAO_Utils.lib")
#     pragma comment(lib, "TAO_PI.lib")
#     pragma comment(lib, "TAO_PortableServer.lib")
#     pragma comment(lib, "TAO_AnyTypeCode.lib")
#     pragma comment(lib, "TAO.lib")
#     pragma comment(lib, "ACE.lib")
#     pragma comment(lib, "adinterface.lib")
#     pragma comment(lib, "adplugin.lib")
#     pragma comment(lib, "adportable.lib")
#     pragma comment(lib, "acewrapper.lib")
#     pragma comment(lib, "adcontrols.lib")
#     pragma comment(lib, "portfolio.lib")
#  endif
#endif

# include <tao/Utils/ORB_Manager.h>
# include <ace/Thread_Manager.h>
# include <ace/Process_Manager.h>

#include <acewrapper/orbservant.hpp>
#include <acewrapper/constants.hpp>
#include <acewrapper/acewrapper.hpp>
#include <adportable/debug.hpp>

#include "manager_i.hpp"
#include "brokermanager.hpp"

using namespace acewrapper;

adBroker::adBroker(void)
{
}

adBroker::~adBroker(void)
{
}

void *
adBroker::query_interface_workaround( const char * _typenam )
{
    const std::string typenam( _typenam );
    if ( typenam == typeid( adplugin::orbServant ).name() )
        return static_cast< adplugin::orbServant * >(this);
    else if ( typenam == typeid( adplugin::plugin ).name() )
        return static_cast< adplugin::plugin * >(this);
    return 0;
}

bool
adBroker::initialize( CORBA::ORB_ptr orb, PortableServer::POA_ptr poa, PortableServer::POAManager_ptr mgr )
{
    ORBServant< adbroker::manager_i > * pServant = adbroker::singleton::manager::instance();
	pServant->initialize( orb, poa, mgr );
	return true;
}

const char *
adBroker::activate()
{
    ORBServant< adbroker::manager_i > * pServant = adbroker::singleton::manager::instance();
	pServant->activate();
    return pServant->ior().c_str();
}

bool
adBroker::deactivate()
{
	adbroker::BrokerManager::terminate(); // terminate task
	adbroker::singleton::manager::instance()->deactivate();
	return true;
}

void
adBroker::initial_reference( const char * )
{
    // do nothing
}

adBroker::operator bool() const
{
    return true;
}


class adbroker_plugin : public adplugin::plugin
                      , public adplugin::orbFactory {

    static adbroker_plugin * instance_;
    adbroker_plugin() {}
    ~adbroker_plugin() {}
public:
    static adbroker_plugin * instance();
    // plugin
    virtual const char * iid() const;
    virtual void accept( adplugin::visitor&, const char * );
    virtual void * query_interface_workaround( const char * typenam );

    // orbFactory
    virtual adplugin::orbServant * create_instance() {
        return new adBroker;        
    }
};

adbroker_plugin * adbroker_plugin::instance_ = 0;
static boost::mutex __mutex;

adbroker_plugin *
adbroker_plugin::instance()
{
    if ( instance_ == 0 ) {
        boost::mutex::scoped_lock lock( __mutex );
        if ( instance_ == 0 )
            instance_ = new adbroker_plugin();
    }
    return instance_;
}

const char *
adbroker_plugin::iid() const
{
    return "com.ms-cheminfo.qtplatz.plugins.orbfactory.adbroker";
}

void
adbroker_plugin::accept( adplugin::visitor& v, const char * adplugin )
{
	v.visit( this, adplugin );
}

void *
adbroker_plugin::query_interface_workaround( const char * typenam )
{
    adportable::debug(__FILE__, __LINE__) << "##### query_interface_workaround for " << typenam << " called.";
    if ( std::string( typenam ) == typeid( orbFactory ).name() )
        return static_cast<orbFactory *>(this);
    return 0;
}

Q_DECL_EXPORT adplugin::plugin * adplugin_plugin_instance()
{
    return adbroker_plugin::instance();
}
