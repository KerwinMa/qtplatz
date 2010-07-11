// This is a -*- C++ -*- header.
//////////////////////////////////////////
// Copyright (C) 2010 Toshinobu Hondo, Ph.D.
// Science Liaison / Advanced Instrumentation Project
//////////////////////////////////////////
#ifndef TIMERHANDLER_H
#define TIMERHANDLER_H

#include <ace/Event_Handler.h>

namespace acewrapper {

   class TimerHandler {
      public:
	 TimerHandler();
	 ACE_HANDLE get_handle() const;
   };

   ////////////////
   template<class T> class TimerReceiver : public T
					 , public TimerHandler {
      public:
	 TimerReceiver() {}
	 //int handle_input( ACE_HANDLE h ) { return T::handle_input(*this, h); }
	 int handle_timeout( const ACE_Time_Value& tv, const void * arg) { return T::handle_timeout(tv, arg); }
	 //int handle_close( ACE_HANDLE h, ACE_Reactor_Mask mask ) { return T::handle_close( h, mask ); }
  };

}

#endif // TIMERHANDLER_H