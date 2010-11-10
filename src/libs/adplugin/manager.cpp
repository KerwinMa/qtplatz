//////////////////////////////////////////
// Copyright (C) 2010 Toshinobu Hondo, Ph.D.
// Science Liaison / Advanced Instrumentation Project
//////////////////////////////////////////

#include "manager.h"
#include "adplugin.h"
#include "lifecycle.h"
#include "orbLoader.h"
#include <adportable/configuration.h>
#include <acewrapper/constants.h>
#include <adportable/configloader.h>
#include <adportable/debug.h>
#include <adportable/string.h>
#include <qtwrapper/qstring.h>
#include <ace/Singleton.h>
#include <QLibrary>
#include <QDir>
#include <QMessageBox>

#include <map>
#include <fstream>
#include <boost/smart_ptr.hpp>


#include <QWidget>

using namespace adplugin;

manager::manager(void)
{
}

manager::~manager(void)
{
}

////////////////

namespace adplugin {

    class manager_impl : public manager {
    public:
        ~manager_impl();
        manager_impl();

        // manager impl
        bool loadConfig( adportable::Configuration&, const std::wstring&, const wchar_t * );
        adplugin::ifactory * loadFactory( const std::wstring& );
        bool unloadFactory( const std::wstring& );

        virtual adplugin::orbLoader& orbLoader( const std::wstring& name );

        virtual void register_ior( const std::string& name, const std::string& ior );
        virtual const char * lookup_ior( const std::string& name );

    private:
        typedef std::map< std::wstring, adplugin::ifactory * > librariesType;
        typedef std::map< std::wstring, boost::shared_ptr<adplugin::orbLoader> > orbLoadersType;

        librariesType libraries_;
        orbLoadersType orbLoaders_;
		orbLoadersType failedLoaders_;
        std::map< std::string, std::string > iorMap_;
    };
}
////////////////////////////////////
manager *
manager::instance()
{
    typedef ACE_Singleton< manager_impl, ACE_Recursive_Thread_Mutex > impl;
    return impl::instance();
}

// static
QWidget *
manager::widget_factory( const adportable::Configuration& config, const wchar_t * path, QWidget * parent )
{
	if ( config.module().library_filename().empty() )
		return 0;

    std::wstring appbase( path );
	std::wstring loadfile = appbase + config.module().library_filename();

    adplugin::ifactory * pfactory = manager::instance()->loadFactory( loadfile );
	if ( pfactory ) {
        QWidget * pWidget = pfactory->create_widget( config.interface().c_str(), parent );
        adplugin::LifeCycle * pLifeCycle = dynamic_cast< adplugin::LifeCycle * > ( pWidget );
		if ( pLifeCycle )
            pLifeCycle->OnCreate( config );
		return pWidget;
	}
    return 0;
}

std::string
manager::iorBroker()
{
	return manager::instance()->lookup_ior( acewrapper::constants::adbroker::manager::_name() );
}

std::string
manager::ior( const char * name )
{
	return manager::instance()->lookup_ior( name );
}

///////////////////////////////////

manager_impl::manager_impl()
{
}

manager_impl::~manager_impl()
{
}

bool
manager_impl::loadConfig( adportable::Configuration& config, const std::wstring& filename, const wchar_t * query )
{
    return adportable::ConfigLoader::loadConfigFile( config, filename, query );
}

adplugin::ifactory *
manager_impl::loadFactory( const std::wstring& filename )
{
	librariesType::iterator it = libraries_.find( filename );
	if ( it == libraries_.end() ) {
        QLibrary lib( qtwrapper::qstring::copy(filename) );
        if ( lib.load() ) {
            typedef adplugin::ifactory * (*ad_plugin_instance_t)();
            ad_plugin_instance_t instance = static_cast<ad_plugin_instance_t>( lib.resolve( "ad_plugin_instance" ) );
            if ( instance ) {
                libraries_[ filename ] = instance();
            } else {
                QMessageBox::critical( 0, "adplugin::orbLoader", "some method(s) can not be assigned" );
            }
        }
    }
    if ( ( it = libraries_.find( filename ) ) != libraries_.end() )
        return it->second;

    return 0;
}

bool
manager_impl::unloadFactory( const std::wstring& filename )
{
	librariesType::iterator it = libraries_.find( filename );
	if ( it != libraries_.end() ) {
        if ( it->second )
            it->second->release();
        return true;
	}
    return false;
}

void
manager_impl::register_ior( const std::string& name, const std::string& ior )
{
	iorMap_[name] = ior;

	QDir dir = QDir::home();
	if ( ! dir.exists( ".ior" ) )
		dir.mkdir( ".ior" );
	dir.cd( ".ior" );
	std::string path = dir.absolutePath().toStdString();
	path += std::string("/") + name + ".ior";
	std::ofstream of( path.c_str() );
	of << ior;
}

const char *
manager_impl::lookup_ior( const std::string& name )
{
#if defined _DEBUG
	std::string path = QDir::home().absolutePath().toStdString();
	path += std::string( "/.ior/" ) + name + ".ior";
	std::ifstream inf( path.c_str() );

	if ( ! inf.fail() ) {
		std::string ior;
		inf >> ior;
	}
#endif

	std::map< std::string, std::string >::iterator it = iorMap_.find( name );
	if ( it != iorMap_.end() )
		return it->second.c_str();
	return 0;
}


//////////////////////////////////////
////////////////////////////////////////

class ORBLoaderError : public adplugin::orbLoader {
	std::string errmsg_;
public:
	ORBLoaderError::ORBLoaderError( const std::string& errmsg ) : errmsg_( errmsg ) {}
	ORBLoaderError::~ORBLoaderError() { }
	virtual operator bool() const { return false; }
	virtual bool initialize( CORBA::ORB *, PortableServer::POA * , PortableServer::POAManager * ) { return false; }
	virtual void initial_reference( const char * ) { }
	virtual const char * activate() { return ""; }
	virtual bool deactivate() { return false; }
	virtual const char * error_description() { return errmsg_.c_str(); }
};

//-------------------
adplugin::orbLoader&
manager_impl::orbLoader( const std::wstring& file )
{
	orbLoadersType::iterator it = orbLoaders_.find( file );

	if ( it != orbLoaders_.end() )
		return *it->second;

    int rcode = _waccess( file.c_str(), 0 );
	if ( rcode ) {
		adportable::debug dbg;
		const char * reason = 0;
        if ( errno == EACCES ) reason = "access denied";
		else if ( errno == ENOENT ) reason = "file name or path not found";
		else if ( errno == EINVAL ) reason = "invalid parameter";
		else reason = "n/a";
		dbg << "error: " << reason << " '" << file << "'";
		failedLoaders_[ file ].reset( new ORBLoaderError( dbg.str() ) );
		return *failedLoaders_[ file ];
	}
    
	QLibrary lib( qtwrapper::qstring::copy( file ) );
	if ( lib.load() ) {
		typedef adplugin::orbLoader * (*instance_t)();
		instance_t instance = static_cast<instance_t>( lib.resolve( "instance" ) );
		if ( instance ) {
			boost::shared_ptr< adplugin::orbLoader > loader( instance() );
			if ( loader )
				orbLoaders_[ file ] = loader;
		} else {
			failedLoaders_[ file ].reset( new ORBLoaderError( "dll entry point 'instance' can not be found" ) );
			return *failedLoaders_[ file ];
		}
	}
	if ( ( it = orbLoaders_.find( file ) ) != orbLoaders_.end() )
		return *it->second;

	failedLoaders_[ file ].reset( new ORBLoaderError( "dll load failed." ) );
	return *failedLoaders_[ file ];
}