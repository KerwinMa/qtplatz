/**************************************************************************
** Copyright (C) 2010-2012 Toshinobu Hondo, Ph.D.
** Copyright (C) 2013 MS-Cheminformatics LLC
*
** Contact: toshi.hondo@scienceliaison.com
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

#ifndef CHEMSPIDERMANAGER_HPP
#define CHEMSPIDERMANAGER_HPP

#include <QObject>
#include <vector>
#include <boost/smart_ptr.hpp>
#include <utils/fancymainwindow.h>

class QDockWidget;
class QMainWindow;
namespace Utils { class FancyMainWindow; }
namespace adportable { class Configuration; }
namespace adcontrols { class ProcessMethod; }

namespace ChemSpider { namespace Internal {

	class ChemSpiderManager : public QObject {
		Q_OBJECT
	public:
		explicit ChemSpiderManager(QObject *parent = 0);
		void init( const adportable::Configuration&, const std::wstring& apppath );

        void OnInitialUpdate();
        void OnFinalClose();
		QMainWindow * mainWindow() const;
		void setSimpleDockWidgetArrangement();

    signals:
		void signalGetProcessMethod( adcontrols::ProcessMethod& );

	public slots:
		void handleApply( adcontrols::ProcessMethod& );

	private:
		boost::scoped_ptr< Utils::FancyMainWindow > mainWindow_;    
		std::vector< QDockWidget * > dockWidgets_;
	};

}
}

#endif // CHEMSPIDERMANAGER_HPP
