
#include "acquireactions.h"
#include <utils/qtcassert.h>
#include <utils/pathchooser.h>

#include <QtCore/QDebug>
#include <QtCore/QSettings>

#include <QtGui/QAction>
#include <QtGui/QAbstractButton>
#include <QtGui/QRadioButton>
#include <QtGui/QCheckBox>
#include <QtGui/QLineEdit>

using namespace Utils;

namespace Acquire {
  namespace internal {

    //////////////////////////////////////////////////////////////////////////
    //
    // AcquireSettings
    //
    //////////////////////////////////////////////////////////////////////////
    
    AcquireSettings::AcquireSettings(QObject *parent) : QObject(parent) {}
    
    AcquireSettings::~AcquireSettings()
    {
      qDeleteAll(m_items);
    }
    
    void AcquireSettings::insertItem(int code, SavedAction *item)
    {
      QTC_ASSERT(!m_items.contains(code),
		 qDebug() << code << item->toString(); return);
      QTC_ASSERT(item->settingsKey().isEmpty() || item->defaultValue().isValid(),
		 qDebug() << "NO DEFAULT VALUE FOR " << item->settingsKey());
      m_items[code] = item;
    }
    
    void AcquireSettings::readSettings(QSettings *settings)
    {
      foreach (SavedAction *item, m_items)
        item->readSettings(settings);
    }
    
    void AcquireSettings::writeSettings(QSettings *settings) const
    {
      foreach (SavedAction *item, m_items)
        item->writeSettings(settings);
    }
    
    SavedAction *AcquireSettings::item(int code) const
    {
      QTC_ASSERT(m_items.value(code, 0), qDebug() << "CODE: " << code; return 0);
      return m_items.value(code, 0);
    }
    
    QString AcquireSettings::dump() const
    {
      QString out;
      QTextStream ts(&out);
      ts << "Acquire settings: ";
      foreach (SavedAction *item, m_items) {
        QString key = item->settingsKey();
        if (!key.isEmpty())
	  ts << '\n' << key << ": " << item->value().toString()
	     << "  (default: " << item->defaultValue().toString() << ")";
      }
      return out;
    }
    
