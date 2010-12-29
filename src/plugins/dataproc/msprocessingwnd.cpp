//////////////////////////////////////////
// Copyright (C) 2010 Toshinobu Hondo, Ph.D.
// Science Liaison / Advanced Instrumentation Project
//////////////////////////////////////////

#include "msprocessingwnd.h"
#include "dataprocessor.h"
#include <adcontrols/chromatogram.h>
#include <adcontrols/massspectrum.h>
#include <adcontrols/description.h>
#include <adcontrols/lcmsdataset.h>
#include <adcontrols/datafile.h>
#include <adutils/processeddata.h>
#include <portfolio/folium.h>
#include <adwidgets/chromatogramwidget.h>
#include <adwidgets/spectrumwidget.h>
#include <adwidgets/axis.h>
#include <coreplugin/minisplitter.h>
#include <QBoxLayout>
#include <boost/variant.hpp>

using namespace dataproc;
using namespace dataproc::internal;

namespace dataproc {
    namespace internal {
        class MSProcessingWndImpl {
        public:
            ~MSProcessingWndImpl() {}
            MSProcessingWndImpl() : ticPlot_(0)
                                  , profileSpectrum_(0)
                                  , processedSpectrum_(0) {
            }

            adwidgets::ui::ChromatogramWidget * ticPlot_;
            adwidgets::ui::SpectrumWidget * profileSpectrum_;
            adwidgets::ui::SpectrumWidget * processedSpectrum_;

        };
    }
}

MSProcessingWnd::MSProcessingWnd(QWidget *parent) :
    QWidget(parent)
{
    init();
}

void
MSProcessingWnd::init()
{
    pImpl_.reset( new MSProcessingWndImpl );
    Core::MiniSplitter * splitter = new Core::MiniSplitter;
    if ( splitter ) {
        if ( pImpl_->ticPlot_ = new adwidgets::ui::ChromatogramWidget ) {
            adwidgets::ui::Axis axis = pImpl_->ticPlot_->axisX();
            axis.text( L"Time(min)" );
        }

        if ( pImpl_->profileSpectrum_ = new adwidgets::ui::SpectrumWidget ) {
            adwidgets::ui::Axis axis = pImpl_->profileSpectrum_->axisX();
            axis.text( L"m/z" );
        }

        if ( pImpl_->processedSpectrum_ = new adwidgets::ui::SpectrumWidget ) {
            adwidgets::ui::Axis axis = pImpl_->processedSpectrum_->axisX();
            axis.text( L"m/z" );
        }
        splitter->addWidget( pImpl_->ticPlot_ );
        splitter->addWidget( pImpl_->profileSpectrum_ );
        splitter->addWidget( pImpl_->processedSpectrum_ );
        splitter->setOrientation( Qt::Vertical );

        pImpl_->profileSpectrum_->link( pImpl_->processedSpectrum_ );
        pImpl_->processedSpectrum_->link( pImpl_->profileSpectrum_ );
    }

    QBoxLayout * toolBarAddingLayout = new QVBoxLayout( this );
    toolBarAddingLayout->setMargin(0);
    toolBarAddingLayout->setSpacing(0);
    //toolBarAddingLayout->addWidget( toolBar );
    toolBarAddingLayout->addWidget( splitter );
    //toolBarAddingLayout->addWidget( toolBar2 );

}

void
MSProcessingWnd::draw1( adutils::MassSpectrumPtr& ptr )
{
    adcontrols::MassSpectrum& ms = *ptr;
    pImpl_->profileSpectrum_->setData( ms );
}

void
MSProcessingWnd::draw2( adutils::MassSpectrumPtr& ptr )
{
    adcontrols::MassSpectrum& ms = *ptr;
    pImpl_->processedSpectrum_->setData( ms );
}

void
MSProcessingWnd::draw( adutils::ChromatogramPtr& ptr )
{
    adcontrols::Chromatogram& c = *ptr;
    pImpl_->ticPlot_->setData( c );
}

void
MSProcessingWnd::handleSessionAdded( Dataprocessor * processor )
{
    adcontrols::LCMSDataset * dset = processor->getLCMSDataset();
    if ( dset ) {
        adcontrols::Chromatogram c;
        if ( dset->getTIC( 0, c ) ) {
            if ( c.isConstantSampledData() )
                c.getTimeArray();
            c.addDescription( adcontrols::Description( L"filename", processor->file().filename() ) );
            pImpl_->ticPlot_->setData( c );
        }
    }
}

struct selChanged : public boost::static_visitor<void> {
    selChanged( MSProcessingWnd& wnd ) : wnd_(wnd) {}
    template<typename T> void operator ()( T& ) const { }
    template<> void operator () ( adutils::MassSpectrumPtr& ptr ) const {   
        wnd_.draw1( ptr );
    }
    template<> void operator () ( adutils::ChromatogramPtr& ptr ) const {
        wnd_.draw( ptr );
    }
    MSProcessingWnd& wnd_;
};

struct selProcessed : public boost::static_visitor<void> {
    selProcessed( MSProcessingWnd& wnd ) : wnd_(wnd) {}
    template<typename T> void operator ()( T& ) const { }
    template<> void operator () ( adutils::MassSpectrumPtr& ptr ) const {   
        wnd_.draw2( ptr );
    }
    template<> void operator () ( adutils::ChromatogramPtr& ptr ) const {
        wnd_.draw( ptr );
    }
    MSProcessingWnd& wnd_;
};

void
MSProcessingWnd::handleSelectionChanged( Dataprocessor* processor, portfolio::Folium& folium )
{
    adutils::ProcessedData::value_type data = adutils::ProcessedData::toVariant( static_cast<boost::any&>( folium ) );
    boost::apply_visitor( selChanged(*this), data );

    portfolio::Folio attachments = folium.attachments();
    for ( portfolio::Folio::iterator it = attachments.begin(); it != attachments.end(); ++it ) {
        adutils::ProcessedData::value_type contents = adutils::ProcessedData::toVariant( static_cast<boost::any&>( *it ) );
        boost::apply_visitor( selProcessed( *this ), contents );
    }
}
