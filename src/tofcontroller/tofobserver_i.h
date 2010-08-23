// This is a -*- C++ -*- header.
//////////////////////////////////////////
// Copyright (C) 2010 Toshinobu Hondo, Ph.D.
// Science Liaison / Advanced Instrumentation Project
//////////////////////////////////////////

#pragma once

#pragma warning (disable : 4996 )
# include "tofcontrollerS.h"
# include <adinterface/signalobserverS.h>
#pragma warning (default : 4996 )

//#include <acewrapper/orbservant.h>
//#include <boost/smart_ptr.hpp>

namespace tofcontroller {

	class tofObserver_i : public virtual POA_SignalObserver::Observer {
	public:
		tofObserver_i(void);
		~tofObserver_i(void);

		virtual ::SignalObserver::Description * getDescription (void);
		virtual ::CORBA::Boolean setDescription ( const ::SignalObserver::Description & desc );
		virtual ::CORBA::Boolean connect ( ::SignalObserver::ObserverEvents_ptr cb,	::SignalObserver::eUpdateFrequency frequency);
		virtual ::CORBA::Boolean isActive (void);
		virtual ::SignalObserver::Observers * getSiblings (void);
		virtual ::CORBA::Boolean addSibling ( ::SignalObserver::Observer_ptr observer);
		virtual void uptime ( ::CORBA::ULongLong_out usec );
		virtual ::CORBA::Boolean readData ( ::CORBA::Long pos, ::SignalObserver::DataReadBuffer_out dataReadBuffer);
		virtual ::CORBA::WChar * dataInterpreterClsid (void);
	};

}
