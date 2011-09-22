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

#include "logging.hpp"
#include "manager_i.hpp"
#include "constants.hpp"
#include "task.hpp"
#include <adportable/debug.hpp>

using namespace adcontroller;

Logging::Logging( const std::wstring& format
                , ::EventLog::eMSGPRIORITY pri
                , const std::wstring& srcId
                , const std::wstring& msgId ) : msg( format, pri, srcId, msgId )
{
    adportable::debug() << format;
}

Logging::~Logging()
{
    commit_to_broker();
    commit_to_task();
    adportable::debug() << adinterface::EventLog::LogMessageHelper::toString( msg.get() );
}

void
Logging::commit_to_broker()
{
    const EventLog::LogMessage& elog = msg.get();
    if ( elog.format.in() && *elog.format.in() != 0 ) {
        Broker::Logger_var logger = static_cast< manager_i *>(*manager_i::instance())->getLogger();
        if ( ! CORBA::is_nil( logger ) ) {
            Broker::LogMessage blog;
            blog.logId = elog.logId;
            blog.priority = elog.priority;
            blog.tv_sec = elog.tv.sec;
            blog.tv_usec = elog.tv.usec;
            blog.msgId = CORBA::wstring_dup( elog.msgId );
            blog.srcId = CORBA::wstring_dup( elog.srcId );
            blog.text = CORBA::wstring_dup( elog.format );
            blog.args.length( elog.args.length() );
            for ( size_t i = 0; i < elog.args.length(); ++i )
                blog.args[i] = CORBA::wstring_dup( elog.args[i] );
            logger->log( blog );
        }
    }
}

void
Logging::commit_to_task()
{
    // Broker::EventLog, that is not EventLog
    if ( msg.get().format.in() && *msg.get().format.in() != 0 ) {
        TAO_OutputCDR cdr;
        cdr << msg.get();
        ACE_Message_Block * mb = cdr.begin()->duplicate();
        mb->msg_type( constants::MB_EVENTLOG );
        iTask::instance()->putq( mb );
    }
}
