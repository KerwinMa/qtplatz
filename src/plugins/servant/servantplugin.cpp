//////////////////////////////////////////
// Copyright (C) 2010 Toshinobu Hondo, Ph.D.
// Science Liaison / Advanced Instrumentation Project
//////////////////////////////////////////

#include "servantplugin.h"
#include "servantmode.h"
#include "logger.h"

#include <coreplugin/icore.h>
#include <coreplugin/uniqueidmanager.h>
#include <coreplugin/coreconstants.h>
#include <coreplugin/minisplitter.h>
#include <coreplugin/outputpane.h>
#include <coreplugin/navigationwidget.h>
#include <extensionsystem/pluginmanager.h>
#include <QtCore/qplugin.h>
#include <QtCore>
#pragma warning(disable:4996)
#include <ace/Thread_Manager.h>
#include <orbsvcs/CosNamingC.h>
#pragma warning(default:4996)

#include <adbroker/adbroker.h>
#include <adcontroller/adcontroller.h>
#include <adinterface/instrumentC.h>

#include <adplugin/adplugin.h>
#include <adplugin/orbLoader.h>
#include <adplugin/orbmanager.h>

#include <adportable/configuration.h>
#include <adportable/string.h>
#include <adportable/debug.h>
#include <acewrapper/acewrapper.h>
#include <acewrapper/orbservant.h>
#include <acewrapper/constants.h>
#include <acewrapper/brokerhelper.h>
#include <qtwrapper/qstring.h>
#include "outputwindow.h"
#include "servantpluginimpl.h"
#include <QMessageBox>
#include <boost/format.hpp>

using namespace servant;
using namespace servant::internal;

ServantPlugin::~ServantPlugin()
{
    delete pImpl_;
	delete pConfig_;
}

ServantPlugin::ServantPlugin() : pConfig_( 0 )
                               , pImpl_( 0 )
{
}

