//////////////////////////////////////////
// Copyright (C) 2010 Toshinobu Hondo, Ph.D.
// Science Liaison / Advanced Instrumentation Project
//////////////////////////////////////////

#include "mcastserver.h"
#include <iostream>
#include <boost/cast.hpp>
#include <ace/Thread_Manager.h>
#include <ace/Reactor.h>
#include <ace/Log_Msg.h>
#include <acewrapper/mutex.hpp>

using namespace acewrapper;

McastServer::~McastServer()
{
   delete handler_;
}

McastServer::McastServer( Callback& cb, ACE_Reactor * r, u_short port ) : handler_(0)
									, reactor_(r)
									, port_(port)
									, callback_(cb)
{
   handler_ = new Handler( ACE_DEFAULT_MULTICAST_PORT
			   , ACE_DEFAULT_MULTICAST_ADDR
			   , *get_reactor()
			   , callback_ );

}

ACE_Reactor *
McastServer::get_reactor()
{
    return reactor_ ? reactor_ : ACE_Reactor::instance();
}

McastServer::Handler::~Handler()
{
    if ( mcast_.leave(sock_addr_) == -1 )
        ACE_ERROR((LM_ERROR, "%p\n", "leave fails"));
}

McastServer::Handler::Handler( u_short udp_port
                               , const char * ip_addr
                               , ACE_Reactor& reactor
                               , Callback& cb ) : callback_(cb)
{
   sock_addr_ = ACE_INET_Addr(udp_port, ip_addr);
   
   if ( mcast_.join( sock_addr_ ) == -1 ) {
      
      ACE_ERROR((LM_ERROR, "%p\n", "can't subscribe to multicast group"));
      
   } else if ( reactor.register_handler(  mcast_.get_handle()
					  , this
					  , ACE_Event_Handler::READ_MASK) == -1 ) {
      
      ACE_ERROR((LM_ERROR, "%p\n", "can't register with Reactor"));
      std::cout << "can't register with reactor" << std::endl;
      
   }
   std::cout << "Mcast address: " << sock_addr_.get_host_addr()
	     << " port number: " << sock_addr_.get_port_number() << std::endl;
   
}

bool
McastServer::send( const char * pbuf, ssize_t size, const ACE_INET_Addr& )
{
    if ( handler_ )
       return handler_->send( pbuf, size );
    return false;
}

bool
McastServer::send( const char * pbuf, ssize_t size )
{
    if ( handler_ )
       return handler_->send( pbuf, size );
    return false;
}

///////////////////////////////////////////

bool
McastServer::Handler::send( const char * pbuf, ssize_t size )
{
    return mcast_.send( pbuf, size ) == size;
}

ACE_HANDLE
McastServer::Handler::get_handle() const
{
    return mcast_.get_handle();
}

int
McastServer::Handler::handle_input(ACE_HANDLE h)
{
   char buf[2048];
   
   memset(buf, 0, sizeof(buf));
   
   if ( h == ACE_STDIN ) {
      assert(0);
   } else {
      ACE_INET_Addr remote_addr;
      
      ssize_t result = mcast_.recv(buf, sizeof(buf), remote_addr);
      if ( result != -1 ) {
	 /*
	   debug_trace(LOG_DEBUG,
	   "received datagram from host %s(%s) on port %d bytes = %d\n",
	   remote_addr.get_host_name(),
	   remote_addr.get_host_addr(),
	   remote_addr.get_port_number(),
	   result);
	 */
	 
	 callback_(buf, result, remote_addr);
	 return 0;
      }
      ACE_ERROR_RETURN((LM_ERROR, "%p\n", "something a miss"), -1);
   }
   return 0;
}

int
McastServer::Handler::handle_close(ACE_HANDLE, ACE_Reactor_Mask)
{
   return 0;
}