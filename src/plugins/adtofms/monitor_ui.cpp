//////////////////////////////////////////
// Copyright (C) 2010 Toshinobu Hondo, Ph.D.
// Science Liaison / Advanced Instrumentation Project
//////////////////////////////////////////

#include "monitor_ui.h"
#include "ui_form.h"
#include "treemodel.h"
#include "../../../tofcontroller/tofcontrollerC.h"
#include <adportable/configuration.h>
#include <adplugin/orbmanager.h>
#include <acewrapper/nameservice.h>
#include <adinterface/receiverS.h>
#include <QtCore/qplugin.h>
#include <adinterface/eventlog_helper.h>


using namespace adtofms;

namespace adtofms {
    namespace impl {
        
        class TOF : public POA_Receiver {
            monitor_ui& parent_;
            TreeModel * treeModel_;
        public:
            ~TOF();
            TOF( monitor_ui& t );
            TOFInstrument::TofSession_var tof;

            // POA_Receiver
            void message( Receiver::eINSTEVENT msg, CORBA::ULong value );
            void log( const EventLog::LogMessage& );
            void shutdown();
            void debug_print( CORBA::Long pri, CORBA::Long cat, const char * text );
        };

    }
}

monitor_ui::monitor_ui(QWidget *parent) : IMonitor(parent)
                                        , ui(new Ui::Form)
                                        , pTof_(0)
{
    ui->setupUi(this);
    pTof_ = new impl::TOF(*this);
}

monitor_ui::~monitor_ui()
{
    delete ui;
}

void
monitor_ui::OnCreate( const adportable::Configuration& c )
{
    config_ = c;
}

void
monitor_ui::OnInitialUpdate()
{
    // now, it is safe to access CORBA servant
    if ( adplugin::ORBManager::instance()->init( 0, 0 ) >= 0 ) {

        CORBA::Object_var obj = adplugin::ORBManager::instance()->getObject( L"tofcontroller.manager" );

        if ( ! CORBA::is_nil( obj.in() )  ) {

            pTof_->tof = TOFInstrument::TofSession::_narrow( obj );

            if ( ! CORBA::is_nil( pTof_->tof.in() ) ) {
                CORBA::WString_var version_ = pTof_->tof->tof_software_revision();
                pTof_->tof->connect( pTof_->_this(), L"adtofms.monitor_ui" );
            }

        }

    }
}

void
monitor_ui::OnUpdate( boost::any& )
{
}

void
monitor_ui::OnUpdate( unsigned long lHint )
{
    (void)lHint;
}

void
monitor_ui::OnFinalClose()
{
}

// Q_EXPORT_PLUGIN( monitor_ui )

/////////////////////////////////

using namespace adtofms::impl;

TOF::~TOF()
{
    delete treeModel_;
}

TOF::TOF( monitor_ui& t ) : parent_(t)
                          , treeModel_(0)
{
    treeModel_ = new TreeModel();
    parent_.ui->treeView->setModel( treeModel_ );

    for ( int column = 0; column < treeModel_->columnCount(); ++column )
        parent_.ui->treeView->resizeColumnToContents( column );

    //------ test --------
    TreeModel& model = *treeModel_;
    // int row = model.findParent( remote_addr.c_str() );
    // if ( row >= 0 ) {
    int row = 0;
        QModelIndex index = model.index( row, 0 );
        int childRow = model.rowCount( index );
        model.insertRow( childRow, index );
        model.setData( model.index( childRow, 0, index ), "0.0.0.0" );
        model.setData( model.index( childRow, 2, index ), "description" );
        // }
}

void
TOF::message( Receiver::eINSTEVENT msg, CORBA::ULong value )
{
}

void
TOF::log( const EventLog::LogMessage& log )
{
    std::wstring text = adinterface::EventLog::LogMessageHelper::toString( log );
    if ( parent_.ui ) {
        // parent_.ui->treeView-
    }
}

void
TOF::shutdown()
{
}

void
TOF::debug_print( CORBA::Long pri, CORBA::Long cat, const char * text )
{
}

