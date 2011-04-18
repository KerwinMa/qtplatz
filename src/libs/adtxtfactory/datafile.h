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

#ifndef DATAFILE_H
#define DATAFILE_H

#include <adcontrols/datafile.h>
#include <adcontrols/lcmsdataset.h>
#include <boost/smart_ptr.hpp>

namespace adcontrols {
    class Chromatogram;
    class MassSpectrum;
    class ProcessedDataset;
}

namespace adtxtfactory {
    class datafile : public adcontrols::datafile
                   , public adcontrols::LCMSDataset { 
    public:
        ~datafile();
        datafile();
        
        bool open( const std::wstring& filename, bool readonly = false );

        //--------- implement adcontrols::datafile ----------------
        virtual void accept( adcontrols::dataSubscriber& );
        virtual boost::any fetch( const std::wstring& path, const std::wstring& dataType ) const;

        virtual adcontrols::datafile::factory_type factory() { return 0; }

        // LCMSDataset
        virtual size_t getFunctionCount() const;
        virtual size_t getSpectrumCount( int fcn = 0 ) const;
        virtual size_t getChromatogramCount() const;
        virtual bool getTIC( int fcn, adcontrols::Chromatogram& ) const;
        virtual bool getSpectrum( int fcn, int idx, adcontrols::MassSpectrum& ) const;

    private:
        boost::scoped_ptr< adcontrols::ProcessedDataset > processedDataset_;
        boost::any data_;

    };
}

#endif // DATAFILE_H
