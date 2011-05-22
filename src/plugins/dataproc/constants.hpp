// This is a -*- C++ -*- header.
/**************************************************************************
** Copyright (C) 2010-2011 Toshinobu Hondo, Ph.D.
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

#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace dataproc {
    namespace Constants {
        const char * const C_DATAPROCESSOR = "Dataprocessor";
        const char * const C_DATA_MC4_MIMETYPE = "application/data";
        const char * const C_DATA_TEXT_MIMETYPE = "application/txt";
        const char * const C_DATA_INFITOF_MIMETYPE = "application/qtms";
        const char * const C_DATA_NATIVE_MIMETYPE = "application/adfs";
    }

    namespace internal {
        enum ProcessType {
            CentroidProcess
            , IsotopeProcess
            , CalibrationProcess
        };
    }
}


#endif // CONSTANTS_H