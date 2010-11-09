//////////////////////////////////////////
// Copyright (C) 2010 Toshinobu Hondo, Ph.D.
// Science Liaison / Advanced Instrumentation Project
//////////////////////////////////////////

#include "sequence.h"

#if defined _DEBUG
//#     pragma comment(lib, "adwidgetsd.lib")
#     pragma comment(lib, "adportabled.lib")
#     pragma comment(lib, "adplugind.lib")
#     pragma comment(lib, "adcontrolsd.lib")
#     pragma comment(lib, "acewrapperd.lib")
#     pragma comment(lib, "qtwrapperd.lib")
//#     pragma comment(lib, "ACEd.lib")
//#     pragma comment(lib, "QAxContainerd.lib")
#else
//#     pragma comment(lib, "adwidgets.lib")
#     pragma comment(lib, "adportable.lib")
#     pragma comment(lib, "adplugin.lib")
#     pragma comment(lib, "adcontrols.lib")
#     pragma comment(lib, "acewrapper.lib")
#     pragma comment(lib, "qtwrapper.lib")
//#     pragma comment(lib, "ACE.lib")
//#     pragma comment(lib, "QAxContainer.lib")
#endif

using namespace sequence::internal;

Sequence::~Sequence()
{
}

Sequence::Sequence(QObject *parent) :
    Core::IFile(parent)
    , modified_(true)
{
}

void
Sequence::setModified( bool val )
{
  if ( modified_ == val )
    return;
  modified_ = val;
  emit changed();
}

bool
Sequence::isModified() const
{
  return modified_;
}

QString
Sequence::mimeType() const
{
  return mimeType_;
}

bool
Sequence::save( const QString& filename )
{
  Q_UNUSED(filename);
  return false;
}

QString
Sequence::fileName() const
{
  return filename_;
}

QString
Sequence::defaultPath() const
{
  return QString();
}

QString
Sequence::suggestedFileName() const
{
  return QString();
}

bool
Sequence::isReadOnly() const
{
  return false;
}

bool
Sequence::isSaveAsAllowed() const
{
  return true;
}

void
Sequence::modified( ReloadBehavior* behavior)
{
  Q_UNUSED(behavior);
}
