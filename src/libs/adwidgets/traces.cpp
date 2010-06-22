//////////////////////////////////////////
// Copyright (C) 2010 Toshinobu Hondo, Ph.D.
// Science Liaison / Advanced Instrumentation Project
//////////////////////////////////////////
#include "traces.h"
#include "import_sagraphics.h"

using namespace adil::ui;

Traces::~Traces()
{
  if ( pi_ )
    pi_->Release();
}

Traces::Traces( ISADPTraces * pi ) : pi_(pi)
{
  pi_->AddRef();
}

Traces::Traces( const Traces& t )
{
   if ( t.pi_ )
     t.pi_->AddRef(); // AddRef first, in order to avoid unexpected release when self assignment happens
   if ( pi_ )
     pi_->Release();
   pi_ = t.pi_;
}
