// This is a -*- C++ -*- header.
//////////////////////////////////////////
// Copyright (C) 2010 Toshinobu Hondo, Ph.D.
// Science Liaison / Advanced Instrumentation Project
//////////////////////////////////////////

#pragma once

namespace tofcontroller {

    namespace constants {

        enum msg_type {
			MB_EVENTLOG = ACE_Message_Block::MB_USER
            , MB_INITIALIZE
            , MB_CONNECT
            , MB_TIME_VALUE
			, MB_COMMAND
            , MB_DEBUG
			, MB_MCAST
			, MB_DGRAM
			, MB_SENDTO_DEVICE
            , MB_QUERY_DEVICE
			, MB_RECVFROM_DEVICE
        };

        enum SESSION_COMMAND {
            SESSION_INITIALIZE
            , SESSION_PREPARE_FOR_RUN 
            , SESSION_START_RUN
            , SESSION_SUSPEND_RUN
            , SESSION_RESUME_RUN
            , SESSION_STOP_RUN  
			, SESSION_SHUTDOWN
            , SESSION_SENDTO_DEVICE  // subsequent DWORD should be class id
			, SESSION_QUERY_DEVICE   // subsequent DWORD should be class id
            , SESSION_REMOTE_TRIGGER
            , SESSION_ARM_TRIGGER
            , SESSION_DISARM_TRIGGER
        };

    }
}