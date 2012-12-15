/**************************************************************************
** Copyright (C) 2010-2012 Toshinobu Hondo, Ph.D.
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

#include "mode.hpp"
#include "constants.hpp"
#include <coreplugin/editormanager/editormanager.h>
#include <coreplugin/coreconstants.h>
#include <coreplugin/uniqueidmanager.h>
#include <coreplugin/modemanager.h>
#include <coreplugin/editormanager/ieditor.h>

using namespace frequency;

Mode::Mode(QObject * /* parent */)
{
	setName( tr("Frequency") );
	setUniqueModeName( Constants::C_FREQ_MODE );
    setIcon( QIcon( ":/frequency/images/mode_frequency.png" ) );
	setPriority( 960 );
	
	QList<int> contexts = QList<int>() <<
        Core::UniqueIDManager::instance()->uniqueIdentifier( Core::Constants::C_EDITORMANAGER ) <<
        Core::UniqueIDManager::instance()->uniqueIdentifier( frequency::Constants::C_FREQ_MODE ) <<
        Core::UniqueIDManager::instance()->uniqueIdentifier( Core::Constants::C_NAVIGATION_PANE );
    setContext( contexts );

    Core::ModeManager *modeManager = Core::ModeManager::instance();
    connect(modeManager, SIGNAL(currentModeChanged(Core::IMode*)), this, SLOT(grabEditorManager(Core::IMode*)));
}

Mode::~Mode()
{
}

void
Mode::grabEditorManager(Core::IMode *mode)
{
    if (mode != this)
        return;

    Core::EditorManager * em = Core::EditorManager::instance();
    
    if ( em->currentEditor() )
        em->currentEditor()->widget()->setFocus();
}
