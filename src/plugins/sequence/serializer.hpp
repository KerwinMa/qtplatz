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

#ifndef SERIALIZER_HPP
#define SERIALIZER_HPP

#include <vector>
#include <boost/smart_ptr.hpp>

namespace ControlMethod { struct Method; }
namespace adcontrols { class ProcessMethod; }
namespace adfs { namespace detail { class cpio; } }

namespace sequence {

    class serializer {
    public:
        serializer();
        static bool archive( std::vector<char>&, const ControlMethod::Method& );
        static bool restore( ControlMethod::Method&, const std::vector<char>& );
        static bool restore( boost::shared_ptr<ControlMethod::Method>&, const std::vector<char>& );

        static bool archive( std::vector<char>&, const adcontrols::ProcessMethod& );
        static bool restore( adcontrols::ProcessMethod&, const std::vector<char>& );
        static bool restore( boost::shared_ptr<adcontrols::ProcessMethod>&, const std::vector<char>& );

        static bool archive( adfs::detail::cpio&, const ControlMethod::Method& );
        static bool restore( boost::shared_ptr< ControlMethod::Method>&, const adfs::detail::cpio& );
    };

}

#endif // SERIALIZER_HPP