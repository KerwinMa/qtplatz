// This is a -*- C++ -*- header.
//////////////////////////////////////////
// Copyright (C) 2010 Toshinobu Hondo, Ph.D.
// Science Liaison / Advanced Instrumentation Project
//////////////////////////////////////////

#pragma once

#include <adinterface/ReceiverS.h>
#include <adinterface/instrumentC.h>
#include <adportable/configuration.h>
#include <string>
#include <boost/noncopyable.hpp>

namespace ControlMethod {
    struct Methohd;
}

namespace SampleBroker {
	struct SampleSequenceLine;
}

namespace adcontroller {

    class iBroker;

    class iProxy : public POA_Receiver, boost::noncopyable {
    public:
        iProxy( iBroker& );

        void setConfiguration( const adportable::Configuration& );

        // POA_Receiver
		void message( ::Receiver::eINSTEVENT msg, CORBA::ULong value );
        void log( const EventLog::LogMessage& );
        void shutdown();
        void debug_print( CORBA::Long pri, CORBA::Long cat, const char * text );

        // iProxy
        void reset_clock();
        bool connect( const std::wstring& token );
        bool initialize();
		bool request_shutdown();
        bool eventOut( unsigned long event );

        bool prepare_for_run( const SampleBroker::SampleSequenceLine&
                              , const ControlMethod::Method& );
        bool startRun();   // method start
        bool suspendRun(); // method suspend, will hold before next sample load
        bool resumeRun();  // method restart
        bool stopRun();    // method(sequence) stop
       
        unsigned long getStatus();

    private:
        bool objref_;
        Instrument::Session_var impl_;
        iBroker& broker_;
        adportable::Configuration config_;
        std::wstring name_;
    };
    
}


