// Class HELPWIN: Encapsulates the startup/shutdown logic for an OS/2-PM help window.

#define INCL_WIN
#include <os2.h>

class HelpWindow
{
  private:
    HWND Handle ;

  public:
    HelpWindow ( HAB Anchor, HMODULE Module, USHORT Id, PSZ Filename, PSZ Title ) ;
    ~HelpWindow ( ) ;
    HWND QueryHandle () { return ( Handle ) ; }
} ;