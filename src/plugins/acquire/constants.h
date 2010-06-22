
#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace Acquire {

  namespace Constants {
    const char * const MODE_ACQUIRE = "Acquire.Mode";
    const int P_MODE_ACQUIRE        = 99;
    
    // common actions
    const char * const INTERRUPT            = "Debugger.Interrupt";
    const char * const RESET                = "Debugger.Reset";
    const char * const STEP                 = "Debugger.StepLine";
    const char * const STEPOUT              = "Debugger.StepOut";
    const char * const NEXT                 = "Debugger.NextLine";
    const char * const REVERSE              = "Debugger.ReverseDirection";

    // icon
    const char * const ICON_CONNECT         = ":/acquire/images/debugger_start.png";
    const char * const ICON_CONNECT_SMALL   = ":/acquire/images/debugger_start_small.png";
  }

}

#endif // CONSTANTS_H
