/**************************************************************************
** Copyright (C) 2010-2011 Toshinobu Hondo, Ph.D.
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

#include "manager.hpp"
#include "adplugin.hpp"
#include "lifecycle.hpp"
#include "orbLoader.hpp"
#include "constants.hpp"
#include "plugin.hpp"
#include "visitor.hpp"
#include "lifecycleaccessor.hpp"
#include <adportable/configuration.hpp>
#include <acewrapper/constants.hpp>
#include <adportable/configloader.hpp>
#include <adportable/debug.hpp>
#include <adportable/string.hpp>
#include <qtwrapper/qstring.hpp>
#include <QLibrary>
#include <QDir>
#include <QMessageBox>
#include <QWidget>
#include <map>
#include <fstream>
#include <boost/smart_ptr.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/cast.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/foreach.hpp>
#include <boost/regex.hpp>

//----------------------------------

using namespace adplugin;

boost::mutex __mutex;
manager * manager::instance_ = 0;

namespace adplugin { namespace internal {

        class plugin_data {
        public:
            adplugin::plugin * plugin_;
            plugin_data( adplugin::plugin * plugin ) : plugin_( plugin ) {
            }
            plugin_data( const plugin_data& t ) : plugin_( t.plugin_ ) {
            }
            const char * clsid() const { return plugin_->clsid(); }
            const char * iid() const { return plugin_->iid(); }
            adplugin::plugin * plugin() const { return plugin_; }
        };

        class manager_data : boost::noncopyable
                           , adplugin::visitor {
        public:
			typedef std::vector< plugin_data > vector_type;
            typedef std::map< std::string, vector_type > map_type;
			
			// manager
            bool install( QLibrary&, const std::string& );
            plugin * select_iid( const char * iid );
            plugin * select_clsid( const char * clsid ); // return first match
            size_t select_iids( const char * clsid, std::vector< plugin * >& );
            size_t select_clsids( const char * clsid, std::vector< plugin * >& );

            // visitor
            void visit( adplugin::plugin * plugin, const char * adplugin );
		private:
			map_type plugins_;
        };
    }
}


manager::manager(void) : d_( new internal::manager_data )
{
}

manager::~manager(void)
{
    delete d_;
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

       // std::map< std::string, adplugin::orbFactory * > orbFactories_;

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
	if ( instance_ == 0 ) {
		boost::mutex::scoped_lock lock( __mutex );
		if ( instance_ == 0 ) {
			instance_ = new manager_impl();
			atexit( &manager::dispose );
		}
	}
	return instance_;
}

void
manager::dispose()
{
	delete instance_;
}

// static
QWidget *
manager::widget_factory( const adportable::Configuration& config, const wchar_t * path, QWidget * parent )
{
    if ( config.module().library_filename().empty() )
        return 0;
    
    boost::filesystem::path basepath( path );
    boost::filesystem::path loadfile = basepath / pluginDirectory / config.module().library_filename();
    
    adplugin::ifactory * pfactory = manager::instance()->loadFactory( loadfile.wstring() );
    if ( pfactory ) {
        QWidget * pWidget = pfactory->create_widget( config._interface().c_str(), parent );

        adplugin::LifeCycle * pLifeCycle = dynamic_cast< adplugin::LifeCycle * > ( pWidget );
        if ( pLifeCycle == 0 ) {
            // gcc with dlopen() loaded class cannot dynamic_cast unless using RTLD_GLOBA flag.
            // see http://gcc.gnu.org/faq.html#dso
            // since I'd like to use QLibrary which does not apply RTLD_GLOBAL flag, use Qt's signal/slot instead
            LifeCycleAccessor accessor( pWidget );
            pLifeCycle = accessor.get();
        }

        if ( pLifeCycle )
            pLifeCycle->OnCreate( config );
        return pWidget;
    }
    return 0;
}

std::string
manager::iorBroker()
{
    const char * p = manager::instance()->lookup_ior( acewrapper::constants::adbroker::manager::_name() );
    return p ? std::string( p ) : std::string("");
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
            ad_plugin_instance_t instance = reinterpret_cast<ad_plugin_instance_t>( lib.resolve( "ad_plugin_instance" ) );
            if ( instance ) {
                libraries_[ filename ] = instance();
            } else {
                adportable::debug dbg(__FILE__, __LINE__);
                dbg << filename << " \"ad_plugin_instance\" cound not be found";
                QMessageBox::critical( 0, "adplugin::orbLoader", dbg.str().c_str() );
            }
        } else {
            adportable::debug dbg(__FILE__, __LINE__);
			dbg << "manager_impl::loadFactory(" << filename << ")" << qtwrapper::wstring::copy( lib.errorString() );
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

	boost::filesystem::path path( qtwrapper::wstring::copy( dir.absolutePath() ) );
	path /= ".ior";
	if ( ! boost::filesystem::exists( path ) )
		boost::filesystem::create_directory( path );
    path /= name;
	path.replace_extension( ".ior" );
	boost::filesystem::ofstream of( path );

    of << ior;
}

const char *
manager_impl::lookup_ior( const std::string& name )
{
    std::map< std::string, std::string >::iterator it = iorMap_.find( name );
    if ( it != iorMap_.end() )
        return it->second.c_str();
	return 0;
}

bool
manager::install( QLibrary& lib, const std::string& adplugin_conf )
{
    return d_->install( lib, adplugin_conf );
}

plugin *
manager::select_iid( const char * iid ) 
{
    return d_->select_iid( iid );
}

plugin * 
manager::select_clsid( const char * clsid )
{
    return d_->select_clsid( clsid );
}

size_t
manager::select_iids( const char * clsid, std::vector< plugin * >& vec )
{
    return d_->select_iids( clsid, vec );
}

size_t
manager::select_clsids( const char * clsid, std::vector< plugin * >& vec )
{
    return d_->select_clsids( clsid, vec );
}

////////////////////////////////////////

class ORBLoaderError : public adplugin::orbLoader {
    std::string errmsg_;
public:
    ORBLoaderError( const std::string& errmsg ) : errmsg_( errmsg ) {}
    ~ORBLoaderError() { }
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

    boost::filesystem::path filepath( file );
    boost::system::error_code ec;
    if ( ! boost::filesystem::exists( filepath, ec ) ) {
        adportable::debug dbg(__FILE__, __LINE__);
        dbg << "file \"" << filepath.string() << "\" is requested to load library but it does not exist";
        failedLoaders_[ file ].reset( new ORBLoaderError( dbg.str() ) );
        return *failedLoaders_[ file ];
    }
    QLibrary lib( filepath.string().c_str() );
    if ( lib.load() ) {
        typedef adplugin::orbLoader * (*instance_t)();
        instance_t instance = reinterpret_cast<instance_t>( lib.resolve( "instance" ) );
        if ( instance ) {
            boost::shared_ptr< adplugin::orbLoader > loader( instance() );
            if ( loader )
                orbLoaders_[ file ] = loader;
        } else {
            adportable::debug dbg(__FILE__, __LINE__);
            dbg << "library \"" << filepath.string() << "\" loaded but no \"instance()\" method";
            failedLoaders_[ file ].reset( new ORBLoaderError( dbg.str() ) );
            return *failedLoaders_[ file ];
        }
    }
    if ( ( it = orbLoaders_.find( file ) ) != orbLoaders_.end() )
        return *it->second;

    // exists library but failed to load
    adportable::debug dbg(__FILE__, __LINE__);
    dbg << static_cast< const char * >( lib.errorString().toUtf8() );
    
    failedLoaders_[ file ].reset( new ORBLoaderError( dbg.str() ) );
    return *failedLoaders_[ file ];
}


// static
std::wstring
orbLoader::library_fullpath( const std::wstring& apppath, const std::wstring& library_filename )
{
	boost::filesystem::path path = boost::filesystem::path( apppath ) / pluginDirectory;
	boost::filesystem::path fullpath = path / library_filename; 
	return fullpath.wstring();
}

std::wstring
orbLoader::config_fullpath( const std::wstring& apppath, const std::wstring& config_filename )
{
	boost::filesystem::path path = boost::filesystem::path( apppath ) / pluginDirectory;
    boost::filesystem::path fullpath = path / config_filename; 
	return fullpath.wstring();
}

///////////////////////

using namespace adplugin::internal;

bool
manager_data::install( QLibrary& lib, const std::string& adplugin )
{
    if ( plugins_.find( adplugin ) != plugins_.end() )
        return false; // already loaded
    
	typedef adplugin::plugin * (*factory)();

    if ( lib.isLoaded() ) {
        factory f = reinterpret_cast< factory >( lib.resolve( "adplugin_plugin_instance" ) );
		adplugin::plugin * plugin = f();
		if ( plugin ) {
            plugin->clsid_ = adplugin;
			plugin->accept( *this, adplugin.c_str() );
        }
    }
	return true;
}

void
manager_data::visit( adplugin::plugin* plugin, const char * adplugin )
{
    if ( adplugin == 0 || plugin == 0 )
        return;
    plugins_[ adplugin ].push_back( plugin_data( plugin ) );
}

plugin *
manager_data::select_iid( const char * iid ) 
{
    // return first find
    for ( map_type::const_iterator it = plugins_.begin(); it != plugins_.end(); ++it ) {
		BOOST_FOREACH( const plugin_data& d, it->second ) {
            boost::regex re( iid );
            boost::cmatch matches;
            if ( boost::regex_match( d.iid(), matches, re ) ) {
#if defined _DEBUG || defined DEBUG
                for ( size_t i = 0; i < matches.size(); ++i )
                    std::cout << "matches[" << i << "]=" << matches[i].first << ", " << matches[i].second << std::endl;
#endif
                return d.plugin();
            }
		}
	}
    return 0;
}

plugin * 
manager_data::select_clsid( const char * clsid )
{
    for ( map_type::const_iterator it = plugins_.begin(); it != plugins_.end(); ++it ) {
		BOOST_FOREACH( const plugin_data& d, it->second ) {
            boost::regex re( clsid );
            boost::cmatch matches;
            if ( boost::regex_match( d.clsid(), matches, re ) )
                return d.plugin();
		}
	}
    return 0;
}

size_t
manager_data::select_clsids( const char * regex, std::vector< plugin * >& vec )
{
    for ( map_type::const_iterator it = plugins_.begin(); it != plugins_.end(); ++it ) {
		BOOST_FOREACH( const plugin_data& d, it->second ) {
            boost::regex re( regex );
            boost::cmatch matches;
            if ( boost::regex_match( d.clsid(), matches, re ) ) {
#if defined _DEBUG || defined DEBUG
                for ( size_t i = 0; i < matches.size(); ++i )
                    std::cout << "matches[" << i << "]=" << matches[i].first << ", " << matches[i].second << std::endl;
#endif
                vec.push_back( d.plugin() );
            }
		}
	}
	return vec.size();
}

size_t
manager_data::select_iids( const char * regex, std::vector< plugin * >& vec )
{
    for ( map_type::const_iterator it = plugins_.begin(); it != plugins_.end(); ++it ) {
		BOOST_FOREACH( const plugin_data& d, it->second ) {
            boost::regex re( regex );
            boost::cmatch matches;
            if ( boost::regex_match( d.iid(), matches, re ) ) {
#if defined _DEBUG || defined DEBUG
                for ( size_t i = 0; i < matches.size(); ++i )
                    std::cout << "matches[" << i << "]=" << matches[i].first << ", " << matches[i].second << std::endl;
#endif
                vec.push_back( d.plugin() );
            }
		}
	}
	return vec.size();
}

//////////////////////////////////////
