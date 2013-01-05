/**************************************************************************
** Copyright (C) 2010-2013 Toshinobu Hondo, Ph.D.
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

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <utils/fancymainwindow.h>
#include "constants.hpp"

namespace adportable { class Configuration; }
namespace adcontrols { class datafile; class ProcessMethod; }
namespace portfolio { class Folium; }
namespace Core { class IMode; }
class QHBoxLayout;
class QWidget;
class QToolButton;
class QAction;

namespace dataproc {

    class Dataprocessor;

    class MainWindow : public Utils::FancyMainWindow {
        Q_OBJECT
    public:
        ~MainWindow();
        explicit MainWindow(QWidget *parent = 0);

		QWidget * createContents( Core::IMode *, const adportable::Configuration&, const std::wstring& apppath );
		void createActions();

		void activateLayout();
		void setSimpleDockWidgetArrangement();

		static QToolButton * toolButton( const char * );
		static QToolButton * toolButton( QAction * );

        void getProcessMethod( adcontrols::ProcessMethod& );
        void OnInitialUpdate();
        void OnFinalClose();
        
    signals:
        void signalUpdateFile( adcontrols::datafile * );
        void signalGetProcessMethod( adcontrols::ProcessMethod& );
            
    public slots:
        void handleSessionAdded( Dataprocessor * );
        void handleSelectionChanged( Dataprocessor *, portfolio::Folium& );
        void onMethodApply( adcontrols::ProcessMethod& );
        void actionApply();                                                        

    private slots:
        void handleApplyMethod();
        void handleFeatureSelected( int );
        void handleFeatureActivated( int );

    private:
		QWidget * toolBar_;
		QHBoxLayout * toolBarLayout_;
		QDockWidget * toolBarDockWidget_;

		QAction * actionSearch_;
        QAction * actionApply_;
        enum ProcessType currentFeature_;

		void setToolBarDockWidget( QDockWidget * dock );
		QDockWidget * toolBarDockWidget() { return toolBarDockWidget_; }
		void createToolbar();
		QDockWidget * createDockWidget( QWidget *, const QString& title = QString() );
		void createDockWidgets( const adportable::Configuration&, const std::wstring& apppath );
    };

}
    
#endif // MAINWINDOW_HPP