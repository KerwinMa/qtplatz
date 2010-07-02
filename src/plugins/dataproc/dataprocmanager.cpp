//////////////////////////////////////////
// Copyright (C) 2010 Toshinobu Hondo, Ph.D.
// Science Liaison / Advanced Instrumentation Project
//////////////////////////////////////////

#include "dataprocmanager.h"
#include <boost/noncopyable.hpp>

#include <utils/fancymainwindow.h>
#include <utils/styledbar.h>
#include <QtCore/QHash>
#include <QString>
#include <QDockWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QToolBar>
#include <QTextEdit>

#include <vector>

using namespace dataproc::internal;

namespace dataproc {
  namespace internal {
    class DataprocManagerImpl : boost::noncopyable {
    public:
      ~DataprocManagerImpl();
      DataprocManagerImpl();

      Utils::FancyMainWindow * mainWindow_;
      void init();

    public:
      std::vector< QDockWidget * > dockWidgetVec_;

    };
  }
}

DataprocManager::DataprocManager(QObject *parent) :
    QObject(parent)
    , pImpl_( new DataprocManagerImpl() )
{
}

QMainWindow *
DataprocManager::mainWindow() const
{
    return pImpl_->mainWindow_;
}

void
DataprocManager::init()
{
  pImpl_->init();

  QWidget * edit1 = new QTextEdit( "Sequence" );
  edit1->setWindowTitle( tr("Centroid") );
  
  QWidget * edit2 = new QTextEdit( "Log" );
  edit2->setWindowTitle( tr("Elemental Composition") );
  
  QWidget * edit3 = new QTextEdit( "MS" );
  edit3->setWindowTitle( tr("Lockmass") );
  
  QWidget * edit4 = new QTextEdit( "Edit4" );
  edit4->setWindowTitle( tr("Isotope") );
  
  QWidget * edit5 = new QTextEdit( "Edit 5" );
  edit5->setWindowTitle( tr("MS Calibration") );
  
  QWidget * edit6 = new QTextEdit( "Edit 6" );
  edit6->setWindowTitle( tr("Targeting") );
  
  QWidget * edit7 = new QTextEdit( "Edit 6" );
  edit7->setWindowTitle( tr("Chromatogram") );
  
  QWidget * edit8 = new QTextEdit( "Edit 6" );
  edit8->setWindowTitle( tr("Report") );

  DataprocManagerImpl& m = *pImpl_;

    QDockWidget * dock1 = m.mainWindow_->addDockForWidget( edit1 );
    QDockWidget * dock2 = m.mainWindow_->addDockForWidget( edit2 );
    QDockWidget * dock3 = m.mainWindow_->addDockForWidget( edit3 );
    QDockWidget * dock4 = m.mainWindow_->addDockForWidget( edit4 );
    QDockWidget * dock5 = m.mainWindow_->addDockForWidget( edit5 );
    QDockWidget * dock6 = m.mainWindow_->addDockForWidget( edit6 );
    QDockWidget * dock7 = m.mainWindow_->addDockForWidget( edit7 );
    QDockWidget * dock8 = m.mainWindow_->addDockForWidget( edit8 );
    
    m.dockWidgetVec_.push_back( dock1 );
    m.dockWidgetVec_.push_back( dock2 );
    m.dockWidgetVec_.push_back( dock3 );
    m.dockWidgetVec_.push_back( dock4 );
    m.dockWidgetVec_.push_back( dock5 );
    m.dockWidgetVec_.push_back( dock6 );
    m.dockWidgetVec_.push_back( dock7 );
    m.dockWidgetVec_.push_back( dock8 );

}

///////////////////////
DataprocManagerImpl::~DataprocManagerImpl()
{
}

DataprocManagerImpl::DataprocManagerImpl() : mainWindow_(0)
{
}

void
DataprocManagerImpl::init()
{
  mainWindow_ = new Utils::FancyMainWindow;
  if ( mainWindow_ ) {
    mainWindow_->setTabPosition( Qt::AllDockWidgetAreas, QTabWidget::North );
    mainWindow_->setDocumentMode( true );
    
  }
}

void
DataprocManager::setSimpleDockWidgetArrangement()
{
  class setTrackingEnabled {
    Utils::FancyMainWindow& w_;
  public:
    setTrackingEnabled( Utils::FancyMainWindow& w ) : w_(w) { w_.setTrackingEnabled( false ); }
    ~setTrackingEnabled() {  w_.setTrackingEnabled( true ); }
  };

  DataprocManagerImpl& m = *pImpl_;
  setTrackingEnabled lock( *m.mainWindow_ );
  
  QList< QDockWidget *> dockWidgets = m.mainWindow_->dockWidgets();
  
  foreach ( QDockWidget * dockWidget, dockWidgets ) {
    dockWidget->setFloating( false );
    m.mainWindow_->removeDockWidget( dockWidget );
  }

  foreach ( QDockWidget * dockWidget, dockWidgets ) {
    m.mainWindow_->addDockWidget( Qt::BottomDockWidgetArea, dockWidget );
    dockWidget->show();
  }

  for ( unsigned int i = 1; i < m.dockWidgetVec_.size(); ++i )
    m.mainWindow_->tabifyDockWidget( m.dockWidgetVec_[0], m.dockWidgetVec_[i] );
}