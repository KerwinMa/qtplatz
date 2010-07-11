//////////////////////////////////////////////
// Copyright (C) 2010 Toshinobu Hondo, Ph.D.
// Science Liaison Project
//////////////////////////////////////////////

#include "reactorthread.h"
#include <ace/Thread_Manager.h>

using namespace acewrapper;

ReactorThread::~ReactorThread()
{
}

ReactorThread::ReactorThread()
{
}

// satic
void
ReactorThread::spawn( ReactorThread * pThis )
{
    ACE_Thread_Manager * mgr = ACE_Thread_Manager::instance();
    mgr->spawn(ACE_THR_FUNC( thread_entry ), reinterpret_cast<void *>( pThis ) );
}

void *
ReactorThread::thread_entry( void * me )
{
    ReactorThread * pThis = reinterpret_cast<ReactorThread *>(me);
    if ( pThis ) {
        pThis->reactor_.owner( ACE_OS::thr_self() );
        pThis->run_event_loop();
    }
    return 0;
}

void
ReactorThread::run_event_loop()
{
    while ( reactor_.reactor_event_loop_done() == 0 )
        reactor_.run_reactor_event_loop();
}


ACE_Reactor *
ReactorThread::get_reactor()
{ 
    return &reactor_;
}