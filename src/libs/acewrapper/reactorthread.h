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
//////////////////////////////////////////////
// Copyright (C) 2010 Toshinobu Hondo, Ph.D.
// Science Liaison Project
//////////////////////////////////////////////

#ifndef REACTORTHREAD_H
#define REACTORTHREAD_H

class ACE_Semaphore;
class ACE_Reactor;
//template<class T, class X> class ACE_Singleton;

namespace acewrapper {

    class ReactorThread {
	public:
        ~ReactorThread();
        ReactorThread();

        ACE_Reactor * get_reactor();
        void terminate();
        bool spawn();

        static void spawn( ReactorThread * );

    private:
        static void * thread_entry( void * me );
        void run_event_loop();
        ACE_Reactor * reactor_;
		ACE_Semaphore * sema_;
    };
}

#endif // REACTORTHREAD_H
