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
//////////////////////////////////////////
// Copyright (C) 2010 Toshinobu Hondo, Ph.D.
// Science Liaison / Advanced Instrumentation Project
//////////////////////////////////////////
#ifndef TIMERHANDLER_H
#define TIMERHANDLER_H

#pragma warning (disable: 4996)
# include <ace/Event_Handler.h>
#pragma warning (default: 4996)

class ACE_Semaphore;

namespace acewrapper {

    class TimerHandler {
    public:
        ~TimerHandler();
        TimerHandler();
        ACE_HANDLE get_handle() const { return 0; /* meanless for timer */ }
        void cancel( ACE_Reactor*, ACE_Event_Handler *);
        int signal();
        int wait();
    private:
        ACE_Semaphore * sema_;
   };

   ////////////////
   template<class T> class TimerReceiver : public T
					                     , public TimerHandler {
   public:
	 TimerReceiver() {}
	 int handle_timeout( const ACE_Time_Value& tv, const void * arg) { return T::handle_timeout(tv, arg); }
     int handle_close( ACE_HANDLE h, ACE_Reactor_Mask mask ) { signal(); return T::handle_close( h, mask ); }
  };

}

#endif // TIMERHANDLER_H
