//////////////////////////////////////////
// Copyright (C) 2010 Toshinobu Hondo, Ph.D.
// Science Liaison / Advanced Instrumentation Project
//////////////////////////////////////////

#include "centroidmethod.h"

using namespace adcontrols;

CentroidMethod::~CentroidMethod(void)
{
}

CentroidMethod::CentroidMethod() : peakWidthMethod_(ePeakWidthTOF)
                                   , peakCentroidFraction_(0.5)
                                   , baselineWidth_(500.0)
                                   , rsConstInDa_(1.0)
                                   , rsPropoInPpm_(200.0)
                                   , rsTofInDa_(0.1)
                                   , rsTofAtMz_(600.0)
                                   , attenuation_(5.0)
                                   , bCentroidAreaIntensity_(true)
{
}

CentroidMethod::CentroidMethod(const CentroidMethod& t)
{
	operator=(t);
}

CentroidMethod&
CentroidMethod::operator = ( const CentroidMethod& rhs )
{
	peakWidthMethod( rhs.peakWidthMethod() );
	rsTofInDa( rhs.rsTofInDa() );
	rsTofAtMz( rhs.rsTofAtMz() );
	rsPropoInPpm( rhs.rsPropoInPpm() );
	rsConstInDa( rhs.rsConstInDa() );
	baselineWidth( rhs.baselineWidth() );
	attenuation( rhs.attenuation() );
	centroidAreaIntensity( rhs.centroidAreaIntensity() );
	peakWidthMethod( rhs.peakWidthMethod() );
	peakCentroidFraction( rhs.peakCentroidFraction() );

	return * this;
}

bool
CentroidMethod::operator == ( const CentroidMethod & rhs ) const
{
	return	peakWidthMethod() == rhs.peakWidthMethod() &&
				rsTofInDa() == rhs.rsTofInDa() &&
				rsTofAtMz() == rhs.rsTofAtMz() &&
				rsPropoInPpm() == rhs.rsPropoInPpm() &&
				rsConstInDa() == rhs.rsConstInDa() &&
				baselineWidth() == rhs.baselineWidth() &&
				attenuation() == rhs.attenuation() &&
				centroidAreaIntensity() == rhs.centroidAreaIntensity() &&
				peakWidthMethod() == rhs.peakWidthMethod() &&
				peakCentroidFraction() == rhs.peakCentroidFraction();
}

bool
CentroidMethod::operator != ( const CentroidMethod & rhs ) const
{
	return ! operator == ( rhs );
}

double 
CentroidMethod::baselineWidth() const
{
	return baselineWidth_;
}

double 
CentroidMethod::rsConstInDa() const
{
  return rsConstInDa_;
}

double 
CentroidMethod::rsPropoInPpm() const
{
	return rsPropoInPpm_;
}

double 
CentroidMethod::rsTofInDa() const
{
	return rsTofInDa_;
}

double 
CentroidMethod::rsTofAtMz() const
{
	return rsTofAtMz_;
}

double 
CentroidMethod::attenuation() const
{
	return attenuation_;
}

CentroidMethod::ePeakWidthMethod
CentroidMethod::peakWidthMethod() const
{
  return peakWidthMethod_;
}

bool
CentroidMethod::centroidAreaIntensity() const
{
  return bCentroidAreaIntensity_;
}

double
CentroidMethod::peakCentroidFraction() const
{
  return peakCentroidFraction_;
}

void  
CentroidMethod::baselineWidth(double v)
{
	baselineWidth_ = v;
}

void  
CentroidMethod::rsConstInDa(double v)
{
	rsConstInDa_ = v;
}

void  
CentroidMethod::rsPropoInPpm(double v)
{
	rsPropoInPpm_ = v;
}

void  
CentroidMethod::rsTofInDa(double v)
{
	rsTofInDa_ = v;
}

void  
CentroidMethod::rsTofAtMz(double v)
{
	rsTofAtMz_ = v;
}

void  
CentroidMethod::attenuation(double v)
{
	attenuation_ = v;
}

void
CentroidMethod::peakWidthMethod(ePeakWidthMethod e)
{
	peakWidthMethod_ = e;
}

void 
CentroidMethod::centroidAreaIntensity(bool f)
{
	bCentroidAreaIntensity_ = f;
}

void
CentroidMethod::peakCentroidFraction(double v)
{
	peakCentroidFraction_ = v;
}