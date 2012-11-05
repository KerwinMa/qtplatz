/**************************************************************************
** Copyright (C) 2010-2013 Toshinobu Hondo, Ph.D.
** Science Liaison / Advanced Instrumentation Project
*
** Contact: toshi.hondo@scienceliaison.com
**
** Commercial Usage
**
** Licensees holding valid ScienceLiaison commercial licenses may use this file in
** accordance with the ScienceLiaison Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and ScienceLiaison.
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

#ifndef SVGITEM_HPP
#define SVGITEM_HPP

#include <qbytearray.h>
#include <qmetatype.h>

class QPainter;
class QRect;
class QPalette;
// enum EditMode;

namespace chemistry {

	class SvgItem {
	public:
		QByteArray svg_;
		
		SvgItem();
		SvgItem( const SvgItem& );
		void paint( QPainter *, const QRect&, const QPalette& ) const;
	};

}

Q_DECLARE_METATYPE( chemistry::SvgItem )

#endif // SVGITEM_HPP