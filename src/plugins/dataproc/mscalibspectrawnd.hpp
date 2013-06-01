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

#ifndef MSCALIBSPECTRAWND_HPP
#define MSCALIBSPECTRAWND_HPP

#include <QWidget>
#include <portfolio/folium.hpp>
#include <boost/tuple/tuple.hpp>
#if ! defined Q_MOC_RUN
#include <boost/smart_ptr.hpp>
#endif

class QSplitter;

namespace adcontrols {
    class MassSpectrum;
    class MSCalibrateResult;
    class MSAssignedMasses;
    class ProcessMethod;
}
namespace adportable {  class Configuration; }
namespace adwplot { class SpectrumWidget; }

namespace dataproc {

    class Dataprocessor;

    class MSCalibSpectraWnd : public QWidget {
        Q_OBJECT
    public:
        MSCalibSpectraWnd( const adportable::Configuration&, const std::wstring&, QWidget * parent = 0 );
    public slots:
        void handleSessionAdded( Dataprocessor* );
        void handleSelectionChanged( Dataprocessor*, portfolio::Folium& );
        void handleApplyMethod( const adcontrols::ProcessMethod& );

    signals:
        void fireSetData( const adcontrols::MSCalibrateResult&, const adcontrols::MassSpectrum& );

    private slots:
        void handleSelSummary( size_t );
        void handleManuallyAssigned();
        void handleValueChanged();
        void handleUpdatePeakAssign();

    private:
        void init( const adportable::Configuration&, const std::wstring& );
        void applyAssigned( const adcontrols::MSAssignedMasses&, const portfolio::Folium& );

        typedef std::pair< boost::shared_ptr< adcontrols::MSCalibrateResult >, boost::shared_ptr< adcontrols::MassSpectrum > > result_type;
        int populate( std::vector< result_type >& );
        void doCalibration( adcontrols::MassSpectrum& centroid, adcontrols::MSCalibrateResult&, const adcontrols::MSAssignedMasses& assigned );

        std::vector< boost::shared_ptr< adwplot::SpectrumWidget > > wndSpectra_;
        std::vector< portfolio::Folium > folio_;
        portfolio::Folium folium_;
        QWidget * wndCalibSummary_;
        QSplitter * wndSplitter_;
        std::vector< boost::shared_ptr< adcontrols::MassSpectrum > > spectra_;
    };

}

#endif // MSCALIBSPECTRAWND_HPP