// This is a -*- C++ -*- header.
/**************************************************************************
** Copyright (C) 2013 MS-Cheminformatics LLC
** Copyright (C) 2010-2011 Toshinobu Hondo, Ph.D.
*
** Contact: info@ms-cheminfo.com
**
** Commercial Usage
**
** Licensees holding valid MS-Cheminformatics commercial licenses may use this file in
** accordance with the MS-Cheminformatics Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and MS-Cheminformatics.
**
** GNU Lesser General Public License Usage
**
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.TXT included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
**************************************************************************/

#pragma once

#include "adplugin_global.h"
#include <QObject>

namespace adplugin {

    class ADPLUGINSHARED_EXPORT ifactory {
	public:
        ifactory() {}
        virtual ~ifactory() {}

	virtual QWidget * create_widget( const wchar_t * iid, QWidget * parent = 0 ) = 0;
	virtual QObject * create_object( const wchar_t * iid, QObject * parent = 0 ) = 0;
        virtual void release() = 0;
    };

}
