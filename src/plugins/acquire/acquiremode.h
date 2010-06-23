// This is a -*- C++ -*- header.
//////////////////////////////////////////
// Copyright (C) 2010 Toshinobu Hondo, Ph.D.
// Science Liaison / Advanced Instrumentation Project
//////////////////////////////////////////

#ifndef ACQUIREMODE_H
#define ACQUIREMODE_H

#include <coreplugin/basemode.h>

namespace Acquire {
  namespace internal {
    
    class AcquireMode : public Core::BaseMode {
      Q_OBJECT
	;
    public:
      ~AcquireMode();
      explicit AcquireMode(QObject *parent = 0);

    signals:

    public slots:

    private:

    };
    //---------
  }
}

#endif // ACQUIREMODE_H
