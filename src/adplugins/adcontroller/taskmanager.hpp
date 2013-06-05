// This is a -*- C++ -*- header.
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

#include <boost/smart_ptr.hpp>
#include <ace/Singleton.h>
#include <ace/Recursive_Thread_Mutex.h>

class ACE_Reactor;

namespace acewrapper {
    class ReactorThread;
    template<class T> class EventHandler;
    class TimerHandler;
    template<class T> class TimerReceiver;
}

namespace adcontroller {

    class iTask;

    namespace internal {
        class TimeReceiver;
    }
    
    class iTaskManager {
    private:
        ~iTaskManager();
        iTaskManager();
        iTaskManager( const iTaskManager& );  /* not defined */
        
    public:  
        static iTaskManager * instance();

        bool manager_initialize();
        void manager_terminate();
        
        inline ACE_Recursive_Thread_Mutex& mutex() { return mutex_; }
        ACE_Reactor * reactor();
        
        template<class T> T* get();
        inline iTask& task() { return *pTask_; }
        
    private:
        friend class ACE_Singleton<iTaskManager, ACE_Recursive_Thread_Mutex>;
        friend class internal::TimeReceiver;
        int handle_timeout( const ACE_Time_Value&, const void * );
        
        ACE_Recursive_Thread_Mutex mutex_;
        iTask * pTask_;
        acewrapper::ReactorThread * reactor_thread_;    
        acewrapper::EventHandler< acewrapper::TimerReceiver<internal::TimeReceiver> > * timerHandler_;
    };

    template<> iTask * iTaskManager::get<iTask>();
    
}