// This is a -*- C++ -*- header.
//////////////////////////////////////////
// Copyright (C) 2010 Toshinobu Hondo, Ph.D.
// Science Liaison / Advanced Instrumentation Project
//////////////////////////////////////////

#pragma once

#include <extensionsystem/iplugin.h>

namespace adportable {
	class Configuration;
}

namespace servant {

    namespace internal {
        class MainWindow;
    }

	class ServantPlugin : public ExtensionSystem::IPlugin {
	  Q_OBJECT
		public:
	  explicit ServantPlugin();
	  ~ServantPlugin();

      // ExtensionSystem::IPlugin
      virtual bool initialize(const QStringList &arguments, QString *error_message);
	  virtual void extensionsInitialized();
	  virtual void shutdown();
	  
	signals:

	public slots:
      

	private:
        internal::MainWindow * mainWindow_;
		adportable::Configuration * pConfig_;
	  
	};
}