    AcquireSettings *AcquireSettings::instance()
    {
      static AcquireSettings *instance = 0;
      if (instance)
        return instance;
      
      const QString debugModeGroup = QLatin1String("DebugMode");
      instance = new AcquireSettings;
      
      SavedAction *item = 0;
      
      item = new SavedAction(instance);
      instance->insertItem(SettingsDialog, item);
      item->setText(tr("Acquire properties..."));
      
      //
      // View
      //
      item = new SavedAction(instance);
      item->setText(tr("Adjust column widths to contents"));
      instance->insertItem(AdjustColumnWidths, item);
      
      item = new SavedAction(instance);
      item->setText(tr("Always adjust column widths to contents"));
      item->setCheckable(true);
      instance->insertItem(AlwaysAdjustColumnWidths, item);
      
      item = new SavedAction(instance);
      item->setText(tr("Use alternating row colors"));
      item->setSettingsKey(debugModeGroup, QLatin1String("UseAlternatingRowColours"));
      item->setCheckable(true);
      item->setDefaultValue(false);
      instance->insertItem(UseAlternatingRowColors, item);
      
      item = new SavedAction(instance);
      item->setText(tr("Show a message box when receiving a signal"));
      item->setSettingsKey(debugModeGroup, QLatin1String("UseMessageBoxForSignals"));
      item->setCheckable(true);
      item->setDefaultValue(true);
      item->setValue(true);
      instance->insertItem(UseMessageBoxForSignals, item);
      
      item = new SavedAction(instance);
      item->setText(tr("Log time stamps"));
      item->setSettingsKey(debugModeGroup, QLatin1String("LogTimeStamps"));
      item->setCheckable(true);
      item->setDefaultValue(false);
      instance->insertItem(LogTimeStamps, item);
      
      item = new SavedAction(instance);
      item->setText(tr("Operate by instruction"));
      item->setCheckable(true);
      item->setDefaultValue(false);
      item->setIcon(QIcon(":/debugger/images/debugger_stepoverproc_small.png"));
      item->setToolTip(tr("This switches the debugger to instruction-wise "
			  "operation mode. In this mode, stepping operates on single "
			  "instructions and the source location view also shows the "
			  "disassembled instructions."));
      instance->insertItem(OperateByInstruction, item);
      
      item = new SavedAction(instance);
      item->setText(tr("Dereference pointers automatically"));
      item->setCheckable(true);
      item->setDefaultValue(true);
      item->setToolTip(tr("This switches the Locals&Watchers view to "
			  "automatically derefence pointers. This saves a level in the "
			  "tree view, but also loses data for the now-missing intermediate "
			  "level."));
      instance->insertItem(AutoDerefPointers, item);
      
      //
      // Locals & Watchers
      //
      item = new SavedAction(instance);
      item->setTextPattern(tr("Watch expression \"%1\""));
      instance->insertItem(WatchExpression, item);
      
      item = new SavedAction(instance);
      item->setTextPattern(tr("Remove watch expression \"%1\""));
      instance->insertItem(RemoveWatchExpression, item);
      
      item = new SavedAction(instance);
      item->setTextPattern(tr("Watch expression \"%1\" in separate window"));
      instance->insertItem(WatchExpressionInWindow, item);
      
      item = new SavedAction(instance);
      instance->insertItem(AssignValue, item);
      
      item = new SavedAction(instance);
      instance->insertItem(AssignType, item);
      
      item = new SavedAction(instance);
      instance->insertItem(WatchPoint, item);
      
      //
      // DebuggingHelper
      //
      item = new SavedAction(instance);
      item->setSettingsKey(debugModeGroup, QLatin1String("UseDebuggingHelper"));
      item->setText(tr("Use debugging helper"));
      item->setCheckable(true);
      item->setDefaultValue(true);
      item->setValue(true);
      instance->insertItem(UseDebuggingHelpers, item);
      
      item = new SavedAction(instance);
      item->setSettingsKey(debugModeGroup, QLatin1String("UseCustomDebuggingHelperLocation"));
      item->setCheckable(true);
      item->setDefaultValue(false);
      item->setValue(false);
      instance->insertItem(UseCustomDebuggingHelperLocation, item);
      
      item = new SavedAction(instance);
      item->setSettingsKey(debugModeGroup, QLatin1String("CustomDebuggingHelperLocation"));
      item->setCheckable(true);
      item->setDefaultValue(QString());
      item->setValue(QString());
      instance->insertItem(CustomDebuggingHelperLocation, item);
      
      item = new SavedAction(instance);
      item->setSettingsKey(debugModeGroup, QLatin1String("DebugDebuggingHelpers"));
      item->setText(tr("Debug debugging helper"));
      item->setCheckable(true);
      item->setDefaultValue(false);
      item->setValue(false);
      instance->insertItem(DebugDebuggingHelpers, item);
      
      item = new SavedAction(instance);
      item->setSettingsKey(debugModeGroup, QLatin1String("UseCodeModel"));
      item->setText(tr("Use code model"));
      item->setCheckable(true);
      item->setDefaultValue(true);
      item->setValue(true);
      instance->insertItem(UseCodeModel, item);
      
      item = new SavedAction(instance);
      item->setText(tr("Recheck debugging helper availability"));
      instance->insertItem(RecheckDebuggingHelpers, item);
      
      //
      // Breakpoints
      //
      item = new SavedAction(instance);
      item->setText(tr("Synchronize breakpoints"));
      instance->insertItem(SynchronizeBreakpoints, item);
      
      item = new SavedAction(instance);
      item->setText(tr("Use precise breakpoints"));
      item->setCheckable(true);
      item->setDefaultValue(true);
      item->setValue(true);
      item->setSettingsKey(debugModeGroup, QLatin1String("UsePreciseBreakpoints"));
      instance->insertItem(UsePreciseBreakpoints, item);
      
      //
      // Settings
      //
      item = new SavedAction(instance);
      item->setSettingsKey(debugModeGroup, QLatin1String("Location"));
      item->setDefaultValue("gdb");
      instance->insertItem(GdbLocation, item);
      
      item = new SavedAction(instance);
      item->setSettingsKey(debugModeGroup, QLatin1String("Environment"));
      item->setDefaultValue(QString());
      instance->insertItem(GdbEnvironment, item);
      
      item = new SavedAction(instance);
      item->setSettingsKey(debugModeGroup, QLatin1String("ScriptFile"));
      item->setDefaultValue(QString());
      instance->insertItem(GdbScriptFile, item);
      
      item = new SavedAction(instance);
      item->setSettingsKey(debugModeGroup, QLatin1String("AutoQuit"));
      item->setText(tr("Automatically quit debugger"));
      item->setCheckable(true);
      item->setDefaultValue(false);
      instance->insertItem(AutoQuit, item);
      
      item = new SavedAction(instance);
      item->setSettingsKey(debugModeGroup, QLatin1String("UseToolTips"));
      item->setText(tr("Use tooltips in main editor when debugging"));
      item->setToolTip(tr("Checking this will enable tooltips for variable "
			  "values during debugging. Since this can slow down debugging and "
			  "does not provide reliable information as it does not use scope "
			  "information, it is switched off by default."));
      item->setCheckable(true);
      item->setDefaultValue(false);
      instance->insertItem(UseToolTipsInMainEditor, item);
      
      item = new SavedAction(instance);
      item->setSettingsKey(debugModeGroup, QLatin1String("UseToolTipsInLocalsView"));
      item->setText(tr("Use tooltips in locals view when debugging"));
      item->setToolTip(tr("Checking this will enable tooltips in the locals "
			  "view during debugging."));
      item->setCheckable(true);
      item->setDefaultValue(false);
      instance->insertItem(UseToolTipsInLocalsView, item);
      
      item = new SavedAction(instance);
      item->setSettingsKey(debugModeGroup, QLatin1String("UseToolTipsInBreakpointsView"));
      item->setText(tr("Use tooltips in breakpoints view when debugging"));
      item->setToolTip(tr("Checking this will enable tooltips in the breakpoints "
			  "view during debugging."));
      item->setCheckable(true);
      item->setDefaultValue(false);
      instance->insertItem(UseToolTipsInBreakpointsView, item);
      
      item = new SavedAction(instance);
      item->setSettingsKey(debugModeGroup, QLatin1String("UseAddressInBreakpointsView"));
      item->setText(tr("Show address data in breakpoints view when debugging"));
      item->setToolTip(tr("Checking this will show a column with address "
			  "information in the breakpoint view during debugging."));
      item->setCheckable(true);
      item->setDefaultValue(false);
      instance->insertItem(UseAddressInBreakpointsView, item);
      item = new SavedAction(instance);
      
      item = new SavedAction(instance);
      item->setSettingsKey(debugModeGroup, QLatin1String("UseAddressInStackView"));
      item->setText(tr("Show address data in stack view when debugging"));
      item->setToolTip(tr("Checking this will show a column with address "
			  "information in the stack view during debugging."));
      item->setCheckable(true);
      item->setDefaultValue(false);
      instance->insertItem(UseAddressInStackView, item);
      item = new SavedAction(instance);
      
      item->setSettingsKey(debugModeGroup, QLatin1String("ListSourceFiles"));
      item->setText(tr("List source files"));
      item->setCheckable(true);
      item->setDefaultValue(false);
      instance->insertItem(ListSourceFiles, item);
      
      item = new SavedAction(instance);
      item->setSettingsKey(debugModeGroup, QLatin1String("SkipKnownFrames"));
      item->setText(tr("Skip known frames"));
      item->setCheckable(true);
      item->setDefaultValue(false);
      instance->insertItem(SkipKnownFrames, item);
      
      item = new SavedAction(instance);
      item->setSettingsKey(debugModeGroup, QLatin1String("EnableReverseDebugging"));
      item->setText(tr("Enable reverse debugging"));
      item->setCheckable(true);
      item->setDefaultValue(false);
      instance->insertItem(EnableReverseDebugging, item);
      
      item = new SavedAction(instance);
      item->setSettingsKey(debugModeGroup, QLatin1String("AllPluginBreakpoints"));
      item->setDefaultValue(true);
      instance->insertItem(AllPluginBreakpoints, item);
      
      item = new SavedAction(instance);
      item->setSettingsKey(debugModeGroup, QLatin1String("SelectedPluginBreakpoints"));
      item->setDefaultValue(false);
      instance->insertItem(SelectedPluginBreakpoints, item);
      
      item = new SavedAction(instance);
      item->setSettingsKey(debugModeGroup, QLatin1String("NoPluginBreakpoints"));
      item->setDefaultValue(false);
      instance->insertItem(NoPluginBreakpoints, item);
      
      item = new SavedAction(instance);
      item->setSettingsKey(debugModeGroup, QLatin1String("SelectedPluginBreakpointsPattern"));
      item->setDefaultValue(QLatin1String(".*"));
      instance->insertItem(SelectedPluginBreakpointsPattern, item);
      
      item = new SavedAction(instance);
      item->setSettingsKey(debugModeGroup, QLatin1String("MaximalStackDepth"));
      item->setDefaultValue(20);
      instance->insertItem(MaximalStackDepth, item);
      
      item = new SavedAction(instance);
      item->setText(tr("Reload full stack"));
      instance->insertItem(ExpandStack, item);
      
      item = new SavedAction(instance);
      item->setText(tr("Execute line"));
      instance->insertItem(ExecuteCommand, item);
      
      item = new SavedAction(instance);
      item->setSettingsKey(debugModeGroup, QLatin1String("WatchdogTimeout"));
      item->setDefaultValue(20);
      instance->insertItem(GdbWatchdogTimeout, item);
      
      
      return instance;
    }

    
    //////////////////////////////////////////////////////////////////////////
    //
    // AcquireActions
    //
    //////////////////////////////////////////////////////////////////////////
    
    SavedAction *theAcquireAction(int code)
    {
      return AcquireSettings::instance()->item(code);
    }
    
    bool theAcquireBoolSetting(int code)
    {
      return AcquireSettings::instance()->item(code)->value().toBool();
    }
    
    QString theAcquireStringSetting(int code)
    {
      return AcquireSettings::instance()->item(code)->value().toString();
    }
    
  } // namespace Internal
} // namespace Acquire

