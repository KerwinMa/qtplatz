// This is a -*- C++ -*- header.
//////////////////////////////////////////
// Copyright (C) 2010 Toshinobu Hondo, Ph.D.
// Science Liaison / Advanced Instrumentation Project
//////////////////////////////////////////

#pragma once

#include <QtCore/qglobal.h>

#if defined(SERVANT_LIBRARY)
#  define SERVANTSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SERVANTSHARED_EXPORT Q_DECL_IMPORT
#endif
