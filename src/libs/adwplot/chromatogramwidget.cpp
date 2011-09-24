// -*- C++ -*-
/**************************************************************************
** Copyright (C) 2010-2011 Toshinobu Hondo, Ph.D.
** Science Liaison / Advanced Instrumentation Project
*
** Contact: toshi.hondo@scienceliaison.com
**
** Commercial Usage
**
** Licensees holding valid ScienceLiaison commercial licenses may use this
** file in accordance with the ScienceLiaison Commercial License Agreement
** provided with the Software or, alternatively, in accordance with the terms
** contained in a written agreement between you and ScienceLiaison.
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

#include "chromatogramwidget.hpp"
#include "annotation.hpp"
#include "annotations.hpp"
//#include "trace.h"
//#include "traces.h"
#include "zoomer.hpp"
#include "peak.hpp"
#include "baseline.hpp"
#include "plotcurve.hpp"
#include "plotpicker.hpp"
#include "plotpanner.hpp"
#include "seriesdata.hpp"
#include <adcontrols/trace.hpp>
#include <adcontrols/chromatogram.hpp>
#include <adcontrols/peaks.hpp>
#include <adcontrols/peak.hpp>
#include <adcontrols/baselines.hpp>
#include <adcontrols/baseline.hpp>
#include <adcontrols/descriptions.hpp>
#include <adcontrols/description.hpp>
#include <boost/format.hpp>
#include <boost/foreach.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/variant.hpp>
#include <queue>

using namespace adwplot;

namespace adwplot {
    namespace chromatogram_internal {

        template<class T> class series_data : public QwtSeriesData< QPointF >, boost::noncopyable {
            T t_;
        public:
            series_data() {  }
            // series_data( const T& t ) : t_( t ) {  }
            inline operator T& () { return t_; };

            // implements QwtSeriesData<>
            virtual size_t size() const { return t_.size(); }
            virtual QPointF sample( size_t idx ) const { 
                using adcontrols::Chromatogram;
                return QPointF( Chromatogram::toMinutes( t_.getTimeArray()[idx] ), t_.getIntensityArray()[idx] );
            }
            virtual QRectF boundingRect() const { return rect_; }

            // series_data
            void boundingRect( const QRectF& rc ) { rect_ = rc; }
        private:
            QRectF rect_;
        };

        // typedef boost::variant< series_data< adcontrols::Chromatogram>, series_data<adcontrols::Trace > > trace_variant;

        template<class T> class TraceData {
        public:
            ~TraceData() {
            }
            TraceData( Dataplot& plot ) : curve_( plot ), data_( new series_data<T>() ) {
            }
            TraceData( const TraceData& t ) : curve_( t.curve_ ), data_( t.data_ ) {
            }
            void setData( const T& ) {
            }
            inline series_data<T>* get() { return data_.get(); }
        private:
            PlotCurve curve_;
            boost::shared_ptr< series_data<T> > data_;  // must be shared pointer for push into vector
        };

        template<> void TraceData<adcontrols::Trace>::setData( const adcontrols::Trace& trace )
        {
            using adcontrols::Trace;
            namespace ci = chromatogram_internal;

            if ( trace.size() <= 2 )
                return;

            // TODO:  reractor code in order to avoid full data copy
            static_cast<Trace&>(*data_) = trace;
            QRectF rect;
            rect.setCoords( data_->sample(0).x(), trace.range_y().first
                            , data_->sample( trace.size() - 1 ).x(), trace.range_y().second );
            data_->boundingRect( rect );
            curve_.p()->setData( data_.get() );
        }

        template<> void TraceData<adcontrols::Chromatogram>::setData( const adcontrols::Chromatogram& c )
        {
            using adcontrols::Chromatogram;
            const double * intens = c.getIntensityArray();

            static_cast<Chromatogram&>(*data_) = c;
            QRectF rect;
            rect.setCoords( data_->sample(0).x(), intens[ c.min_element() ]
                            , data_->sample( c.size() - 1 ).x(), intens[ c.max_element() ] );
            data_->boundingRect( rect );
            curve_.p()->setData( data_.get() );
        }

        typedef boost::variant< TraceData<adcontrols::Chromatogram>, TraceData<adcontrols::Trace> > trace_variant;        
    }

    struct ChromatogramWidgetImpl : boost::noncopyable {
        std::vector< Annotation > annotations_;
        std::vector< chromatogram_internal::trace_variant > traces_;
        std::vector< Peak > peaks_;
        std::vector< Baseline > baselines_;	
    };
}

ChromatogramWidget::~ChromatogramWidget()
{
    delete impl_;
}

ChromatogramWidget::ChromatogramWidget(QWidget *parent) : Dataplot(parent)
							, impl_( new ChromatogramWidgetImpl )
{
    setAxisTitle(QwtPlot::xBottom, "Time[min]");
    setAxisTitle(QwtPlot::yLeft, "Intensity[uV]");
}

void
ChromatogramWidget::setData( const adcontrols::Trace& d, int idx, bool yaxis2 )
{
    (void)yaxis2;

    if ( d.size() < 2 )
        return;

    using adcontrols::Trace;
    using chromatogram_internal::TraceData;

    while ( int( impl_->traces_.size() ) <= idx )
        impl_->traces_.push_back( TraceData<Trace>( *this ) );

    QRectF rect;
    try {
        TraceData<Trace>& trace = boost::get< TraceData<Trace> >( impl_->traces_[ idx ] );
        trace.setData( d );
        rect = trace.get()->boundingRect();
    } catch ( boost::bad_get& ) {
        std::cerr << "boost::bad_get at " << __FILE__ << " line: " << __LINE__ << std::endl;
    }

    setAxisScale( QwtPlot::xBottom, rect.left(), rect.right() );
    setAxisScale( yaxis2 ? QwtPlot::yRight : QwtPlot::yLeft, rect.bottom(), rect.top() );
    zoomer1_->setZoomBase();
}

void
ChromatogramWidget::setData( const adcontrols::Chromatogram& c )
{
    impl_->annotations_.clear();
    impl_->peaks_.clear();
    impl_->baselines_.clear();
    impl_->traces_.clear();

    using adcontrols::Chromatogram;
    using chromatogram_internal::TraceData;

    impl_->traces_.push_back( TraceData<Chromatogram>( *this ) );
    TraceData<Chromatogram>& trace = boost::get<TraceData<Chromatogram> >(impl_->traces_.back());

    trace.setData( c );

    const double * intens = c.getIntensityArray();

#if 0
    const adcontrols::Baselines& baselines = c.baselines();
    for ( adcontrols::Baselines::vector_type::const_iterator it = baselines.begin(); it != baselines.end(); ++it )
        setBaseline( *it );
#endif
    const adcontrols::Peaks& peaks = c.peaks();

    for ( adcontrols::Peaks::vector_type::const_iterator it = peaks.begin(); it != peaks.end(); ++it )
        setPeak( *it );

    std::pair< double, double > time_range = adcontrols::timeutil::toMinutes( c.timeRange() );
    setAxisScale( QwtPlot::xBottom, time_range.first, time_range.second );
    setAxisScale( QwtPlot::yLeft, intens[ c.min_element() ], intens[ c.max_element() ] );
    zoomer1_->setZoomBase();
    // replot();
}
    

void
ChromatogramWidget::setPeak( const adcontrols::Peak& peak )
{
    double tR = adcontrols::timeutil::toMinutes( peak.peakTime() );

    std::wstring label = peak.name();
    if ( label.empty() )
        label = ( boost::wformat( L"%.3lf" ) % tR ).str();

    Annotations annots( *this, impl_->annotations_ );

    Annotation anno = annots.add( tR, peak.peakHeight(), label );
    anno.setLabelAlighment( Qt::AlignTop | Qt::AlignCenter );

    impl_->peaks_.push_back( adwplot::Peak( *this, peak ) );
}

void
ChromatogramWidget::setBaseline( const adcontrols::Baseline& bs )
{
    impl_->baselines_.push_back( adwplot::Baseline( *this, bs ) );
}

void
ChromatogramWidget::override_zoom_rect( QRectF& )
{
}

void
ChromatogramWidget::zoom( const QRectF& rect )
{
    zoomer1_->zoom( rect );
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

