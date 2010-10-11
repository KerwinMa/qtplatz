// This is a -*- C++ -*- header.
//////////////////////////////////////////
// Copyright (C) 2010 Toshinobu Hondo, Ph.D.
// Science Liaison / Advanced Instrumentation Project
//////////////////////////////////////////

#pragma once

#pragma warning(disable:4996)
#include <adinterface/signalobserverS.h>
#include <adinterface/instrumentC.h>
#pragma warning(default:4996)

#include <adportable/configuration.h>
#include <boost/noncopyable.hpp>

namespace adcontroller {

    class iBroker;
    class iProxy;

	class oProxy : public POA_SignalObserver::ObserverEvents, boost::noncopyable {
    public:
		~oProxy();
		oProxy( iBroker& );

		// POA_SignalObserver::ObserverEvents implementation
		virtual void OnUpdateData (::CORBA::Long pos);
		virtual void OnMethodChanged (::CORBA::Long pos);
		virtual void OnEvent (::CORBA::ULong event,	::CORBA::Long pos);

		// oProxy implementation
        bool initialize();
		bool connect( const std::wstring& token );
		bool setInstrumentSession( Instrument::Session_ptr p );
		size_t populateObservers( unsigned long objId );
        void setConfiguration( const adportable::Configuration& );
        void objId( unsigned long objid );
		unsigned long objId() const;
		SignalObserver::Observer_ptr getObject();

    private:
		bool objref_;
		unsigned long objId_;
		iBroker& broker_;
		Instrument::Session_var iSession_;
		SignalObserver::Observer_var impl_;
        adportable::Configuration config_;
    };
    
}