bool
ServantPlugin::initialize(const QStringList &arguments, QString *error_message)
{
    Q_UNUSED(arguments);
    int nErrors = 0;

    OutputWindow * outputWindow = new OutputWindow;
    addAutoReleasedObject( outputWindow );
    pImpl_ = new internal::ServantPluginImpl( outputWindow );

    // ACE initialize
	acewrapper::instance_manager::initialize();
	acewrapper::singleton::orbServantManager::instance()->init( 0, 0 );
    // <------

	std::wstring apppath;
	do {
		QDir dir = QCoreApplication::instance()->applicationDirPath();
		dir.cdUp();
		apppath = qtwrapper::wstring::copy( dir.path() );
	} while(0);

	std::wstring configFile = apppath + L"/lib/qtPlatz/plugins/ScienceLiaison/servant.config.xml";

	const wchar_t * query = L"/ServantConfiguration/Configuration";

	pConfig_ = new adportable::Configuration();
	adportable::Configuration& config = *pConfig_;

	if ( ! adplugin::manager::instance()->loadConfig( config, configFile, query ) ) {
		error_message = new QString( "loadConfig load failed" );
        adportable::debug() << "ServantPlugin::initialize loadConfig failed";
	}

    // --------------------------------
    std::string iorBroker;
	Broker::Manager_var mgr;
	do { // adborker, it has to be on top of servant activation
        CORBA::ORB_var orb = acewrapper::singleton::orbServantManager::instance()->orb();
        adBroker::initialize( orb ); 
        iorBroker = adBroker::activate();
        if ( ! iorBroker.empty() )
			adplugin::manager::instance()->register_ior( acewrapper::constants::adbroker::manager::_name(), iorBroker );
		adBroker::run();
  
        // ---- private naming service (Broker::Manager) start ----
		CORBA::Object_var obj = acewrapper::singleton::orbServantManager::instance()->orb()->string_to_object( iorBroker.c_str() );
		if ( ! CORBA::is_nil( obj ) ) {
			mgr = Broker::Manager::_narrow( obj );
			if ( CORBA::is_nil( mgr ) ) {
				QMessageBox::critical(0, "ServantPlugin", "Broker::Manager creation failed" );
                *error_message = "Broker::Manager creation failed";
				return false;
			}
		}
        // store broker's ior to self
        try {
            mgr->register_ior( acewrapper::constants::adbroker::manager::_name(), iorBroker.c_str() );
        } catch ( CORBA::Exception& ex ) {
            adportable::debug() << "CORBA::Exception: " << ex._info().c_str();
            QMessageBox::critical(0, "ServantPlugin - Broker::Manager::registor_ior", ex._info().c_str() );
            *error_message = "Broker::Manager::registor_ior exception";
            return false;
        }
 	} while ( 0 );

	for ( adportable::Configuration::vector_type::iterator it = config.begin(); it != config.end(); ++it ) {
        std::wstring name = it->name();
		std::wstring component = it->component();

		if ( name == L"adbroker" ) {
			/* nothing */
        } else if ( it->attribute(L"type") == L"orbLoader" ) {
			// adcontroller must be on top
            std::wstring file = apppath + it->module().library_filename();
            it->attribute( L"fullpath", file );
			std::string name = adportable::string::convert( it->attribute( L"ns_name" ) );
            adplugin::orbLoader& loader = adplugin::manager::instance()->orbLoader( file );
            if ( loader ) {
                loader.initialize( acewrapper::singleton::orbServantManager::instance()->orb() );
                loader.initial_reference( iorBroker.c_str() );
				std::string ior = loader.activate();            // activate object
                mgr->register_ior( name.c_str(), ior.c_str() ); // set ior to Broker::Manager
                loader.run();
            }
        }
	}

    Core::ICore * core = Core::ICore::instance();
    QList<int> context;
    if ( core ) {
        Core::UniqueIDManager * uidm = core->uniqueIDManager();
        if ( uidm ) {
            context.append( uidm->uniqueIdentifier( QLatin1String("Servant.MainView") ) );
            context.append( uidm->uniqueIdentifier( Core::Constants::C_NAVIGATION_PANE ) );
        }
    } else
        return false;

    // 
    ControlServer::Session_var session;
    std::vector< Instrument::Session_var > i8t_sessions;

	for ( adportable::Configuration::vector_type::iterator it = config.begin(); it != config.end(); ++it ) {

		if ( it->name() == L"adbroker" ) {
            pImpl_->init_debug_adbroker( this );
		} else if ( it->name() == L"adcontroller" ) {
            pImpl_->init_debug_adcontroller( this );
            // CORBA::Object_var obj;
            std::string ior = adplugin::manager::instance()->ior( acewrapper::constants::adbroker::manager::_name() );
            CORBA::Object_var obj = adplugin::ORBManager::instance()->string_to_object( ior );
            ControlServer::Manager_var manager = ControlServer::Manager::_narrow( obj );
			if ( ! CORBA::is_nil( manager ) ) {
                session = manager->getSession( L"debug" );
				session->setConfiguration( it->xml().c_str() );
			}

		} else if ( it->attribute( L"type" ) == L"orbLoader" ) {
			std::string ns_name = adportable::string::convert( it->attribute( L"ns_name" ) );
			if ( ! ns_name.empty() ) {
                //obj = acewrapper::singleton::orbManager::instance()->getObject( name );
				CORBA::String_var ior = mgr->ior( ns_name.c_str() );
                CORBA::Object_var obj = adplugin::ORBManager::instance()->string_to_object( ior.in() );
                try {
                    Instrument::Session_var isession = Instrument::Session::_narrow( obj );
                    if ( ! CORBA::is_nil( isession ) ) {
                        isession->setConfiguration( it->xml().c_str() );
                        i8t_sessions.push_back( isession );
                    }
                } catch ( CORBA::Exception& src ) {
                    Logger log;
                    log( (boost::format("CORBA::Exception for Instrument::Session(%1%) %2%") % ns_name.c_str() % src._info() ).str() );
                    QMessageBox mbx;
                    mbx.critical( 0, "Servant error", QString("CORBA::Exception: ") 
                        + ( boost::format("%1% : %2%") % src._name() % src._info()).str().c_str() );
                    ++nErrors;
                }
			}
		}
	}

    if ( ! CORBA::is_nil( session ) ) {
        for ( std::vector< Instrument::Session_var >::iterator it = i8t_sessions.begin(); it != i8t_sessions.end(); ++it )
            (*it)->configComplete();
        session->configComplete();
    }
    Logger log;
    log( ( nErrors ? L"Servant iitialized with errors" : L"Servernt initialized successfully") );

    do { adportable::debug() << "ServantPlugin::initialize() completed."; } while(0);
    return true;
}

void
ServantPlugin::extensionsInitialized()
{
    do { adportable::debug() << "ServantPlugin::extensionsInitialized() entered."; } while(0);
    OutputWindow * outputWindow = ExtensionSystem::PluginManager::instance()->getObject< servant::OutputWindow >();
    if ( outputWindow )
        outputWindow->appendLog( L"ServantPlugin::extensionsInitialized()" );
}

void
ServantPlugin::shutdown()
{
	adportable::Configuration& config = *pConfig_;

    // destriction must be reverse order
	for ( adportable::Configuration::vector_type::reverse_iterator it = config.rbegin(); it != config.rend(); ++it ) {
		std::wstring name = it->name();
		if ( name == L"adbroker" ) {
			// adBroker::deactivate();
			// } else if ( name == L"adcontroller" ) {
			// adController::deactivate();
        } else if ( it->attribute(L"type") == L"orbLoader" ) {
			std::wstring file = it->attribute( L"fullpath" );
			adplugin::orbLoader& loader = adplugin::manager::instance()->orbLoader( file );
			if ( loader )
				loader.deactivate();
		}
	}
	adBroker::deactivate();
    Logger::shutdown();

	acewrapper::singleton::orbServantManager::instance()->orb()->shutdown();
	acewrapper::singleton::orbServantManager::instance()->fini();
	ACE_Thread_Manager::instance()->wait();
}




Q_EXPORT_PLUGIN( ServantPlugin )
