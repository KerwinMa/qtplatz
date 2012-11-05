/**************************************************************************
** Copyright (C) 2010-2013 Toshinobu Hondo, Ph.D.
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

#ifndef MASSDEFECTFORM_HPP
#define MASSDEFECTFORM_HPP

#include <QWidget>
#include <boost/smart_ptr.hpp>

namespace Ui {
class MassDefectForm;
}

namespace adportable {
	class Configuration;
}

class QStandardItemModel;

namespace chemistry {

	class MassDefectMethod;
	class MassDefectDelegate;

	class MassDefectForm : public QWidget {
		Q_OBJECT
    
	public:
		explicit MassDefectForm(QWidget *parent = 0);
		~MassDefectForm();

		// adplugin::LifeCycle
		void OnCreate( const adportable::Configuration& );
		void OnInitialUpdate();
		void OnFinalClose();

	private:
		Ui::MassDefectForm *ui;
		boost::scoped_ptr< QStandardItemModel > model_;
		boost::scoped_ptr< adportable::Configuration > config_;
		boost::scoped_ptr< chemistry::MassDefectMethod > method_;
		boost::scoped_ptr< chemistry::MassDefectDelegate > delegate_;
	};

}

#endif // MASSDEFECTFORM_HPP