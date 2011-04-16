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

#include "dataplot.h"

namespace adcontrols {
    class Trace;
    class Chromatogram; 
    class Baseline;
    class Peak;
}

namespace adwplot {

    class Peak;
    class Baseline;
    class SeriesData;
    class Annotation;

    class ChromatogramWidget : public Dataplot {
        Q_OBJECT
    public:
        explicit ChromatogramWidget(QWidget *parent = 0);

        void setData( const adcontrols::Trace&, int idx = 0, bool yaxis2 = false );
        void setData( const adcontrols::Chromatogram& );
        void setBaseline( const adcontrols::Baseline& );
        void setPeak( const adcontrols::Peak& );

    signals:

    public slots:

    private:
        std::vector< Annotation > annotations_;
        std::vector< Trace > traces_;
        std::vector< Peak > peaks_;
        std::vector< Baseline > baselines_;
    };

}
