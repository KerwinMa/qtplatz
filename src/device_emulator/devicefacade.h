// This is a -*- C++ -*- header.
//////////////////////////////////////////
// Copyright (C) 2010 Toshinobu Hondo, Ph.D.
// Science Liaison / Advanced Instrumentation Project
//////////////////////////////////////////

#pragma once

#include <QObject>
#include <boost/variant.hpp>
#include <adportable/protocollifecycle.h>
#include <ace/Task.h>
#include <ace/Barrier.h>

template<class T, class X> class ACE_Singleton;
class ACE_Recursive_Thread_Mutex;
class ACE_Message_Block;
class ACE_INET_Addr;
class ACE_OutputCDR;
class ACE_InputCDR;
class DeviceFacadeImpl;

namespace acewrapper {
    class DgramHandler;
    class McastHandler;
}

namespace device_emulator {

	class device_averager;
	class device_hvcontroller;

	typedef boost::variant< device_averager, device_hvcontroller> device_facade_type;
    typedef boost::shared_ptr< device_facade_type > device_facade_ptr;

    class DeviceFacade;

    namespace singleton {
        typedef ACE_Singleton<DeviceFacade, ACE_Recursive_Thread_Mutex> device_facade;
    }

    ////////////////////////////////////////

	class DeviceFacade : public QObject
                       , public ACE_Task<ACE_SYNCH> {
		Q_OBJECT
		~DeviceFacade();
		DeviceFacade( size_t n_threads = 1 );
	public:
        ACE_HANDLE get_dgram_handle() const;
        ACE_HANDLE get_mcast_handle() const;
        // ACE_Task
        virtual int handle_input( ACE_HANDLE );
        virtual int handle_timeout( const ACE_Time_Value& tv, const void * );
        virtual int svc();
        void close();

		// attach/detach device, return true if supported otherwise false
        bool attach_device( device_facade_ptr& );
        bool detach_device( device_facade_type& );
		bool initialize_dgram();
		bool initialize_mcast();
        bool cancel_timer();
        bool close_dgram();
		bool close_mcast();

        const ACE_INET_Addr& get_local_addr() const;

        bool handle_data( ACE_InputCDR& cdr );

		bool lifeCycleUpdate( adportable::protocol::LifeCycleCommand );
        bool heartBeatToController( unsigned long value );

		const adportable::protocol::LifeCycle& lifeCycle() const { return lifeCycle_; }
		adportable::protocol::LifeCycle& lifeCycle() { return lifeCycle_; }
		const ACE_INET_Addr& get_remote_addr() const;
		void set_remote_addr( const ACE_INET_Addr& );

	protected:
		bool notify_dgram( ACE_Message_Block * );

	private:
        friend singleton::device_facade;
        ACE_Barrier barrier_;
        size_t n_threads_;
        boost::shared_ptr< acewrapper::DgramHandler > dgram_handler_;
        boost::shared_ptr< acewrapper::McastHandler > mcast_handler_;

		DeviceFacadeImpl * pImpl_;
		adportable::protocol::LifeCycle lifeCycle_;
        bool handleIt( ACE_Message_Block * );

        void handle_lifecycle_mcast( const ACE_INET_Addr&, const adportable::protocol::LifeCycleFrame&, const adportable::protocol::LifeCycleData& );

        adportable::protocol::LifeCycleCommand handle_lifecycle_dgram( const ACE_INET_Addr&
            , const adportable::protocol::LifeCycleFrame&, const adportable::protocol::LifeCycleData& );

    signals:
		void signal_device_attached( std::string device );
		void signal_device_detached( std::string device );
		void signal_dgram( ACE_Message_Block * );
		void signal_debug( QString msg );
	};

}