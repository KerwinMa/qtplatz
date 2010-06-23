//////////////////////////////////////////
// Copyright (C) 2010 Toshinobu Hondo, Ph.D.
// Science Liaison / Advanced Instrumentation Project
//////////////////////////////////////////
#include "traces.h"
#include "trace.h"
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

Trace
Traces::operator [] (long idx)
{
  return item(idx);
}

Trace
Traces::item(long Index)
{
  CComPtr<ISADPTrace> p;
  pi_->get_Item( Index, &p );
  return Trace( p );
}

size_t
Traces::size() const
{
  long nSize = 0;
  if ( pi_->get_Count(&nSize) == S_OK )
    return size_t(nSize);
  return 0;
}

Trace
Traces::add()
{
  CComPtr<ISADPTrace> p;
  pi_->Add(&p);
  return Trace(p);
}

void
Traces::remove(long Index)
{
  pi_->Remove( Index );
}

void
Traces::clear()
{
  pi_->Clear();
}

bool
Traces::visible() const
{
	VARIANT_BOOL v;
	pi_->get_Visible(&v);
	return internal::variant_bool::to_native(v);
}

void
Traces::visible(bool newValue)
{
	pi_->put_Visible( internal::variant_bool::to_variant(newValue) );
}


