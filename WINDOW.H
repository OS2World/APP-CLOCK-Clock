// Class WINDOW: Encapsulates the startup/shutdown logic for a OS/2-PM window.

#define INCL_BASE
#define INCL_PM
#include <os2.h>

class Window
{
  private:
    HWND Handle ;

  public:
    Window
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
    ) ;

    ~Window ( ) ;

    HWND QueryHandle ()
    {
      return ( Handle ) ;
    }
} ;
