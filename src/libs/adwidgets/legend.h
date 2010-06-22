// This is a -*- C++ -*- header.
//////////////////////////////////////////
// Copyright (C) 2010 Toshinobu Hondo, Ph.D.
// Science Liaison / Advanced Instrumentation Project
//////////////////////////////////////////

#ifndef LEGEND_H
#define LEGEND_H

struct ISADPLegend;

namespace adil {
  namespace ui {
    class Legend	  {
    public:
      ~Legend();
      Legend( ISADPLegend * pi = 0 );
      Legend( const Legend& );
	private:
      ISADPLegend * pi_;
    };
  }
}
#endif // LEGEND_H
