// This is a -*- C++ -*- header.
//////////////////////////////////////////
// Copyright (C) 2010 Toshinobu Hondo, Ph.D.
// Science Liaison / Advanced Instrumentation Project
//////////////////////////////////////////

#ifndef MONITOR_UI_H
#define MONITOR_UI_H

#include <adplugin/imonitor.h>

namespace adtofms {

	class monitor_ui : public adplugin::ui::IMonitor	{
		Q_OBJECT
		Q_INTERFACES( adplugin::ui::IMonitor )
	public:
		explicit monitor_ui(QWidget *parent = 0);

		// implement IMonitor
		virtual void OnInitialUpdate( const wchar_t * xml = 0 );
		virtual void OnFinalClose();

    signals:

	public slots:

	};

}

#endif // MONITOR_UI_H
