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
//////////////////////////////////////////
// Copyright (C) 2010 Toshinobu Hondo, Ph.D.
// Science Liaison / Advanced Instrumentation Project
//////////////////////////////////////////

#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

#include <coreplugin/ifile.h>
#include <boost/smart_ptr.hpp>
#include <adcontrols/dataSubscriber.h>

namespace adcontrols {
    class datafile;
    class LCMSDataset;
}

namespace portfolio {
    class Portfolio;
    class Folium;
}

namespace dataproc {

    class datafileimpl;

    class Dataprocessor : QObject
                        , public adcontrols::dataSubscriber {
        Q_OBJECT
    public:
        ~Dataprocessor();
        Dataprocessor();

        bool open( const QString& );
        Core::IFile * ifile();

        QString filename() const;
        adcontrols::datafile& file();
        adcontrols::LCMSDataset* getLCMSDataset();
        portfolio::Portfolio getPortfolio();
        void setCurrentSelection( portfolio::Folium& );

        // implement adcontrols::dataSubscriber
        virtual void subscribe( adcontrols::LCMSDataset& );
        virtual void subscribe( adcontrols::ProcessedDataset& );
        // <------------------------

    signals:
        // void changeSelection( portfolio::Folium& );

    public slots:
        // void slotTitleChanged( const QString& title );
        // void handle_changeSelection( portfolio::Folium& );

    private:
        boost::scoped_ptr< datafileimpl > datafileimpl_;
        boost::scoped_ptr< portfolio::Portfolio > portfolio_;
    };

} // dataproc

#endif // DATAPROCESSOR_H
