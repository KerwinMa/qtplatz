//////////////////////////////////////////
// Copyright (C) 2010 Toshinobu Hondo, Ph.D.
// Science Liaison / Advanced Instrumentation Project
//////////////////////////////////////////

#include "dataplotimpl.h"
#include "dataplot.h"
#include <QResizeEvent>
#include <atlbase.h>
#include <atlcom.h>
#include <QAxWidget>  // Fix me, this module requre commercial license
#include <QUuid>

#include "import_sagraphics.h"

#define QCLSID_SADataplot "{1033423F-6431-46CD-9824-C1A9CAE5861E}"
static QUuid QIID_ISADataplot(0x9bda62de,0x514e,0x4ffb,0x8d,0xcc,0xe1,0xa3,0x55,0xcf,0x6b,0xff);

using namespace adil::ui::internal::win32;

DataplotImpl::DataplotImpl( adil::ui::Dataplot& dataplot ) : dataplot_( dataplot )
                                                 , QAxWidget( &dataplot )
{
}

DataplotImpl::~DataplotImpl()
{
	HRESULT hr;
	hr = IDispEventSimpleImpl<100, DataplotImpl, &DIID__ISADataplotEvents>::DispEventUnadvise( pi_ );
	ATLASSERT( hr == S_OK);
	hr = IDispEventSimpleImpl<100, DataplotImpl, &DIID__ISADataplotEvents2>::DispEventUnadvise( pi_ );
    ATLASSERT( hr == S_OK );
}

bool
DataplotImpl::createControl()
{
	if ( this->setControl( QCLSID_SADataplot ) ) {
		pi_.Release();
		if ( this->queryInterface( QIID_ISADataplot, reinterpret_cast<void **>(&pi_) ) == S_OK ) {
            HRESULT hr;
            hr = IDispEventSimpleImpl<100, DataplotImpl, &DIID__ISADataplotEvents>::DispEventAdvise( pi_ );
            ATLASSERT( hr == S_OK );
            hr = IDispEventSimpleImpl<100, DataplotImpl, &DIID__ISADataplotEvents2>::DispEventAdvise( pi_ );
            ATLASSERT( hr == S_OK );
			this->activateWindow();
			return true;
		}
	}
	return false;
}

STDMETHODIMP
DataplotImpl::OnMouseDown( double x, double y, short button )
{
    dataplot_.OnMouseDown( x, y, button );
	return S_OK;
}

STDMETHODIMP
DataplotImpl::OnMouseUp( double x, double y, short button )
{
    dataplot_.OnMouseUp( x, y, button );
	return S_OK;
}

STDMETHODIMP
DataplotImpl::OnMouseMove( double x, double y, short button )
{
    dataplot_.OnMouseMove( x, y, button );
	return S_OK;
}

STDMETHODIMP
DataplotImpl::OnCharacter( long KeyCode )
{
    dataplot_.OnCharacter( KeyCode );
	return S_OK;
}

STDMETHODIMP
DataplotImpl::OnKeyDown( long KeyCode )
{
    dataplot_.OnKeyDown( KeyCode );
	return S_OK;
}

STDMETHODIMP
DataplotImpl::OnSetFocus( long hWnd )
{
    dataplot_.OnSetFocus( hWnd );
	return S_OK;
}

STDMETHODIMP
DataplotImpl::OnKillFocus( long hWnd )
{
    dataplot_.OnKillFocus( hWnd );
	return S_OK;
}

STDMETHODIMP
DataplotImpl::OnMouseDblClk(double x, double y, short button )
{
    dataplot_.OnMouseDblClk( x, y, button );
	return S_OK;
}
