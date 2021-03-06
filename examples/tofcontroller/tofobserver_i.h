// -*- C++ -*-
/**************************************************************************
** Copyright (C) 2010-2011 Toshinobu Hondo, Ph.D.
** Copyright (C) MS-Cheminformatics LLC / Advanced Instrumentation Project
*
** Contact: toshi.hondo@scienceliaison.com
**
** Commercial Usage
**
** Licensees holding valid MS-Cheminformatics commercial licenses may use this
** file in accordance with the MS-Cheminformatics Commercial License Agreement
** provided with the Software or, alternatively, in accordance with the terms
** contained in a written agreement between you and MS-Cheminformatics.
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

#pragma once

#pragma warning (disable : 4996 )
# include "tofcontrollerS.h"
# include <adinterface/signalobserverS.h>
#pragma warning (default : 4996 )

#include <vector>
#include <deque>

namespace tofcontroller {

    class TOFTask;

	class tofObserver_i : public virtual POA_SignalObserver::Observer {
	public:
		tofObserver_i( TOFTask& );
		~tofObserver_i(void);

		virtual ::SignalObserver::Description * getDescription (void);
		virtual ::CORBA::Boolean setDescription ( const ::SignalObserver::Description & desc );
		virtual ::CORBA::ULong objId();
		virtual void assign_objId( CORBA::ULong oid );
		virtual ::CORBA::Boolean connect( ::SignalObserver::ObserverEvents_ptr cb
			                             , ::SignalObserver::eUpdateFrequency frequency
										 , const CORBA::WChar * );
        virtual ::CORBA::Boolean disconnect( ::SignalObserver::ObserverEvents_ptr cb );
		virtual ::CORBA::Boolean isActive (void);
		virtual ::SignalObserver::Observers * getSiblings (void);
		virtual ::CORBA::Boolean addSibling ( ::SignalObserver::Observer_ptr observer);
        virtual ::SignalObserver::Observer * findObserver( CORBA::ULong objId, CORBA::Boolean recursive );
		virtual void uptime ( ::CORBA::ULongLong_out usec );
		virtual ::CORBA::Boolean readData ( ::CORBA::Long pos, ::SignalObserver::DataReadBuffer_out dataReadBuffer);
		virtual ::CORBA::WChar * dataInterpreterClsid (void);
        virtual ::CORBA::Long posFromTime( CORBA::ULongLong usec );

        // internal
        void push_profile_data( ACE_Message_Block * mb );

	private:

        ACE_Recursive_Thread_Mutex mutex_;        
		TOFTask & task_;
        unsigned long objId_;
		SignalObserver::Description desc_;
        typedef std::vector< ::SignalObserver::Observer_var > sibling_vector_type;
        sibling_vector_type siblings_;

        struct cache_item {
            ~cache_item();
            cache_item( long pos, ACE_Message_Block * mb, unsigned long event );
            cache_item( const cache_item & );
            operator long () const;
            long pos_;
            unsigned long wellKnownEvents_;
            ACE_Message_Block * mb_;
        };
        std::deque< cache_item > fifo_;
	};

}
