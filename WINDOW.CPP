// Class WINDOW: Encapsulates the startup/shutdown logic for a OS/2-PM window.

#define INCL_BASE
#define INCL_PM
#include <os2.h>

#include "debug.h"

#include "window.h"

  // Constructor

Window::Window
(
  HWND Parent,
  PSZ ClassName,
  PSZ Title,
  ULONG Style,
  LONG x,
  LONG y,
  LONG cx,
  LONG cy,
  HWND Owner,
  HWND Zorder,
  ULONG Id,
  PVOID Parms,
  PVOID PresParams
)
{
  Handle = WinCreateWindow
  (
    Parent,
    ClassName,
    Title,
    Style,
    x, y, cx, cy,
    Owner,
    Zorder,
    Id,
    Parms,
    PresParams
  ) ;
}


  // Destructor

Window::~Window ( )
{
  WinSendMsg ( Handle, WM_SAVEAPPLICATION, 0, 0 ) ;
  WinDestroyWindow ( Handle ) ;
}
