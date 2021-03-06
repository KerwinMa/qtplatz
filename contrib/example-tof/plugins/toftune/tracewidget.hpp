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

#pragma once

#include <adwplot/dataplot.hpp>

namespace adcontrols { class MassSpectrum; }

namespace hmqtune {

    namespace internal { class TraceData; }

    class TraceWidget : public adwplot::Dataplot {
        Q_OBJECT
    public:
        explicit TraceWidget(QWidget *parent = 0);
        ~TraceWidget();

        void clear();
        void setData( const adcontrols::MassSpectrum& );
        void setData( const adcontrols::MassSpectrum&, const adcontrols::MassSpectrum& );
        void setData( const adcontrols::MassSpectrum&, int idx, bool yaxis1 = false );

    private:
        struct TraceWidgetImpl * impl_;
        bool autoYZoom_;

    signals:
        void onMoved( const QPointF& );
		void onSelected( const QPointF& );
		void onSelected( const QRectF& );

	private:
        virtual void zoom( const QRectF& );

    public slots:
		virtual void override_zoom_rect( QRectF& );
		virtual void moved( const QPointF& );
		virtual void selected( const QPointF& );
		virtual void selected( const QRectF& );
    };

}

