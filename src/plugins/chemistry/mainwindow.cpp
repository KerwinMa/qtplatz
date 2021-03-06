/**************************************************************************
** Copyright (C) 2010-2013 Toshinobu Hondo, Ph.D.
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

#include "mainwindow.hpp"
#include "moltabledelegate.hpp"
#include "moltableview.hpp"
#include <adchem/sdfile.hpp>
#include "massdefectform.hpp"
#include "chemistryconstants.hpp"
#include <adportable/profile.hpp>
#include <qtwrapper/trackingenabled.hpp>

#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/imode.h>
#include <coreplugin/editormanager/editormanager.h>
#include <coreplugin/findplaceholder.h>
#include <coreplugin/rightpane.h>
#include <coreplugin/minisplitter.h>
#include <coreplugin/outputpane.h>
#include <coreplugin/navigationwidget.h>
#include <coreplugin/coreconstants.h>
#include <coreplugin/icore.h>
#include <utils/styledbar.h>

#include <QDebug>
#include <QDockWidget>
#include <QFileDialog>
#include <QLineEdit>
#include <QMenu>
#include <QResizeEvent>
#include <qstackedwidget.h>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QLabel>
#include <QtGui/QIcon>
#include <QStandardItemModel>
#include <QProgressBar>

#include <boost/filesystem/path.hpp>
#include <boost/bind.hpp>
#include <algorithm>

using namespace chemistry;

MainWindow * MainWindow::instance_ = 0;

MainWindow::~MainWindow()
{
}

MainWindow::MainWindow( QWidget * parent ) : Utils::FancyMainWindow( parent )
                                           , tableView_( new MolTableView( this ) )
                                           , toolBar_( 0 )
                                           , toolBarLayout_( 0 )
                                           , toolBarDockWidget_( 0 )
                                           , actionSearch_( 0 )
                                           , topLineEdit_( 0 )
                                           , progressBar_( 0 )
{
	instance_ = this;
}


void
MainWindow::OnInitialUpdate()
{
	setSimpleDockWidgetArrangement();
}

void
MainWindow::activateLayout()
{
}

void
MainWindow::createActions()
{
	actionSearch_ = new QAction( QIcon( ":/chemistry/images/search.png" ), tr("Search"), this );
    connect( actionSearch_, SIGNAL( triggered() ), this, SLOT( actionSearch_ ) );
}

QWidget *
MainWindow::createContents( Core::IMode * mode )
{
    setTabPosition( Qt::AllDockWidgetAreas, QTabWidget::East );
    setDocumentMode( true );
    setDockNestingEnabled( true );

    QBoxLayout * editorHolderLayout = new QVBoxLayout;
	editorHolderLayout->setMargin( 0 );
	editorHolderLayout->setSpacing( 0 );

    if ( tableView_ ) {
		tableView_->setContextMenuPolicy( Qt::CustomContextMenu );
        connect( tableView_.get(), SIGNAL( dropped( const QList<QUrl>& ) ), this, SLOT( handleDropped( const QList<QUrl>& ) ) );
    }

    QWidget * editorAndFindWidget = new QWidget;
    if ( editorAndFindWidget ) {
        editorAndFindWidget->setLayout( editorHolderLayout );
        editorAndFindWidget->setLayout( editorHolderLayout );

        editorHolderLayout->addWidget( tableView_.get() );
    }

    Core::MiniSplitter * documentAndRightPane = new Core::MiniSplitter;
    if ( documentAndRightPane ) {
        documentAndRightPane->addWidget( editorAndFindWidget );
        documentAndRightPane->addWidget( new Core::RightPanePlaceHolder( mode ) );
        documentAndRightPane->setStretchFactor( 0, 1 );
        documentAndRightPane->setStretchFactor( 1, 0 );
    }

    Utils::StyledBar * toolBar1 = createTopStyledBar();
    Utils::StyledBar * toolBar2 = createMidStyledBar();

	if ( QDockWidget * dock = new QDockWidget( "Chemistry Toolbar" ) ) {
        dock->setObjectName( QLatin1String( "Chemistry Toolbar" ) );
        // dock->setWidget( toolBar );
        dock->setFeatures( QDockWidget::NoDockWidgetFeatures );
        dock->setAllowedAreas( Qt::BottomDockWidgetArea );
        dock->setTitleBarWidget( new QWidget( dock ) );
        dock->setProperty( "managed_dockwidget", QLatin1String( "true" ) );
        addDockWidget( Qt::BottomDockWidgetArea, dock );
        setToolBarDockWidget( dock );
    }

	//---------- centraol widget ------------
	QWidget * centralWidget = new QWidget;
	setCentralWidget( centralWidget );

	QVBoxLayout * centralLayout = new QVBoxLayout( centralWidget );
	centralLayout->setMargin( 0 );
	centralLayout->setSpacing( 0 );
    centralLayout->addWidget( toolBar1 );
    centralLayout->addWidget( documentAndRightPane );
	centralLayout->addWidget( toolBar2 );

	// Right-side window with editor, output etc.
	Core::MiniSplitter * mainWindowSplitter = new Core::MiniSplitter;
    QWidget * outputPane = new Core::OutputPanePlaceHolder( mode, mainWindowSplitter );
    outputPane->setObjectName( QLatin1String( "ChemistryOutputPanePlaceHolder" ) );
	mainWindowSplitter->addWidget( this );
    mainWindowSplitter->addWidget( outputPane );
	mainWindowSplitter->setStretchFactor( 0, 10 );
	mainWindowSplitter->setStretchFactor( 1, 0 );
	mainWindowSplitter->setOrientation( Qt::Vertical );

	// Navigation and right-side window
	Core::MiniSplitter * splitter = new Core::MiniSplitter;
	splitter->addWidget( new Core::NavigationWidgetPlaceHolder( mode ) );
    splitter->addWidget( mainWindowSplitter );
    splitter->setStretchFactor( 0, 0 );
    splitter->setStretchFactor( 1, 1 );
    splitter->setObjectName( QLatin1String( "ChemistryModeWidget" ) );

	createDockWidgets();

	return splitter;
}

void
MainWindow::setSimpleDockWidgetArrangement()
{
	qtwrapper::TrackingEnabled<MainWindow> x( *this );

	QList< QDockWidget *> dockWidgets = this->dockWidgets();
    foreach ( QDockWidget * dockWidget, dockWidgets ) {
		dockWidget->setFloating( false );
		removeDockWidget( dockWidget );
	}
  
    size_t npos = 0;
    foreach ( QDockWidget * dockWidget, dockWidgets ) {
		addDockWidget( Qt::BottomDockWidgetArea, dockWidget );
		dockWidget->show();
        if ( npos++ > 1 )
			tabifyDockWidget( dockWidgets[1], dockWidget );
	}


	QDockWidget * toolBarDock = toolBarDockWidget();

	toolBarDock->show();
	update();
}

void
MainWindow::setToolBarDockWidget( QDockWidget * dock )
{
	toolBarDockWidget_ = dock;
}

QDockWidget *
MainWindow::createDockWidget( QWidget * widget, const QString& title )
{
	QDockWidget * dockWidget = addDockForWidget( widget );
	dockWidget->setObjectName( widget->objectName() );
    if ( title.isEmpty() )
		dockWidget->setWindowTitle( widget->objectName() );
	else
		dockWidget->setWindowTitle( title );

	addDockWidget( Qt::BottomDockWidgetArea, dockWidget );
#if 0
	QAction * toggleViewAction = dockWidget->toggleViewAction();
	QList<int> globalContext;
	globalContext << Core::Constants::C_GLOBAL_ID;
	// Core::Command * cmd = Core::ActionManager::registerAction( toggleViewAction, "Chemistry." + widget->objectName(), globalContext );
#endif

	return dockWidget;
}

void
MainWindow::createDockWidgets()
{
    MassDefectForm * form = new MassDefectForm;
	form->setObjectName( "massdefect" );
    form->OnInitialUpdate();
	createDockWidget( form );
}

void
MainWindow::createToolbar()
{
	QWidget * toolbarContainer = new QWidget;
	QHBoxLayout * hbox = new QHBoxLayout( toolbarContainer );
    hbox->setMargin( 0 );
    hbox->setSpacing( 0 );
    hbox->addWidget( toolButton( "STOP" ) ); // should create action in 'plugin' with icon
}

// static
QToolButton * 
MainWindow::toolButton( QAction * action )
{
	QToolButton * button = new QToolButton;
	if ( button )
		button->setDefaultAction( action );
	return button;
}

// static
QToolButton * 
MainWindow::toolButton( const char * id )
{
	Core::ActionManager * mgr = Core::ICore::instance()->actionManager();
	return toolButton( mgr->command(id)->action() );
}

// slot
void
MainWindow::actionSearch()
{
}

MainWindow *
MainWindow::instance()
{
	return instance_;
}

void
MainWindow::handleDropped( const QList< QUrl >& urls )
{
    for ( auto& url: urls ) {
        boost::filesystem::path path( url.toLocalFile().toStdWString() );
        topLineEdit_->setText( QString::fromStdWString( path.wstring() ) );
        adchem::SDFile file( path.string() );
        tableView_->setMol( file, *progressBar_ );
    }
}

Utils::StyledBar *
MainWindow::createMidStyledBar()
{
    Utils::StyledBar * toolBar = new Utils::StyledBar;
    if ( toolBar ) {
        toolBar->setProperty( "topBorder", true );
        QHBoxLayout * toolBarLayout = new QHBoxLayout( toolBar );
        toolBarLayout->setMargin( 0 );
        toolBarLayout->setSpacing( 0 );
        toolBarLayout->addWidget( toolButton( actionSearch_ ) );
        toolBarLayout->addWidget( new QLabel( tr("Alchemy") ) );
        toolBarLayout->addWidget( new QLabel( tr("Chemistry") ) );
        toolBarLayout->addWidget( new QLabel( tr("Physics") ) );
    }
    return toolBar;
}

Utils::StyledBar *
MainWindow::createTopStyledBar()
{
    Utils::StyledBar * toolBar = new Utils::StyledBar;
    if ( toolBar ) {
        toolBar->setProperty( "topBorder", true );
        QHBoxLayout * toolBarLayout = new QHBoxLayout( toolBar );
        toolBarLayout->setMargin( 0 );
        toolBarLayout->setSpacing( 0 );

        toolBarLayout->addWidget( new QLabel( tr(">> Drop SD File on table: " ) ) );
        topLineEdit_ = new QLineEdit;
        toolBarLayout->addWidget( topLineEdit_ );

        Core::ActionManager * am = Core::ICore::instance()->actionManager();
        if ( am ) {
            QList<int> globalcontext;
            globalcontext << Core::Constants::C_GLOBAL_ID;
            toolBarLayout->addWidget(toolButton(am->command(Constants::SDFILE_OPEN)->action()));
        }
        
        toolBarLayout->addWidget( new Utils::StyledSeparator );
        toolBarLayout->addItem( new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum) );
        progressBar_ = new QProgressBar;
		progressBar_->setVisible( false );
        toolBarLayout->addWidget( progressBar_ );
        progressBar_->setStyleSheet( QString("QProgressBar { color: lightgreen}") );
    }
    return toolBar;
}

void
MainWindow::actSDFileOpen()
{
    boost::filesystem::path datapath( adportable::profile::user_data_dir<char>() );
    datapath /= "data";

    QString name
        = QFileDialog::getOpenFileName( this
                                        , tr("Open SDFile" )
										, datapath.string().c_str()
                                        , tr("Structure Data files(*.sdf)") );
    if ( ! name.isEmpty() ) {

        topLineEdit_->setText( name );
        adchem::SDFile file( name.toStdString() );
        tableView_->setMol( file, *progressBar_ );

	}
}

