// Class HELPWIN: Encapsulates the startup/shutdown logic for an OS/2-PM help window.

#define INCL_BASE
#define INCL_PM
#include <os2.h>

#include <string.h>

#include "debug.h"

#include "helpwin.h"

  // Constructor

HelpWindow::HelpWindow ( HAB Anchor, HMODULE Module, USHORT Id, PSZ Filename, PSZ Title ) {
  HELPINIT HelpInit ;

  memset ( &HelpInit, 0, sizeof(HelpInit) ) ;

  HelpInit.cb = sizeof(HelpInit) ;
  HelpInit.phtHelpTable = (PHELPTABLE)MAKEULONG(Id,0xFFFF);
  HelpInit.hmodHelpTableModule = Module ;
  HelpInit.pszHelpWindowTitle = Title ;
  HelpInit.fShowPanelId = CMIC_HIDE_PANEL_ID ;
  HelpInit.pszHelpLibraryName = Filename ;

  Handle = WinCreateHelpInstance ( Anchor, &HelpInit ) ;
}

  // Destructor

HelpWindow::~HelpWindow ( ) {
  if ( Handle ) {
    WinDestroyHelpInstance ( Handle ) ;
  }
}
