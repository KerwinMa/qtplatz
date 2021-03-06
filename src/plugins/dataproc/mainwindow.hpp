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

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <utils/fancymainwindow.h>
#include "constants.hpp"
#include <memory>

namespace adportable { class Configuration; }
namespace adcontrols { class datafile; class ProcessMethod; class MSAssignedMasses; }
namespace portfolio { class Folium; }
namespace Core { class IMode; }
namespace Utils { class StyledBar; }
class QHBoxLayout;
class QWidget;
class QToolButton;
class QAction;
class QStackedWidget;
class QLineEdit;
class QComboBox;

namespace dataproc {

    class Dataprocessor;

    class MainWindow : public Utils::FancyMainWindow {
        Q_OBJECT
        public:
        ~MainWindow();
        explicit MainWindow(QWidget *parent = 0);

        static MainWindow * instance();
        
        QWidget * createContents( Core::IMode *, const adportable::Configuration&, const std::wstring& apppath );
        // if you are looking for 'create_actions', you should see dataproc::ActionManager
        
        void activateLayout();
        void setSimpleDockWidgetArrangement();
        
        static QToolButton * toolButton( const char * );
        static QToolButton * toolButton( QAction * );
        
        void getProcessMethod( adcontrols::ProcessMethod& );
		void setProcessMethod( const adcontrols::ProcessMethod& );
        void OnInitialUpdate();
        void OnFinalClose();

        void applyCalibration( const adcontrols::MSAssignedMasses& );
        void applyCalibration( const adcontrols::MSAssignedMasses&, portfolio::Folium& );
        
        void processMethodSaved( const QString& );
        void processMethodLoaded( const QString&, const adcontrols::ProcessMethod& );
        int currentProcessView( std::string& ) const;
        void printCurrentView( const QString& ) const;

        void saveDefaultMSCalibrateResult( portfolio::Folium& );

    public slots:
        void handleSessionAdded( Dataprocessor * );
        void handleSelectionChanged( Dataprocessor *, portfolio::Folium& );
        void onMethodApply( adcontrols::ProcessMethod& );
        void actionApply();                                                        

    private slots:
        void handleApplyMethod();
        void handleFeatureSelected( int );
        void handleFeatureActivated( int );

        void actionSelMSProcess();
        void actionSelElementalComp();
        void actionSelMSCalibration();
        void actionSelMSCalibSpectra();
        void actionSelChromatogram();

    private:
        QWidget * toolBar_;
        QHBoxLayout * toolBarLayout_;
        QDockWidget * toolBarDockWidget_;
        QComboBox * axisChoice_;

        QAction * actionSearch_;
        QAction * actionApply_;
        QAction * actionSelMSProcess_;
        QAction * actionSelElementalComp_;
        QAction * actionSelMSCalibration_;
        QAction * actionSelMSCalibSpectra_;
        QAction * actionSelChromatogram_;
        QStackedWidget * stack_;
        std::unique_ptr< QLineEdit > processMethodNameEdit_;

        enum ProcessType currentFeature_;

        void setToolBarDockWidget( QDockWidget * dock );
        QDockWidget * toolBarDockWidget() { return toolBarDockWidget_; }
        void createToolbar();
        QDockWidget * createDockWidget( QWidget *, const QString& title = QString() );
        void createDockWidgets();
        Utils::StyledBar * createStyledBarTop();
        Utils::StyledBar * createStyledBarMiddle();

    signals:
        void onPrintCurrentView( const QString& ) const;
    };

}
    
#endif // MAINWINDOW_HPP
