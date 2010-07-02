// This is a -*- C++ -*- header.
//////////////////////////////////////////
// Copyright (C) 2010 Toshinobu Hondo, Ph.D.
// Science Liaison / Advanced Instrumentation Project
//////////////////////////////////////////

#ifndef MSCALIBRATIONWND_H
#define MSCALIBRATIONWND_H

#include <QWidget>
#include <boost/smart_ptr.hpp>

namespace dataproc {
  namespace internal {

    class MSCalibrationWndImpl;

    class MSCalibrationWnd : public QWidget {
      Q_OBJECT
	public:
      explicit MSCalibrationWnd(QWidget *parent = 0);
      void init();
      
    signals:
      
      public slots:

      private:
        boost::shared_ptr<MSCalibrationWndImpl> pImpl_;
      
    };

  }
}

#endif // MSCALIBRATIONWND_H