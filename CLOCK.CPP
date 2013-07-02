/****************************************************************** CLOCK.CPP
 *                                                                          *
 *                Analog Clock for Presentation Manager                     *
 *                                                                          *
 *                Original program by Charles Petzold                       *
 *                                                                          *
 ****************************************************************************/

//
// Things to do:
//
//   (1) Show Date (optional)
//

#define INCL_BASE
#define INCL_PM
#include <os2.h>

#include <process.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "helpwin.h"
#include "module.h"
#include "process.h"
#include "profile.h"
#include "restring.h"
#include "support.h"
#include "window.h"

#include "about.h"
#include "config.h"

#include "clock.h"


/****************************************************************************
 *                                                                          *
 *                       Definitions & Declarations                         *
 *                                                                          *
 ****************************************************************************/

  // Constants

#define DATEFMT_MM_DD_YY    (0x0000)
#define DATEFMT_DD_MM_YY    (0x0001)
#define DATEFMT_YY_MM_DD    (0x0002)

enum { ALERT_TASKCOUNT, ALERT_LOAD } ;

#define WM_REFRESH WM_USER


  // Type Definitions

typedef struct {
  SHORT cxClient ;
  SHORT cyClient ;
  SHORT cxPixelDiam ;
  SHORT cyPixelDiam ;
} WINDOWINFO, *PWINDOWINFO ;

typedef struct {
  volatile BOOL Active ;
  volatile PULONG Counter ;
  HWND Owner ;
} MONITOR_PARMS, *PMONITOR_PARMS ;

typedef struct {
  volatile BOOL Active ;
  volatile PULONG Counter ;
} COUNTER_PARMS, *PCOUNTER_PARMS ;

typedef struct {      // Parameters saved to system.

  SWP    Position ;             // Window size & location.
  BOOL   fPosition ;

  BOOL   Hour24 ;               // User options.
  BOOL   fHour24 ;

  BOOL   HideControls ;
  BOOL   fHideControls ;

  BOOL   Chime ;
  BOOL   fChime ;

  BOOL   Float ;
  BOOL   fFloat ;

  BOOL   Animate ;
  BOOL   fAnimate ;

  BOOL   Seconds ;
  BOOL   fSeconds ;

  BOOL   Analog ;
  BOOL   fAnalog ;

  BOOL   AlertType ;
  BOOL   fAlertType ;

  USHORT AlertLevels [2] [2] ;
  BOOL   fAlertLevels ;

  CHAR   FontNameSize [80] ;    // Presentation Parameters
  BOOL   fFontNameSize ;

  COLOR  BackColor ;
  BOOL   fBackColor ;

  COLOR  TextColor ;
  BOOL   fTextColor ;
} INIDATA, *PINIDATA ;

typedef struct {

  Process *Proc ;
  Module  *Library ;
  Profile *IniFile ;

  INIDATA IniData ;

  HWND hwndTitleBar ;
  HWND hwndSysMenu ;
  HWND hwndMinMax ;
  HWND Menu ;

  DATETIME PreviousDateTime ;
  USHORT PreviousHour ;
  LONG xPixelsPerMeter, yPixelsPerMeter ;
  WINDOWINFO wi ;

  USHORT Alert ;

  ULONG MaxCount ;
  ULONG IdleCounter ;
  ULONG IdleCount ;

  TID   IdleLoopTID ;
  COUNTER_PARMS IdleLoopParms ;

  TID   MonitorTID ;
  MONITOR_PARMS MonitorParms ;

  COUNTRYINFO CountryInfo ;
  char szAm [3] ;
  char szPm [3] ;

} DATA, *PDATA ;

typedef struct {
   short Filler ;
   Process *Proc ;
   Module *Library ;
   Profile *IniFile ;
   BOOL Analog ;
   BOOL fAnalog ;
} PARMS, *PPARMS ;


  // Function Prototypes

extern int main ( int argc, char *argv[] ) ;

static FNWP MessageProcessor ;

static FNWP Create ;
static FNWP Destroy ;
static FNWP Size ;
static FNWP SaveApplication ;
static FNWP Paint ;
static FNWP InitMenu ;
static FNWP Command ;
static FNWP ResetDefaults ;
static FNWP HideControls ;
static FNWP Configure ;
static FNWP About ;
static FNWP ButtonDown ;
static FNWP ButtonDblClick ;
static FNWP ContextMenu ;
static FNWP PresParamChanged ;
static FNWP SysColorChange ;
static FNWP QueryKeysHelp ;
static FNWP HelpError ;
static FNWP ExtHelpUndefined ;
static FNWP HelpSubitemNotFound ;
static FNWP Refresh ;

static int GetIniData ( HINI IniHandle, PINIDATA IniData ) ;
static VOID PutIniData ( HINI IniHandle, PINIDATA IniData ) ;

static VOID RotatePoint ( POINTL aptl[],  SHORT sNum, SHORT sAngle ) ;
static VOID ScalePoint ( POINTL aptl[], SHORT sNum, PWINDOWINFO pwi ) ;
static VOID TranslatePoint ( POINTL aptl[], SHORT sNum, PWINDOWINFO pwi ) ;
static VOID DrawHand ( HPS hPS, POINTL aptlIn[], SHORT sNum, SHORT sAngle,
  PWINDOWINFO pwi ) ;
static void PaintBackground ( HWND hwnd, HPS hPS, PDATA Data, BOOL MustPaint ) ;
static void PaintBorder ( HWND hwnd, HPS hPS, PDATA Data, BOOL MustPaint ) ;
static void PaintDigitalTime ( HWND hwnd, HPS hPS, PDATA Data, PDATETIME DateTime ) ;

static void HideControls
(
  BOOL fHide,
  HWND hwndFrame,
  HWND hwndSysMenu,
  HWND hwndTitleBar,
  HWND hwndMinMax
) ;

static void _Optlink MonitorThread ( PVOID Parameter ) ;

static ULONG CalibrateLoadMeter ( void ) ;

static void _Optlink CounterThread ( PVOID Parameter ) ;


  // Global Data

static Process Proc ;                   // Must be declared first.
static Module Library ( PSZ(PROGRAM_NAME) ) ;

HMODULE LibraryHandle ;


/****************************************************************************
 *                                                                          *
 *      Program Mainline                                                    *
 *                                                                          *
 ****************************************************************************/

extern int main ( int argc, char *argv[] ) {

 /***************************************************************************
  * Save the resource library handle for use by the exception handler.      *
  ***************************************************************************/

  LibraryHandle = Library.QueryHandle() ;

 /***************************************************************************
  * Set the codepage.  Abort if unable to do so.                            *
  ***************************************************************************/

  PVOID Offset ;
  DosGetResource ( Library.QueryHandle(), RT_RCDATA, 1, &Offset ) ;
 
  if ( !WinSetCp ( Proc.QueryQueue(), *((int*)Offset) ) ) {
     Debug ( HWND_DESKTOP, "ERROR: Unable to set code page %i.", *((int*)Offset) ) ;
     return ( 1 ) ;
  } /* endif */

 /***************************************************************************
  * Get the program title.                                                  *
  ***************************************************************************/

  ResourceString Title ( Library.QueryHandle(), IDS_TITLE ) ;

 /***************************************************************************
  * Decipher command-line parameters.                                       *
  ***************************************************************************/

  ResourceString ResetCommand ( Library.QueryHandle(), IDS_PARMS_RESET ) ;
  ResourceString AnalogCommand ( Library.QueryHandle(), IDS_PARMS_ANALOG ) ;
  ResourceString DigitalCommand ( Library.QueryHandle(), IDS_PARMS_DIGITAL ) ;

  BOOL Reset = FALSE ;
  BOOL Analog = TRUE ;
  BOOL fAnalog = FALSE ;

  while ( --argc ) {

    argv ++ ;

    if ( *argv[0] == '?' ) {
      ResourceString Message ( Library.QueryHandle(), IDS_PARAMETERLIST ) ;
      WinMessageBox ( HWND_DESKTOP, HWND_DESKTOP, PSZ(Message),
        PSZ(Title), 0, MB_ENTER | MB_NOICON ) ;
      return ( 0 ) ;
    }

    if ( !stricmp ( *argv, PCHAR(ResetCommand) ) ) {
      Reset = TRUE ;
      continue ;
    }

    if ( !stricmp ( *argv, PCHAR(AnalogCommand) ) ) {
      Analog = TRUE ;
      fAnalog = TRUE ;
      continue ;
    }

    if ( !stricmp ( *argv, PCHAR(DigitalCommand) ) ) {
      Analog = FALSE ;
      fAnalog = TRUE ;
      continue ;
    }

    ResourceString Format ( Library.QueryHandle(), IDS_ERROR_INVALIDPARM ) ;
    BYTE Message [200] ;
    sprintf ( PCHAR(Message), PCHAR(Format), *argv ) ;
    WinMessageBox ( HWND_DESKTOP, HWND_DESKTOP, Message,
      PSZ(Title), 0, MB_ENTER | MB_ICONEXCLAMATION ) ;
    return ( 1 ) ;
  }

 /***************************************************************************
  * Create the help instance.                                               *
  ***************************************************************************/

  ResourceString HelpTitle ( Library.QueryHandle(), IDS_HELPTITLE ) ;

  HelpWindow Help ( Proc.QueryAnchor(), 0,
    ID_MAIN, PSZ(PROGRAM_NAME ".hlp"), PSZ(HelpTitle) ) ;

  if ( Help.QueryHandle() == 0 ) {
    ERRORID Error = WinGetLastError ( Proc.QueryAnchor() ) ;
    ResourceString Format ( Library.QueryHandle(), IDS_ERROR_CREATEHELP ) ;
    CHAR Message [200] ;
    sprintf ( Message, PCHAR(Format), Error ) ;
    WinMessageBox ( HWND_DESKTOP, HWND_DESKTOP, PSZ(Message),
      PSZ(Title), 0, MB_ENTER | MB_ICONEXCLAMATION ) ;
  }

 /***************************************************************************
  * Open/create the profile file.  Reset if requested.                      *
  ***************************************************************************/

  Profile IniFile ( PSZ(PROGRAM_NAME),
    Proc.QueryAnchor(), Library.QueryHandle(),
    IDD_PROFILE_PATH, Help.QueryHandle(), Reset ) ;

  if ( IniFile.QueryHandle() == 0 ) {
    ResourceString Message ( Library.QueryHandle(), IDS_ERROR_PRFOPENPROFILE ) ;
    Log ( "%s", PSZ(Message) ) ;
    WinMessageBox ( HWND_DESKTOP, HWND_DESKTOP, PSZ(Message),
      PSZ(Title), 0, MB_ENTER | MB_ICONEXCLAMATION ) ;
    return ( 2 ) ;
  }

 /***************************************************************************
  * Read the profile to find out if we're to animate the frame window.      *
  ***************************************************************************/

  BOOL Animate = FALSE ;
  ULONG Size ;
  if
  (
    PrfQueryProfileSize ( IniFile.QueryHandle(), PSZ(PROGRAM_NAME), PSZ("Animate"), &Size )
    AND
    ( ( Size == sizeof(Animate) ) OR ( Size == sizeof(short) ) )
    AND
    PrfQueryProfileData ( IniFile.QueryHandle(), PSZ(PROGRAM_NAME), PSZ("Animate"), &Animate, &Size )
  )
  {
    ;
  }

 /***************************************************************************
  * Create the frame window.                                                *
  ***************************************************************************/

  FRAMECDATA FrameControlData ;
  FrameControlData.cb = sizeof(FrameControlData) ;
  FrameControlData.flCreateFlags =
    FCF_TITLEBAR | FCF_SYSMENU | FCF_SIZEBORDER |
    FCF_MINMAX | FCF_NOBYTEALIGN | FCF_ACCELTABLE ;
  FrameControlData.hmodResources = 0 ;
  FrameControlData.idResources = ID_MAIN ;

  Window Frame ( HWND_DESKTOP, WC_FRAME, PSZ(Title),
    Animate ? WS_ANIMATE : 0,
    0, 0, 0, 0, HWND_DESKTOP, HWND_TOP, ID_MAIN,
    &FrameControlData, NULL ) ;

  if ( Frame.QueryHandle() == 0 ) {
    ERRORID Error = WinGetLastError ( Proc.QueryAnchor() ) ;
    ResourceString Format ( Library.QueryHandle(), IDS_ERROR_CREATEFRAME ) ;
    CHAR Message [200] ;
    sprintf ( Message, PCHAR(Format), Error ) ;
    Log ( "%s", Message ) ;
    WinMessageBox ( HWND_DESKTOP, HWND_DESKTOP, PSZ(Message),
      PSZ(Title), 0, MB_ENTER | MB_ICONEXCLAMATION ) ;
    return ( 3 ) ;
  }

 /***************************************************************************
  * Associate the help instance with the frame window.                      *
  ***************************************************************************/

  if ( Help.QueryHandle() ) {
    WinAssociateHelpInstance ( Help.QueryHandle(), Frame.QueryHandle() ) ;
  }

 /***************************************************************************
  * Register the client window class.                                       *
  ***************************************************************************/

  if
  (
    !WinRegisterClass
    (
      Proc.QueryAnchor(),
      PSZ(CLASS_NAME),
      MessageProcessor,
      CS_MOVENOTIFY | CS_SIZEREDRAW,
      sizeof(PVOID)
    )
  )
  {
    ERRORID Error = WinGetLastError ( Proc.QueryAnchor() ) ;
    ResourceString Format ( Library.QueryHandle(), IDS_ERROR_WINREGISTERCLASS ) ;
    CHAR Message [200] ;
    sprintf ( Message, PCHAR(Format), CLASS_NAME, Error ) ;
    Log ( "%s", Message ) ;
    WinMessageBox ( HWND_DESKTOP, HWND_DESKTOP, PSZ(Message),
      PSZ(Title), 0, MB_ENTER | MB_ICONEXCLAMATION ) ;
    return ( 4 ) ;
  }

 /***************************************************************************
  * Create client window.  If this fails, destroy frame and return.         *
  ***************************************************************************/

  PARMS Parms ;
  Parms.Filler = 0 ;
  Parms.Proc = & Proc ;
  Parms.Library = & Library ;
  Parms.IniFile = & IniFile ;
  Parms.Analog = Analog ;
  Parms.fAnalog = fAnalog ;

  Window Client ( Frame.QueryHandle(), PSZ(CLASS_NAME), PSZ(""), 0, 0, 0, 0, 0,
     Frame.QueryHandle(), HWND_BOTTOM, FID_CLIENT, &Parms, NULL ) ;

  if ( Client.QueryHandle() == 0 ) {
    ERRORID Error = WinGetLastError ( Proc.QueryAnchor() ) ;
    ResourceString Format ( Library.QueryHandle(), IDS_ERROR_CREATECLIENT ) ;
    CHAR Message [200] ;
    sprintf ( Message, PCHAR(Format), Error ) ;
    Log ( "%s", Message ) ;
    WinMessageBox ( HWND_DESKTOP, HWND_DESKTOP, PSZ(Message),
      PSZ(Title), 0, MB_ENTER | MB_ICONEXCLAMATION ) ;
    return ( 5 ) ;
  }

 /***************************************************************************
  * Wait for and process messages to the window's queue.  Terminate         *
  *   when the WM_QUIT message is received.                                 *
  ***************************************************************************/

  QMSG QueueMessage ;
  while ( WinGetMsg ( Proc.QueryAnchor(), &QueueMessage, 0, 0, 0 ) ) {
    WinDispatchMsg ( Proc.QueryAnchor(), &QueueMessage ) ;
  }

 /***************************************************************************
  * Discard all that was requested of the system and terminate.             *
  ***************************************************************************/

  return ( 0 ) ;
}

/****************************************************************************
 *                                                                          *
 *      Window Message Processor                                            *
 *                                                                          *
 ****************************************************************************/

static MRESULT EXPENTRY MessageProcessor ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

 /***************************************************************************
  * Dispatch the message according to the method table and return the       *
  *   result.  Any messages not defined above get handled by the system     *
  *   default window processor.                                             *
  ***************************************************************************/

  static METHOD Methods [] = {
    { WM_CREATE,                Create              },
    { WM_DESTROY,               Destroy             },
    { WM_SIZE,                  Size                },
    { WM_MOVE,                  Size                },
    { WM_SAVEAPPLICATION,       SaveApplication     },
    { WM_PAINT,                 Paint               },
    { WM_INITMENU,              InitMenu            },
    { WM_COMMAND,               Command             },
    { WM_BUTTON1DOWN,           ButtonDown          },
    { WM_BUTTON1DBLCLK,         ButtonDblClick      },
    { WM_CONTEXTMENU,           ContextMenu         },
    { WM_PRESPARAMCHANGED,      PresParamChanged    },
    { WM_SYSCOLORCHANGE,        SysColorChange      },
    { HM_QUERY_KEYS_HELP,       QueryKeysHelp       },
    { HM_ERROR,                 HelpError           },
    { HM_EXT_HELP_UNDEFINED,    ExtHelpUndefined    },
    { HM_HELPSUBITEM_NOT_FOUND, HelpSubitemNotFound },
    { WM_REFRESH,               Refresh             }
  } ;

  return ( DispatchMessage ( hwnd, msg, mp1, mp2, Methods, sizeof(Methods)/sizeof(Methods[0]), WinDefWindowProc ) ) ;
}

/****************************************************************************
 *                                                                          *
 *      Initialize main window.                                             *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Create ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

 /***************************************************************************
  * Allocate instance data.                                                 *
  ***************************************************************************/

  PDATA Data = PDATA ( malloc ( sizeof(DATA) ) ) ;

  memset ( Data, 0, sizeof(DATA) ) ;

  WinSetWindowPtr ( hwnd, QWL_USER, Data ) ;

 /***************************************************************************
  * Grab any parameters from the WM_CREATE message.                         *
  ***************************************************************************/

  PPARMS Parms = PPARMS ( PVOIDFROMMP ( mp1 ) ) ;

  Data->Proc = Parms->Proc ;
  Data->Library = Parms->Library ;
  Data->IniFile = Parms->IniFile ;

 /***************************************************************************
  * Load the window context menu.                                           *
  ***************************************************************************/

  Data->Menu = WinLoadMenu ( HWND_DESKTOP, Data->Library->QueryHandle(), IDM_MENU ) ;

 /***************************************************************************
  * Get profile data. Try the OS2.INI first, then try for private INI.      *
  *   If obtained from OS2.INI, erase it afterwards.                        *
  ***************************************************************************/

  if ( GetIniData ( HINI_USERPROFILE, &Data->IniData ) ) {
     GetIniData ( Data->IniFile->QueryHandle(), &Data->IniData ) ;
  } else {
     PrfWriteProfileData ( HINI_USERPROFILE, PSZ(PROGRAM_NAME), PSZ(NULL), PSZ(NULL), 0 ) ;
  }

  if ( Parms->fAnalog ) {
     Data->IniData.Analog = Parms->Analog ;
     Data->IniData.fAnalog = TRUE ;
  }

 /***************************************************************************
  * Get country information.                                                *
  ***************************************************************************/

  COUNTRYCODE CountryCode ;
  ULONG Count ;
  ULONG Status ;

  CountryCode.country = 0 ;
  CountryCode.codepage = 0 ;

  Status = DosGetCtryInfo ( sizeof(Data->CountryInfo), &CountryCode,
     &Data->CountryInfo, &Count ) ;
  if ( Status ) {
     BYTE Message [80] ;
     WinLoadMessage ( Data->Proc->QueryAnchor(), Data->Library->QueryHandle(), IDS_ERROR_DOSGETCTRYINFO,
        sizeof(Message), Message ) ;
     Debug ( hwnd, PCHAR(Message), Status ) ;
     Data->CountryInfo.fsDateFmt = DATEFMT_MM_DD_YY ;
     Data->CountryInfo.fsTimeFmt = 0 ;
     Data->CountryInfo.szDateSeparator[0] = '/' ;
     Data->CountryInfo.szDateSeparator[1] = 0 ;
     Data->CountryInfo.szTimeSeparator[0] = ':' ;
     Data->CountryInfo.szTimeSeparator[1] = 0 ;
  }

  char Text [2] ;
  ULONG Size = 2 ;
  if ( PrfQueryProfileData ( HINI_USERPROFILE, PSZ("PM_National"), PSZ("iDate"), Text, &Size ) ) {
     Data->CountryInfo.fsDateFmt = atoi ( Text ) ;
  } /* endif */

  Size = 2 ;
  if ( PrfQueryProfileData ( HINI_USERPROFILE, PSZ("PM_National"), PSZ("iTime"), Text, &Size ) ) {
     Data->CountryInfo.fsTimeFmt = UCHAR ( atoi ( Text ) ) ;
  } /* endif */

  Size = 2 ;
  PrfQueryProfileData ( HINI_USERPROFILE, PSZ("PM_National"), PSZ("sDate"), Data->CountryInfo.szDateSeparator, &Size ) ;

  Size = 2 ;
  PrfQueryProfileData ( HINI_USERPROFILE, PSZ("PM_National"), PSZ("sTime"), Data->CountryInfo.szTimeSeparator, &Size ) ;

  Size = 3 ;
  strcpy ( Data->szAm, "am" ) ;
  PrfQueryProfileData ( HINI_USERPROFILE, PSZ("PM_National"), PSZ("s1159"), Data->szAm, &Size ) ;

  Size = 3 ;
  strcpy ( Data->szPm, "pm" ) ;
  PrfQueryProfileData ( HINI_USERPROFILE, PSZ("PM_National"), PSZ("s2359"), Data->szPm, &Size ) ;

  if ( NOT Data->IniData.fHour24 ) {
    Data->IniData.Hour24 = Data->CountryInfo.fsTimeFmt ;
  }

 /***************************************************************************
  * Get initial time.                                                       *
  ***************************************************************************/

  DosGetDateTime ( &Data->PreviousDateTime ) ;

  Data->PreviousHour = USHORT ( ( Data->PreviousDateTime.hours * 5 ) % 60
    + Data->PreviousDateTime.minutes/12 ) ;

 /***************************************************************************
  * Get the frame handle.                                                   *
  ***************************************************************************/

  HWND FrameWindow = WinQueryWindow ( hwnd, QW_PARENT ) ;

 /***************************************************************************
  * Get the control window handles.                                         *
  ***************************************************************************/

  Data->hwndSysMenu  = WinWindowFromID ( FrameWindow, FID_SYSMENU  ) ;
  Data->hwndTitleBar = WinWindowFromID ( FrameWindow, FID_TITLEBAR ) ;
  Data->hwndMinMax   = WinWindowFromID ( FrameWindow, FID_MINMAX   ) ;

 /***************************************************************************
  * Add basic extensions to the system menu.                                *
  ***************************************************************************/

  static MENUITEM MenuSeparator =
    { MIT_END, MIS_SEPARATOR, 0, 0, 0, 0 } ;

  AddSysMenuItem ( FrameWindow, &MenuSeparator, PSZ(NULL) ) ;

  static MENUITEM MenuItems [] = {
    { MIT_END, MIS_TEXT,      0, IDM_SAVE_APPLICATION, 0, 0 },
    { MIT_END, MIS_TEXT,      0, IDM_RESET_DEFAULTS,   0, 0 },
    { MIT_END, MIS_TEXT,      0, IDM_HIDE_CONTROLS,    0, 0 },
    { MIT_END, MIS_TEXT,      0, IDM_CONFIGURE,        0, 0 },
  } ;

  for ( int i=0; i<sizeof(MenuItems)/sizeof(MenuItems[0]); i++ )
  {
    ResourceString MenuText ( Data->Library->QueryHandle(), i+IDS_SAVE_APPLICATION ) ;
    AddSysMenuItem ( FrameWindow, MenuItems+i, PSZ(MenuText) ) ;
  }

 /***************************************************************************
  * Add 'About' to the system menu.                                         *
  ***************************************************************************/

  AddSysMenuItem ( FrameWindow, &MenuSeparator, PSZ(NULL) ) ;

  ResourceString AboutText ( Data->Library->QueryHandle(), IDS_ABOUT ) ;

  static MENUITEM MenuAbout =
    { MIT_END, MIS_TEXT, 0, IDM_ABOUT, 0, 0 } ;

  AddSysMenuItem ( FrameWindow, &MenuAbout, PSZ(AboutText) ) ;

 /***************************************************************************
  * Add 'Help' to the system menu.                                          *
  ***************************************************************************/

  ResourceString HelpText ( Data->Library->QueryHandle(), IDS_HELP ) ;

  static MENUITEM MenuHelp =
    { MIT_END, MIS_HELP, 0, 0, 0, 0 } ;

  AddSysMenuItem ( FrameWindow, &MenuHelp, PSZ(HelpText) ) ;

 /***************************************************************************
  * Calibrate the old-style load meter, if the high resolution timer's      *
  *   available.                                                            *
  ***************************************************************************/

  Data->MaxCount = CalibrateLoadMeter ( ) ;
  Data->MaxCount = (ULONG) max ( 1L, Data->MaxCount ) ;

 /***************************************************************************
  * Start the load meter.  Put it to sleep if we're not using it.           *
  ***************************************************************************/

  Data->IdleLoopParms.Active = TRUE ;
  Data->IdleLoopParms.Counter = & Data->IdleCounter ;
  Data->IdleLoopTID = _beginthread ( CounterThread, NULL, 0x3000, &Data->IdleLoopParms ) ;
  DosSetPriority ( PRTYS_THREAD, PRTYC_IDLETIME, PRTYD_MINIMUM, Data->IdleLoopTID ) ;
  DosSuspendThread ( Data->IdleLoopTID ) ;

  Data->IdleCount = 0 ;
  Data->IdleCounter = 0 ;

  if ( Data->IniData.AlertType == ALERT_LOAD )
  {
    DosResumeThread ( Data->IdleLoopTID ) ;
  }

  Data->MonitorParms.Active = TRUE ;
  Data->MonitorParms.Counter = & Data->IdleCounter ;
  Data->MonitorParms.Owner = hwnd ;
  Data->MonitorTID = _beginthread ( MonitorThread, NULL, 0x3000, &Data->MonitorParms ) ;

 /***************************************************************************
  * Add the program to the system task list.                                *
  ***************************************************************************/

  ResourceString Title ( Data->Library->QueryHandle(), IDS_TITLE ) ;
  Add2TaskList ( FrameWindow, PSZ(Title) ) ;

 /***************************************************************************
  * Get device capabilities before you mess around with resizing.           *
  ***************************************************************************/

  HPS hPS = WinGetPS ( hwnd ) ;
  HDC hDC = GpiQueryDevice ( hPS ) ;
  DevQueryCaps ( hDC, CAPS_VERTICAL_RESOLUTION, 1L,
    &Data->yPixelsPerMeter ) ;
  DevQueryCaps ( hDC, CAPS_HORIZONTAL_RESOLUTION, 1L,
    &Data->xPixelsPerMeter ) ;
  WinReleasePS ( hPS ) ;

 /***************************************************************************
  * If window hasn't been positioned before, set the default position.      *
  ***************************************************************************/

  RECTL Rectangle ;
  if ( NOT Data->IniData.fPosition )
  {
    WinQueryWindowRect ( HWND_DESKTOP, &Rectangle ) ;

    Rectangle.xLeft = Rectangle.xRight - Rectangle.xRight / 4 ;
    Rectangle.yBottom = Rectangle.yTop - Rectangle.yTop / 3 ;

    Data->IniData.Position.x = (SHORT) Rectangle.xLeft ;
    Data->IniData.Position.y = (SHORT) Rectangle.yBottom ;

    Data->IniData.Position.cx = (SHORT) ( Rectangle.xRight - Rectangle.xLeft ) ;
    Data->IniData.Position.cy = (SHORT) ( Rectangle.yTop - Rectangle.yBottom ) ;
  }

 /***************************************************************************
  * Position & size the window.                                             *
  ***************************************************************************/

  Rectangle.xLeft   = Data->IniData.Position.x ;
  Rectangle.xRight  = Data->IniData.Position.x + Data->IniData.Position.cx ;
  Rectangle.yBottom = Data->IniData.Position.y ;
  Rectangle.yTop    = Data->IniData.Position.y + Data->IniData.Position.cy ;

  if ( Data->IniData.HideControls )
  {
    WinSetParent ( Data->hwndSysMenu,  HWND_OBJECT, FALSE ) ;
    WinSetParent ( Data->hwndTitleBar, HWND_OBJECT, FALSE ) ;
    WinSetParent ( Data->hwndMinMax,   HWND_OBJECT, FALSE ) ;

    WinSendMsg ( FrameWindow, WM_UPDATEFRAME,
      MPFROMSHORT ( FCF_TITLEBAR | FCF_SYSMENU | FCF_MINBUTTON ), 0L ) ;

    WinCalcFrameRect ( FrameWindow, &Rectangle, TRUE ) ;

    WinSetParent ( Data->hwndSysMenu,  FrameWindow, TRUE ) ;
    WinSetParent ( Data->hwndTitleBar, FrameWindow, TRUE ) ;
    WinSetParent ( Data->hwndMinMax,   FrameWindow, TRUE ) ;

    WinSendMsg ( FrameWindow, WM_UPDATEFRAME,
      MPFROMSHORT ( FCF_TITLEBAR | FCF_SYSMENU | FCF_MINBUTTON ), 0L ) ;

    WinCalcFrameRect ( FrameWindow, &Rectangle, FALSE ) ;
  }

  WinSetWindowPos ( FrameWindow, HWND_BOTTOM,
    (SHORT) Rectangle.xLeft, (SHORT) Rectangle.yBottom,
    (SHORT) ( Rectangle.xRight - Rectangle.xLeft ),
    (SHORT) ( Rectangle.yTop - Rectangle.yBottom ),
    SWP_SIZE | SWP_MOVE | SWP_ZORDER |
    ( Data->IniData.Position.fl & SWP_MINIMIZE ) |
    ( Data->IniData.Position.fl & SWP_MAXIMIZE ) |
    ( Data->IniData.Position.fl & SWP_RESTORE ) ) ;

 /***************************************************************************
  * Hide the controls if so configured and not minimized.                   *
  ***************************************************************************/

  if ( Data->IniData.HideControls AND NOT ( Data->IniData.Position.fl & SWP_MINIMIZE ) ) {

    HideControls
    (
      TRUE,
      FrameWindow,
      Data->hwndSysMenu,
      Data->hwndTitleBar,
      Data->hwndMinMax
    ) ;
  }

 /***************************************************************************
  * Get the saved presentation parameters and reinstate them.               *
  ***************************************************************************/

  if ( Data->IniData.fFontNameSize ) {
    WinSetPresParam ( hwnd, PP_FONTNAMESIZE,
      strlen(Data->IniData.FontNameSize)+1, Data->IniData.FontNameSize ) ;
  }

  if ( Data->IniData.fBackColor ) {
    WinSetPresParam ( hwnd, PP_BACKGROUNDCOLOR,
      sizeof(Data->IniData.BackColor), &Data->IniData.BackColor ) ;
  }

  if ( Data->IniData.fTextColor ) {
    WinSetPresParam ( hwnd, PP_FOREGROUNDCOLOR,
      sizeof(Data->IniData.TextColor), &Data->IniData.TextColor ) ;
  }

 /***************************************************************************
  * Now that the window's in order, make it visible.                        *
  ***************************************************************************/

  WinShowWindow ( FrameWindow, TRUE ) ;

 /***************************************************************************
  * Success?  Return no error.                                              *
  ***************************************************************************/

  return ( MRFROMSHORT ( 0 ) ) ;
}

/****************************************************************************
 *                                                                          *
 *      Destroy main window.                                                *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Destroy ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PDATA Data = PDATA ( WinQueryWindowPtr ( hwnd, QWL_USER ) ) ;

 /***************************************************************************
  * Kill the extra threads.                                                 *
  ***************************************************************************/

  DosResumeThread ( Data->MonitorTID ) ;
  Data->MonitorParms.Active = FALSE ;
  DosWaitThread ( &Data->MonitorTID, DCWW_WAIT ) ;

  DosResumeThread ( Data->IdleLoopTID ) ;
  Data->IdleLoopParms.Active = FALSE ;
  DosSetPriority ( PRTYS_THREAD, PRTYC_TIMECRITICAL, PRTYD_MAXIMUM, Data->IdleLoopTID ) ;
  DosWaitThread ( &Data->IdleLoopTID, DCWW_WAIT ) ;

 /***************************************************************************
  * Release the instance memory.                                            *
  ***************************************************************************/

  free ( Data ) ;

 /***************************************************************************
  * We're done.                                                             *
  ***************************************************************************/

  return ( MRFROMSHORT ( 0 ) ) ;
}

/****************************************************************************
 *                                                                          *
 *      Resize the main window.                                             *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Size ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PDATA Data = PDATA ( WinQueryWindowPtr ( hwnd, QWL_USER ) ) ;

 /***************************************************************************
  * Find out the window's new position and size.                            *
  ***************************************************************************/

  HWND FrameWindow = WinQueryWindow ( hwnd, QW_PARENT ) ;

  SWP Position ;
  WinQueryWindowPos ( FrameWindow, &Position ) ;

  if ( NOT ( Position.fl & SWP_MINIMIZE )
    AND NOT ( Position.fl & SWP_MAXIMIZE ) )
  {
    Data->IniData.Position.x = Position.x ;
    Data->IniData.Position.y = Position.y ;

    Data->IniData.Position.cx = Position.cx ;
    Data->IniData.Position.cy = Position.cy ;
  }

 /***************************************************************************
  * If the message was a sizing notification, recompute scaling factors.    *
  ***************************************************************************/

  if ( msg == WM_SIZE )
  {
    Data->wi.cxClient = SHORT1FROMMP ( mp2 ) ;
    Data->wi.cyClient = SHORT2FROMMP ( mp2 ) ;

    SHORT sDiamMM = (SHORT) min ( Data->wi.cxClient*1000L/Data->xPixelsPerMeter,
      Data->wi.cyClient*1000L/Data->yPixelsPerMeter ) ;

    Data->wi.cxPixelDiam = (SHORT) ( Data->xPixelsPerMeter * sDiamMM / 1000 ) ;
    Data->wi.cyPixelDiam = (SHORT) ( Data->yPixelsPerMeter * sDiamMM / 1000 ) ;
  }

 /***************************************************************************
  * If hiding the controls . . .                                            *
  ***************************************************************************/

  if ( Data->IniData.HideControls )
  {

   /*************************************************************************
    * If changing to or from minimized state . . .                          *
    *************************************************************************/

    if ( ( Position.fl & SWP_MINIMIZE ) != ( Data->IniData.Position.fl & SWP_MINIMIZE ) )
    {

     /***********************************************************************
      * Hide the controls if no longer minimized.                           *
      ***********************************************************************/

      HideControls
      (
        NOT ( Position.fl & SWP_MINIMIZE ),
        FrameWindow,
        Data->hwndSysMenu,
        Data->hwndTitleBar,
        Data->hwndMinMax
      ) ;
    }
  }

  Data->IniData.Position.fl = Position.fl ;

 /***************************************************************************
  * We're done.                                                             *
  ***************************************************************************/

  return ( 0 ) ;
}

/****************************************************************************
 *                                                                          *
 *      Process SAVE APPLICATION message.                                   *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY SaveApplication ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PDATA Data = PDATA ( WinQueryWindowPtr ( hwnd, QWL_USER ) ) ;

 /***************************************************************************
  * Call function to put all profile data out to the system.                *
  ***************************************************************************/

  PutIniData ( Data->IniFile->QueryHandle(), &Data->IniData ) ;

 /***************************************************************************
  * We're done.  Let the system complete default processing.                *
  ***************************************************************************/

  return ( WinDefWindowProc ( hwnd, WM_SAVEAPPLICATION, 0, 0 ) ) ;
}

/****************************************************************************
 *                                                                          *
 *      Process timer message.                                              *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Refresh ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

 /***************************************************************************
  *                             Declarations                                *
  ***************************************************************************/

  static POINTL
    aptlHour[5]   = { 0,-15,  10,0,  0,60,  -10,0,  0,-15 },
    aptlMinute[5] = { 0,-20,   5,0,  0,80,   -5,0,  0,-20 },
    aptlSecond[2] = { 0,  0,   0,80 } ;

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PDATA Data = PDATA ( WinQueryWindowPtr ( hwnd, QWL_USER ) ) ;

 /***************************************************************************
  * If we're supposed to float the window, do so here.                      *
  ***************************************************************************/

  if ( Data->IniData.Float )
    WinSetWindowPos ( WinQueryWindow(hwnd,QW_PARENT), HWND_TOP, 0, 0, 0, 0, SWP_ZORDER ) ;

 /***************************************************************************
  * Save the idle counter.                                                  *
  ***************************************************************************/

  Data->IdleCount = LONGFROMMP ( mp1 ) ;

 /***************************************************************************
  * Get current time.                                                       *
  ***************************************************************************/

  DATETIME DateTime ;
  DosGetDateTime ( &DateTime ) ;

  USHORT Hour = USHORT ( ( DateTime.hours * 5 ) % 60 + DateTime.minutes / 12 ) ;

 /***************************************************************************
  * Get presentation space.                                                 *
  ***************************************************************************/

  HPS hPS = WinGetPS ( hwnd ) ;
  GpiCreateLogColorTable ( hPS, LCOL_RESET, LCOLF_RGB, 0L, 0L, PLONG(NULL) ) ;

 /***************************************************************************
  * If analog or minimized . . .                                            *
  ***************************************************************************/

  if ( Data->IniData.Analog OR ( Data->IniData.Position.fl & SWP_MINIMIZE ) )
  {

   /*************************************************************************
    * If a new minute has arrived, and the clock is minimized, clear it.    *
    *************************************************************************/

    if ( ( Data->IniData.Position.fl & SWP_MINIMIZE )
      AND ( DateTime.minutes != Data->PreviousDateTime.minutes ) )
    {
      PaintBackground ( hwnd, hPS, Data, TRUE ) ;
    }

   /*************************************************************************
    * Erase the old clock hands.                                            *
    *************************************************************************/

    GpiSetColor ( hPS, Data->IniData.BackColor ) ;
    if ( ( Data->wi.cxClient > 50 ) AND ( Data->wi.cyClient > 50 ) )
    {
      DrawHand ( hPS, aptlSecond, 2, Data->PreviousDateTime.seconds, &Data->wi ) ;
    }

    if ( ( Hour != Data->PreviousHour )
      OR ( DateTime.minutes != Data->PreviousDateTime.minutes ) )
    {
      DrawHand ( hPS, aptlHour,   5, Data->PreviousHour,             &Data->wi ) ;
      DrawHand ( hPS, aptlMinute, 5, Data->PreviousDateTime.minutes, &Data->wi ) ;
    }

   /*************************************************************************
    * Draw the new clock hands.                                             *
    *************************************************************************/

    GpiSetColor ( hPS, Data->IniData.TextColor ) ;
    DrawHand ( hPS, aptlHour,   5, Hour,             &Data->wi ) ;
    DrawHand ( hPS, aptlMinute, 5, DateTime.minutes, &Data->wi ) ;
    if ( ( Data->wi.cxClient > 50 ) AND ( Data->wi.cyClient > 50 ) ) {
      DrawHand ( hPS, aptlSecond, 2, DateTime.seconds, &Data->wi ) ;
    }

 /***************************************************************************
  * Else if digital . . .                                                   *
  ***************************************************************************/

  } else {

   /*************************************************************************
    * If minute has changed, or displaying seconds and seconds changed . . .*
    *************************************************************************/

    if ( ( DateTime.minutes != Data->PreviousDateTime.minutes )
      OR ( Data->IniData.Seconds AND ( DateTime.seconds != Data->PreviousDateTime.seconds ) ) ) {
      PaintDigitalTime ( hwnd, hPS, Data, &DateTime ) ;
    }
  }

 /***************************************************************************
  * Adjust the border color to suit the current alert level.                *
  ***************************************************************************/

  PaintBorder ( hwnd, hPS, Data, FALSE ) ;

 /***************************************************************************
  * Release presentation space.                                             *
  ***************************************************************************/

  WinReleasePS ( hPS ) ;

 /***************************************************************************
  * Chime if we've passed the top of the hour.  Save time when done.        *
  ***************************************************************************/

  if ( Data->IniData.Chime AND ( DateTime.hours != Data->PreviousDateTime.hours ) ) {
     if ( !WinAlarm ( HWND_DESKTOP, 11 ) ) {     // If no cuckoo clock, just beep.
        DosBeep  ( 400, 100 ) ;
        DosSleep (      100 ) ;
        DosBeep  ( 400, 100 ) ;
     } /* endif */
  }

  Data->PreviousDateTime = DateTime ;
  Data->PreviousHour = Hour ;

  return ( 0 ) ;
}

/****************************************************************************
 *                                                                          *
 *      Repaint entire window.                                              *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Paint ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

 /***************************************************************************
  *                             Declarations                                *
  ***************************************************************************/

  static POINTL
    aptlHour[5]   = { 0,-15,  10,0,  0,60,  -10,0,  0,-15 },
    aptlMinute[5] = { 0,-20,   5,0,  0,80,   -5,0,  0,-20 },
    aptlSecond[2] = { 0,  0,   0,80 } ;

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PDATA Data = PDATA ( WinQueryWindowPtr ( hwnd, QWL_USER ) ) ;

 /***************************************************************************
  * Get presentation space and make it use RGB colors.                      *
  ***************************************************************************/

  HPS hPS = WinBeginPaint ( hwnd, HPS(NULL), PRECTL(NULL) ) ;
  GpiCreateLogColorTable ( hPS, LCOL_RESET, LCOLF_RGB, 0L, 0L, PLONG(NULL) ) ;

 /***************************************************************************
  * Paint the background.                                                   *
  ***************************************************************************/

  PaintBackground ( hwnd, hPS, Data, TRUE ) ;

 /***************************************************************************
  * If analog or minimized . . .                                            *
  ***************************************************************************/

  if ( Data->IniData.Analog OR ( Data->IniData.Position.fl & SWP_MINIMIZE ) ) {

   /*************************************************************************
    * Draw hour, minute and second hands.                                   *
    *************************************************************************/

    DrawHand ( hPS, aptlHour,   5, Data->PreviousHour,             &Data->wi ) ;
    DrawHand ( hPS, aptlMinute, 5, Data->PreviousDateTime.minutes, &Data->wi ) ;

    if ( ( Data->wi.cxClient > 50 ) AND ( Data->wi.cyClient > 50 ) ) {
      DrawHand ( hPS, aptlSecond, 2, Data->PreviousDateTime.seconds, &Data->wi ) ;
    }

 /***************************************************************************
  * Else if digital . . .                                                   *
  ***************************************************************************/

  } else {
    PaintDigitalTime ( hwnd, hPS, Data, &Data->PreviousDateTime ) ;
  }

 /***************************************************************************
  * Release presentation space and return.                                  *
  ***************************************************************************/

  WinEndPaint ( hPS ) ;

  return ( 0 ) ;
}

/****************************************************************************
 *                                                                          *
 *      Process requests for menu initialization.                           *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY InitMenu ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PDATA Data = PDATA ( WinQueryWindowPtr ( hwnd, QWL_USER ) ) ;

 /***************************************************************************
  * Get the message data.                                                   *
  ***************************************************************************/

  int MenuID = SHORT1FROMMP ( mp1 ) ;
  HWND Menu = HWNDFROMMP ( mp2 ) ;

 /***************************************************************************
  * Process according to which menu's about to be displayed.                *
  ***************************************************************************/

  switch ( MenuID ) {

     case FID_SYSMENU:
     case FID_MENU:
     case IDM_MENU: {
        CheckMenuItem ( Menu, IDM_HIDE_CONTROLS, Data->IniData.HideControls ) ;
        break ; }

  }

 /***************************************************************************
  * We're done.                                                             *
  ***************************************************************************/

  return ( MRFROMSHORT ( 0 ) ) ;
}

/****************************************************************************
 *                                                                          *
 *      Process commands received by Main Window                            *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Command ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

 /***************************************************************************
  * Dispatch the messages.  There is no default message processor.          *
  ***************************************************************************/

  static METHOD Methods [] = {
    { IDM_SAVE_APPLICATION, SaveApplication },
    { IDM_RESET_DEFAULTS,   ResetDefaults   },
    { IDM_HIDE_CONTROLS,    HideControls    },
    { IDM_CONFIGURE,        Configure       },
    { IDM_EXIT,             Exit            },
    { IDM_ABOUT,            About           }
  } ;

  return ( DispatchMessage ( hwnd, SHORT1FROMMP(mp1), mp1, mp2, Methods, sizeof(Methods)/sizeof(Methods[0]), 0 ) ) ;
}

/****************************************************************************
 *                                                                          *
 *      Process Reset Defaults menu command.                                *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY ResetDefaults ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PDATA Data = PDATA ( WinQueryWindowPtr ( hwnd, QWL_USER ) ) ;

 /***************************************************************************
  * Reset all profile data for this program.                                *
  ***************************************************************************/

  PrfWriteProfileData ( Data->IniFile->QueryHandle(), PSZ(PROGRAM_NAME), PSZ(NULL), PSZ(NULL), 0 ) ;

 /***************************************************************************
  * Reset the program's presentation parameters.                            *
  ***************************************************************************/

  WinRemovePresParam ( hwnd, PP_FONTNAMESIZE ) ;
  WinRemovePresParam ( hwnd, PP_FOREGROUNDCOLOR ) ;
  WinRemovePresParam ( hwnd, PP_BACKGROUNDCOLOR ) ;

 /***************************************************************************
  * Done.                                                                   *
  ***************************************************************************/

  return ( MRFROMSHORT ( 0 ) ) ;
}

/****************************************************************************
 *                                                                          *
 *      Process Hide Controls menu command.                                 *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY HideControls ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PDATA Data = PDATA ( WinQueryWindowPtr ( hwnd, QWL_USER ) ) ;

 /***************************************************************************
  * Get the frame window handle.                                            *
  ***************************************************************************/

  HWND FrameWindow = WinQueryWindow ( hwnd, QW_PARENT ) ;

 /***************************************************************************
  * Toggle the Hide Controls setting.                                       *
  ***************************************************************************/

  Data->IniData.HideControls = Data->IniData.HideControls ? FALSE : TRUE ;
  Data->IniData.fHideControls = TRUE ;

 /***************************************************************************
  * If not minimized right now, hide or reveal the controls.                *
  ***************************************************************************/

  if ( NOT ( Data->IniData.Position.fl & SWP_MINIMIZE ) )
  {
    HideControls
    (
      Data->IniData.HideControls,
      FrameWindow,
      Data->hwndSysMenu,
      Data->hwndTitleBar,
      Data->hwndMinMax
    ) ;
  }

 /***************************************************************************
  * Done.                                                                   *
  ***************************************************************************/

  return ( MRFROMSHORT ( 0 ) ) ;
}

/****************************************************************************
 *                                                                          *
 *      Process Configure command.                                          *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Configure ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PDATA Data = PDATA ( WinQueryWindowPtr ( hwnd, QWL_USER ) ) ;

 /***************************************************************************
  * Invoke the Configure dialog.                                            *
  ***************************************************************************/

  CONFIG_PARMS Parms ;
  Parms.Id           = IDD_CONFIGURE ;
  Parms.Analog       = Data->IniData.Analog ;
  Parms.Hour24       = Data->IniData.Hour24 ;
  Parms.HideControls = Data->IniData.HideControls ;
  Parms.Chime        = Data->IniData.Chime ;
  Parms.Float        = Data->IniData.Float ;
  Parms.Animate      = Data->IniData.Animate ;
  Parms.Seconds      = Data->IniData.Seconds ;
  Parms.AlertType    = USHORT ( Data->IniData.AlertType ) ;

  memcpy ( Parms.AlertLevels, Data->IniData.AlertLevels, sizeof(Parms.AlertLevels) ) ;

  if ( WinDlgBox ( HWND_DESKTOP, hwnd, ConfigureProcessor,
    Data->Library->QueryHandle(), IDD_CONFIGURE, &Parms ) )
  {
    Data->IniData.fHour24 = TRUE ;
    if ( Data->IniData.Hour24 != Parms.Hour24 )
    {
      Data->IniData.Hour24 = Parms.Hour24 ;
      WinInvalidateRect ( hwnd, PRECTL(NULL), FALSE ) ;
    }

    Data->IniData.fChime = TRUE ;
    Data->IniData.Chime = Parms.Chime ;

    Data->IniData.fFloat = TRUE ;
    Data->IniData.Float = Parms.Float ;

    Data->IniData.fAnimate = TRUE ;
    Data->IniData.Animate = Parms.Animate ;

    Data->IniData.fSeconds = TRUE ;
    if ( Data->IniData.Seconds != Parms.Seconds ) {
      Data->IniData.Seconds = Parms.Seconds ;
      WinInvalidateRect ( hwnd, PRECTL(NULL), FALSE ) ;
    } /* endif */

    Data->IniData.fAlertType = TRUE ;
    if ( Data->IniData.AlertType != Parms.AlertType )
    {
      Data->IniData.AlertType = Parms.AlertType ;
      if ( Data->IniData.AlertType == ALERT_LOAD )
        DosResumeThread ( Data->IdleLoopTID ) ;
      else
        DosSuspendThread ( Data->IdleLoopTID ) ;
    }

    Data->IniData.fAnalog = TRUE ;
    if ( Data->IniData.Analog != Parms.Analog )
    {
      Data->IniData.Analog = Parms.Analog ;
      WinInvalidateRect ( hwnd, PRECTL(NULL), FALSE ) ;
    }

    Data->IniData.fHideControls = TRUE ;
    if ( Data->IniData.HideControls != Parms.HideControls )
    {
      HWND FrameWindow = WinQueryWindow ( hwnd, QW_PARENT ) ;
      Data->IniData.HideControls = Parms.HideControls ;
      if ( NOT ( Data->IniData.Position.fl & SWP_MINIMIZE ) )
      {
        HideControls
        (
          Data->IniData.HideControls,
          FrameWindow,
          Data->hwndSysMenu,
          Data->hwndTitleBar,
          Data->hwndMinMax
        ) ;
      }
    }

    Data->IniData.fAlertLevels = TRUE ;
    memcpy ( Data->IniData.AlertLevels, Parms.AlertLevels, sizeof(Parms.AlertLevels) ) ;
  }

 /***************************************************************************
  * Done.                                                                   *
  ***************************************************************************/

  return ( MRFROMSHORT ( 0 ) ) ;
}

/****************************************************************************
 *                                                                          *
 *      Process About menu command.                                         *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY About ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PDATA Data = PDATA ( WinQueryWindowPtr ( hwnd, QWL_USER ) ) ;

 /***************************************************************************
  * Invoke the About dialog.                                                *
  ***************************************************************************/

  WinDlgBox ( HWND_DESKTOP, hwnd, AboutProcessor,
    Data->Library->QueryHandle(), IDD_ABOUT, 0 ) ;

 /***************************************************************************
  * Done.                                                                   *
  ***************************************************************************/

  return ( MRFROMSHORT ( 0 ) ) ;
}

/****************************************************************************
 *                                                                          *
 *      Process Mouse Button being pressed.                                 *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY ButtonDown ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

//PDATA Data = PDATA ( WinQueryWindowPtr ( hwnd, QWL_USER ) ) ;

 /***************************************************************************
  * Determine the new window position.                                      *
  ***************************************************************************/

  TRACKINFO TrackInfo ;
  memset ( &TrackInfo, 0, sizeof(TrackInfo) ) ;

  TrackInfo.cxBorder = 1 ;
  TrackInfo.cyBorder = 1 ;
  TrackInfo.cxGrid = 1 ;
  TrackInfo.cyGrid = 1 ;
  TrackInfo.cxKeyboard = 8 ;
  TrackInfo.cyKeyboard = 8 ;

  HWND FrameWindow = WinQueryWindow ( hwnd, QW_PARENT ) ;

  SWP Position ;
  WinQueryWindowPos ( FrameWindow, &Position ) ;
  TrackInfo.rclTrack.xLeft   = Position.x ;
  TrackInfo.rclTrack.xRight  = Position.x + Position.cx ;
  TrackInfo.rclTrack.yBottom = Position.y ;
  TrackInfo.rclTrack.yTop    = Position.y + Position.cy ;

  WinQueryWindowPos ( HWND_DESKTOP, &Position ) ;
  TrackInfo.rclBoundary.xLeft   = Position.x ;
  TrackInfo.rclBoundary.xRight  = Position.x + Position.cx ;
  TrackInfo.rclBoundary.yBottom = Position.y ;
  TrackInfo.rclBoundary.yTop    = Position.y + Position.cy ;

  TrackInfo.ptlMinTrackSize.x = 0 ;
  TrackInfo.ptlMinTrackSize.y = 0 ;
  TrackInfo.ptlMaxTrackSize.x = Position.cx ;
  TrackInfo.ptlMaxTrackSize.y = Position.cy ;

  TrackInfo.fs = TF_MOVE | TF_STANDARD | TF_ALLINBOUNDARY ;

  if ( WinTrackRect ( HWND_DESKTOP, HPS(NULL), &TrackInfo ) )
  {
    WinSetWindowPos ( FrameWindow, HWND(NULL),
      (SHORT) TrackInfo.rclTrack.xLeft,
      (SHORT) TrackInfo.rclTrack.yBottom,
      0, 0, SWP_MOVE ) ;
  }

 /***************************************************************************
  * Return through the default processor, letting window activation         *
  *   and other system functions occur.                                     *
  ***************************************************************************/

  return ( WinDefWindowProc ( hwnd, msg, mp1, mp2 ) ) ;
}

/****************************************************************************
 *                                                                          *
 *      Process Mouse Button having been double-clicked.                    *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY ButtonDblClick ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

 /***************************************************************************
  * Send message to self to stop hiding the controls.                       *
  ***************************************************************************/

  WinPostMsg ( hwnd, WM_COMMAND,
    MPFROM2SHORT ( IDM_HIDE_CONTROLS, 0 ),
    MPFROM2SHORT ( CMDSRC_OTHER, TRUE ) ) ;

 /***************************************************************************
  * Return through the default processor, letting window activation         *
  *   and other system functions occur.                                     *
  ***************************************************************************/

  return ( WinDefWindowProc ( hwnd, msg, mp1, mp2 ) ) ;
}

/****************************************************************************
 *                                                                          *
 *      Process Presentation Parameter Changed notification.                *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY ContextMenu ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

  /**************************************************************************
   * Find the instance data.                                                *
   **************************************************************************/

   PDATA Data = PDATA ( WinQueryWindowPtr ( hwnd, QWL_USER ) ) ;

  /**************************************************************************
   * Invoke the window's context menu.                                      *
   **************************************************************************/

   WinSetPresParam ( Data->Menu, PP_FONTNAMESIZE, 0, PSZ("") ) ;

   WinPopupMenu ( hwnd, hwnd, Data->Menu, SHORT1FROMMP(mp1), SHORT2FROMMP(mp1),
      0, PU_HCONSTRAIN | PU_VCONSTRAIN | PU_KEYBOARD | PU_MOUSEBUTTON1 ) ;

  /**************************************************************************
   * Done.                                                                  *
   **************************************************************************/

   return ( MRESULT ( 0 ) ) ;
}

/****************************************************************************
 *                                                                          *
 *      Process Presentation Parameter Changed notification.                *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY PresParamChanged ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PDATA Data = PDATA ( WinQueryWindowPtr ( hwnd, QWL_USER ) ) ;

 /***************************************************************************
  * Get the presentation parameter that changed.                            *
  ***************************************************************************/

  switch ( LONGFROMMP(mp1) )
  {

   /*************************************************************************
    * If font, note the fact that we now have a font to be saved as         *
    *   part of the configuration.  Get the font metrics and resize         *
    *   the window appropriately.                                           *
    *************************************************************************/

    case PP_FONTNAMESIZE:
    {
      ULONG ppid ;
      if ( WinQueryPresParam ( hwnd, PP_FONTNAMESIZE, 0, &ppid,
        sizeof(Data->IniData.FontNameSize), &Data->IniData.FontNameSize,
        0 ) )
      {
        Data->IniData.fFontNameSize = TRUE ;
      }
      else
      {
        strcpy ( PCHAR(Data->IniData.FontNameSize), "" ) ;
        Data->IniData.fFontNameSize = FALSE ;
        PrfWriteProfileData ( Data->IniFile->QueryHandle(), PSZ(PROGRAM_NAME), PSZ("FontNameSize"), NULL, 0 ) ;
      }
      WinInvalidateRect ( hwnd, PRECTL(NULL), TRUE ) ;
      break ;
    }

   /*************************************************************************
    * If background color, note the fact and repaint the window.            *
    *************************************************************************/

    case PP_BACKGROUNDCOLOR:
    {
      ULONG ppid ;
      if ( WinQueryPresParam ( hwnd, PP_BACKGROUNDCOLOR, 0, &ppid,
        sizeof(Data->IniData.BackColor), &Data->IniData.BackColor, 0 ) )
      {
        Data->IniData.fBackColor = TRUE ;
      }
      else
      {
        Data->IniData.BackColor = WinQuerySysColor ( HWND_DESKTOP, SYSCLR_WINDOW, 0L ) ;
        Data->IniData.fBackColor = FALSE ;
        PrfWriteProfileData ( Data->IniFile->QueryHandle(), PSZ(PROGRAM_NAME), PSZ("BackgroundColor"), NULL, 0 ) ;
      }
      WinInvalidateRect ( hwnd, PRECTL(NULL), TRUE ) ;
      break ;
    }

   /*************************************************************************
    * If foreground color, note the fact and repaint the window.            *
    *************************************************************************/

    case PP_FOREGROUNDCOLOR:
    {
      ULONG ppid ;
      if ( WinQueryPresParam ( hwnd, PP_FOREGROUNDCOLOR, 0, &ppid,
        sizeof(Data->IniData.TextColor), &Data->IniData.TextColor, 0 ) )
      {
        Data->IniData.fTextColor = TRUE ;
      }
      else
      {
        Data->IniData.TextColor = WinQuerySysColor ( HWND_DESKTOP, SYSCLR_OUTPUTTEXT, 0L ) ;
        Data->IniData.fTextColor = FALSE ;
        PrfWriteProfileData ( Data->IniFile->QueryHandle(), PSZ(PROGRAM_NAME), PSZ("ForegroundColor"), NULL, 0 ) ;
      }
      WinInvalidateRect ( hwnd, PRECTL(NULL), TRUE ) ;
      break ;
    }
  }

 /***************************************************************************
  * Return through the default processor, letting window activation         *
  *   and other system functions occur.                                     *
  ***************************************************************************/

  return ( WinDefWindowProc ( hwnd, msg, mp1, mp2 ) ) ;
}

/****************************************************************************
 *                                                                          *
 *      Process System Color Change notification.                           *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY SysColorChange ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PDATA Data = PDATA ( WinQueryWindowPtr ( hwnd, QWL_USER ) ) ;

 /***************************************************************************
  * If we aren't using custom colors, then query for the new defaults.      *
  ***************************************************************************/

  if ( NOT Data->IniData.fBackColor ) {
    Data->IniData.BackColor = WinQuerySysColor ( HWND_DESKTOP, SYSCLR_WINDOW, 0L ) ;
  }

  if ( NOT Data->IniData.fTextColor ) {
    Data->IniData.TextColor = WinQuerySysColor ( HWND_DESKTOP, SYSCLR_OUTPUTTEXT, 0L ) ;
  }

 /***************************************************************************
  * Return value must be NULL, according to the documentation.              *
  ***************************************************************************/

  return ( MRFROMP ( NULL ) ) ;
}

/****************************************************************************
 *                                                                          *
 *      Process Query for Keys Help resource id.                            *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY QueryKeysHelp ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

 /***************************************************************************
  * Simply return the ID of the Keys Help panel.                            *
  ***************************************************************************/

  return ( (MRESULT) IDM_KEYS_HELP ) ;
}

/****************************************************************************
 *                                                                          *
 *      Process Help Manager Error                                          *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY HelpError ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PDATA Data = PDATA ( WinQueryWindowPtr ( hwnd, QWL_USER ) ) ;

 /***************************************************************************
  * Find the error code in the message table.                               *
  ***************************************************************************/

  static struct {
    ULONG Error ;
    USHORT StringId ;
  } HelpErrors [] = {
    { HMERR_NO_FRAME_WND_IN_CHAIN,     IDS_HMERR_NO_FRAME_WND_IN_CHAIN },
    { HMERR_INVALID_ASSOC_APP_WND,     IDS_HMERR_INVALID_ASSOC_APP_WND },
    { HMERR_INVALID_ASSOC_HELP_INST,   IDS_HMERR_INVALID_ASSOC_HELP_IN },
    { HMERR_INVALID_DESTROY_HELP_INST, IDS_HMERR_INVALID_DESTROY_HELP_ },
    { HMERR_NO_HELP_INST_IN_CHAIN,     IDS_HMERR_NO_HELP_INST_IN_CHAIN },
    { HMERR_INVALID_HELP_INSTANCE_HDL, IDS_HMERR_INVALID_HELP_INSTANCE },
    { HMERR_INVALID_QUERY_APP_WND,     IDS_HMERR_INVALID_QUERY_APP_WND },
    { HMERR_HELP_INST_CALLED_INVALID,  IDS_HMERR_HELP_INST_CALLED_INVA },
    { HMERR_HELPTABLE_UNDEFINE,        IDS_HMERR_HELPTABLE_UNDEFINE    },
    { HMERR_HELP_INSTANCE_UNDEFINE,    IDS_HMERR_HELP_INSTANCE_UNDEFIN },
    { HMERR_HELPITEM_NOT_FOUND,        IDS_HMERR_HELPITEM_NOT_FOUND    },
    { HMERR_INVALID_HELPSUBITEM_SIZE,  IDS_HMERR_INVALID_HELPSUBITEM_S },
    { HMERR_HELPSUBITEM_NOT_FOUND,     IDS_HMERR_HELPSUBITEM_NOT_FOUND },
    { HMERR_INDEX_NOT_FOUND,           IDS_HMERR_INDEX_NOT_FOUND       },
    { HMERR_CONTENT_NOT_FOUND,         IDS_HMERR_CONTENT_NOT_FOUND     },
    { HMERR_OPEN_LIB_FILE,             IDS_HMERR_OPEN_LIB_FILE         },
    { HMERR_READ_LIB_FILE,             IDS_HMERR_READ_LIB_FILE         },
    { HMERR_CLOSE_LIB_FILE,            IDS_HMERR_CLOSE_LIB_FILE        },
    { HMERR_INVALID_LIB_FILE,          IDS_HMERR_INVALID_LIB_FILE      },
    { HMERR_NO_MEMORY,                 IDS_HMERR_NO_MEMORY             },
    { HMERR_ALLOCATE_SEGMENT,          IDS_HMERR_ALLOCATE_SEGMENT      },
    { HMERR_FREE_MEMORY,               IDS_HMERR_FREE_MEMORY           },
    { HMERR_PANEL_NOT_FOUND,           IDS_HMERR_PANEL_NOT_FOUND       },
    { HMERR_DATABASE_NOT_OPEN,         IDS_HMERR_DATABASE_NOT_OPEN     },
    { 0,                               IDS_HMERR_UNKNOWN               }
  } ;

  ULONG ErrorCode = (ULONG) LONGFROMMP ( mp1 ) ;
  int Index = 0 ;
  while ( HelpErrors[Index].Error
    AND ( HelpErrors[Index].Error != ErrorCode ) ) {
    Index ++ ;
  }

 /***************************************************************************
  * Get the message texts.                                                  *
  ***************************************************************************/

  ResourceString Title ( Data->Library->QueryHandle(), IDS_HMERR ) ;

  ResourceString Message ( Data->Library->QueryHandle(), HelpErrors[Index].StringId ) ;

 /***************************************************************************
  * Display the error message.                                              *
  ***************************************************************************/

  WinMessageBox
  (
    HWND_DESKTOP,
    hwnd,
    PSZ(Message),
    PSZ(Title),
    0,
    MB_OK | MB_WARNING
  ) ;

 /***************************************************************************
  * Return zero, indicating that the message was processed.                 *
  ***************************************************************************/

  return ( MRFROMSHORT ( 0 ) ) ;
}

/****************************************************************************
 *                                                                          *
 *      Process "Extended Help Undefined" notification                      *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY ExtHelpUndefined ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PDATA Data = PDATA ( WinQueryWindowPtr ( hwnd, QWL_USER ) ) ;

 /***************************************************************************
  * Get the message texts.                                                  *
  ***************************************************************************/

  ResourceString Title ( Data->Library->QueryHandle(), IDS_HMERR ) ;

  ResourceString Message ( Data->Library->QueryHandle(), IDS_HMERR_EXTHELPUNDEFINED ) ;

 /***************************************************************************
  * Display the error message.                                              *
  ***************************************************************************/

  WinMessageBox
  (
    HWND_DESKTOP,
    hwnd,
    PSZ(Message),
    PSZ(Title),
    0,
    MB_OK | MB_WARNING
  ) ;

 /***************************************************************************
  * Return zero, indicating that the message was processed.                 *
  ***************************************************************************/

  return ( MRFROMSHORT ( 0 ) ) ;
}

/****************************************************************************
 *                                                                          *
 *      Process "Help Subitem Not Found" notification                       *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY HelpSubitemNotFound ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PDATA Data = PDATA ( WinQueryWindowPtr ( hwnd, QWL_USER ) ) ;

 /***************************************************************************
  * Get the title text.                                                     *
  ***************************************************************************/

  ResourceString Title ( Data->Library->QueryHandle(), IDS_HMERR ) ;

 /***************************************************************************
  * Format the error message.                                               *
  ***************************************************************************/

  USHORT Topic = (USHORT) SHORT1FROMMP ( mp2 ) ;
  USHORT Subtopic = (USHORT) SHORT2FROMMP ( mp2 ) ;

  ResourceString Frame   ( Data->Library->QueryHandle(), IDS_HELPMODE_FRAME ) ;
  ResourceString Menu    ( Data->Library->QueryHandle(), IDS_HELPMODE_MENU ) ;
  ResourceString Window  ( Data->Library->QueryHandle(), IDS_HELPMODE_WINDOW ) ;
  ResourceString Unknown ( Data->Library->QueryHandle(), IDS_HELPMODE_UNKNOWN ) ;

  PBYTE Mode ;
  switch ( SHORT1FROMMP ( mp1 ) )
  {
    case HLPM_FRAME:
      Mode = PSZ(Frame) ;
      break ;

    case HLPM_MENU:
      Mode = PSZ(Menu) ;
      break ;

    case HLPM_WINDOW:
      Mode = PSZ(Window) ;
      break ;

    default:
      Mode = PSZ(Unknown) ;
  }

  ResourceString Format ( Data->Library->QueryHandle(), IDS_HELPSUBITEMNOTFOUND ) ;

  BYTE Message [200] ;
  sprintf ( PCHAR(Message), PCHAR(Format), Mode, Topic, Subtopic ) ;

 /***************************************************************************
  * Display the error message.                                              *
  ***************************************************************************/

  WinMessageBox
  (
    HWND_DESKTOP,
    hwnd,
    Message,
    PSZ(Title),
    0,
    MB_OK | MB_WARNING
  ) ;

 /***************************************************************************
  * Return zero, indicating that the message was processed.                 *
  ***************************************************************************/

  return ( MRFROMSHORT ( 0 ) ) ;
}


/****************************************************************************
 *                                                                          *
 *                           Get Profile Data                               *
 *                                                                          *
 ****************************************************************************/

static int GetIniData ( HINI IniHandle, PINIDATA IniData )
{
 /***************************************************************************
  * Get the window's current size and position.                             *
  ***************************************************************************/

  #pragma pack(2)
  typedef struct {
    USHORT Filler ;
    USHORT fs ;
    USHORT cy, cx, y, x ;
    HWND hwndInsertBehind ;
    HWND hwnd ;
  } OLDSWP ;
  #pragma pack()

  ULONG Size ;
  memset ( &IniData->Position, 0, sizeof(IniData->Position) ) ;
  IniData->fPosition = FALSE ;
  if ( PrfQueryProfileSize ( IniHandle, PSZ(PROGRAM_NAME), PSZ("Position"), &Size ) )
  {
    if ( Size == sizeof(OLDSWP)-sizeof(USHORT) )
    {
      OLDSWP OldPosition ;
      if ( PrfQueryProfileData ( IniHandle, PSZ(PROGRAM_NAME), PSZ("Position"), &OldPosition.fs, &Size ) )
      {
        IniData->Position.fl = OldPosition.fs ;
        IniData->Position.cy = OldPosition.cy ;
        IniData->Position.cx = OldPosition.cx ;
        IniData->Position.y = OldPosition.y ;
        IniData->Position.x = OldPosition.x ;
        IniData->Position.hwndInsertBehind = OldPosition.hwndInsertBehind ;
        IniData->Position.hwnd = OldPosition.hwnd ;
        IniData->fPosition = TRUE ;
      }
    }
    else if ( Size == sizeof(IniData->Position) )
    {
      if ( PrfQueryProfileData ( IniHandle, PSZ(PROGRAM_NAME), PSZ("Position"), &IniData->Position, &Size ) )
      {
        IniData->fPosition = TRUE ;
      }
    }
  }

  if ( NOT IniData->fPosition )
  {
    if ( IniHandle == HINI_USERPROFILE )
    {
      return ( 1 ) ;
    }
  }

 /***************************************************************************
  * Get the program options.                                                *
  ***************************************************************************/

  IniData->Hour24 = FALSE ;
  if
  (
    PrfQueryProfileSize ( IniHandle, PSZ(PROGRAM_NAME), PSZ("Hour24"), &Size )
    AND
    ( ( Size == sizeof(IniData->Hour24) ) OR ( Size == sizeof(SHORT) ) )
    AND
    PrfQueryProfileData ( IniHandle, PSZ(PROGRAM_NAME), PSZ("Hour24"), &IniData->Hour24, &Size )
  )
  {
    IniData->fHour24 = TRUE ;
  }

  IniData->HideControls = FALSE ;
  if
  (
    PrfQueryProfileSize ( IniHandle, PSZ(PROGRAM_NAME), PSZ("HideControls"), &Size )
    AND
    ( ( Size == sizeof(IniData->HideControls) ) OR ( Size == sizeof(SHORT) ) )
    AND
    PrfQueryProfileData ( IniHandle, PSZ(PROGRAM_NAME), PSZ("HideControls"), &IniData->HideControls, &Size )
  )
  {
    IniData->fHideControls = TRUE ;
  }

  IniData->Chime = FALSE ;
  if
  (
    PrfQueryProfileSize ( IniHandle, PSZ(PROGRAM_NAME), PSZ("Chime"), &Size )
    AND
    ( ( Size == sizeof(IniData->Chime) ) OR ( Size == sizeof(SHORT) ) )
    AND
    PrfQueryProfileData ( IniHandle, PSZ(PROGRAM_NAME), PSZ("Chime"), &IniData->Chime, &Size )
  )
  {
    IniData->fChime = TRUE ;
  }

  IniData->Float = FALSE ;
  if
  (
    PrfQueryProfileSize ( IniHandle, PSZ(PROGRAM_NAME), PSZ("Float"), &Size )
    AND
    ( ( Size == sizeof(IniData->Float) ) OR ( Size == sizeof(SHORT) ) )
    AND
    PrfQueryProfileData ( IniHandle, PSZ(PROGRAM_NAME), PSZ("Float"), &IniData->Float, &Size )
  )
  {
    IniData->fFloat = TRUE ;
  }

  IniData->Animate = FALSE ;
  if
  (
    PrfQueryProfileSize ( IniHandle, PSZ(PROGRAM_NAME), PSZ("Animate"), &Size )
    AND
    ( ( Size == sizeof(IniData->Animate) ) OR ( Size == sizeof(SHORT) ) )
    AND
    PrfQueryProfileData ( IniHandle, PSZ(PROGRAM_NAME), PSZ("Animate"), &IniData->Animate, &Size )
  )
  {
    IniData->fAnimate = TRUE ;
  }

  IniData->Seconds = FALSE ;
  if
  (
    PrfQueryProfileSize ( IniHandle, PSZ(PROGRAM_NAME), PSZ("Seconds"), &Size )
    AND
    ( ( Size == sizeof(IniData->Seconds) ) OR ( Size == sizeof(SHORT) ) )
    AND
    PrfQueryProfileData ( IniHandle, PSZ(PROGRAM_NAME), PSZ("Seconds"), &IniData->Seconds, &Size )
  )
  {
    IniData->fSeconds = TRUE ;
  }

  IniData->Analog = TRUE ;
  if
  (
    PrfQueryProfileSize ( IniHandle, PSZ(PROGRAM_NAME), PSZ("Analog"), &Size )
    AND
    ( ( Size == sizeof(IniData->Analog) ) OR ( Size == sizeof(SHORT) ) )
    AND
    PrfQueryProfileData ( IniHandle, PSZ(PROGRAM_NAME), PSZ("Analog"), &IniData->Analog, &Size )
  )
  {
    IniData->fAnalog = TRUE ;
  }

  IniData->AlertType = ALERT_TASKCOUNT ;
  if
  (
    PrfQueryProfileSize ( IniHandle, PSZ(PROGRAM_NAME), PSZ("AlertType"), &Size )
    AND
    ( ( Size == sizeof(IniData->AlertType) ) OR ( Size == sizeof(SHORT) ) )
    AND
    PrfQueryProfileData ( IniHandle, PSZ(PROGRAM_NAME), PSZ("AlertType"), &IniData->AlertType, &Size )
  )
  {
    IniData->fAlertType = TRUE ;
  }

  IniData->AlertLevels [ALERT_TASKCOUNT] [0] = 8 ;
  IniData->AlertLevels [ALERT_TASKCOUNT] [1] = 12 ;
  IniData->AlertLevels [ALERT_LOAD]      [0] = 33 ;
  IniData->AlertLevels [ALERT_LOAD]      [1] = 67 ;

  if
  (
    PrfQueryProfileSize ( IniHandle, PSZ(PROGRAM_NAME), PSZ("AlertLevels"), &Size )
    AND
    ( Size == sizeof(IniData->AlertLevels) )
    AND
    PrfQueryProfileData ( IniHandle, PSZ(PROGRAM_NAME), PSZ("AlertLevels"), IniData->AlertLevels, &Size )
  )
  {
    IniData->fAlertLevels = TRUE ;
  }

 /***************************************************************************
  * Get the presentation parameters.                                        *
  ***************************************************************************/

  strcpy ( PCHAR(IniData->FontNameSize), "" ) ;
  IniData->fFontNameSize = FALSE ;
  if
  (
    PrfQueryProfileSize ( IniHandle, PSZ(PROGRAM_NAME), PSZ("FontNameSize"), &Size )
    AND
    ( Size == sizeof(IniData->FontNameSize) )
    AND
    PrfQueryProfileData ( IniHandle, PSZ(PROGRAM_NAME), PSZ("FontNameSize"), &IniData->FontNameSize, &Size )
  )
  {
    IniData->fFontNameSize = TRUE ;
  }

  IniData->BackColor = WinQuerySysColor ( HWND_DESKTOP, SYSCLR_WINDOW, 0L ) ;
  IniData->fBackColor = FALSE ;
  if
  (
    PrfQueryProfileSize ( IniHandle, PSZ(PROGRAM_NAME), PSZ("BackgroundColor"), &Size )
    AND
    ( Size == sizeof(IniData->BackColor) )
    AND
    PrfQueryProfileData ( IniHandle, PSZ(PROGRAM_NAME), PSZ("BackgroundColor"), &IniData->BackColor, &Size )
  )
  {
    IniData->fBackColor = TRUE ;
  }

  IniData->TextColor = WinQuerySysColor ( HWND_DESKTOP, SYSCLR_OUTPUTTEXT, 0L ) ;
  IniData->fTextColor = FALSE ;
  if
  (
    PrfQueryProfileSize ( IniHandle, PSZ(PROGRAM_NAME), PSZ("ForegroundColor"), &Size )
    AND
    ( Size == sizeof(IniData->TextColor) )
    AND
    PrfQueryProfileData ( IniHandle, PSZ(PROGRAM_NAME), PSZ("ForegroundColor"), &IniData->TextColor, &Size )
  )
  {
    IniData->fTextColor = TRUE ;
  }

 /***************************************************************************
  * Return no error.                                                        *
  ***************************************************************************/

  return ( 0 ) ;
}

/****************************************************************************
 *                                                                          *
 *                           Put Profile Data                               *
 *                                                                          *
 ****************************************************************************/

static VOID PutIniData ( HINI IniHandle, PINIDATA IniData )
{
 /***************************************************************************
  * Save the window's current size and position.                            *
  ***************************************************************************/

  PrfWriteProfileData
  (
    IniHandle,
    PSZ(PROGRAM_NAME),
    PSZ("Position"),
    &IniData->Position,
    sizeof(IniData->Position)
  ) ;

 /***************************************************************************
  * Save the program options.                                               *
  ***************************************************************************/

  if ( IniData->fHour24 )
  {
    PrfWriteProfileData
    (
      IniHandle,
      PSZ(PROGRAM_NAME),
      PSZ("Hour24"),
      &IniData->Hour24,
      (ULONG)sizeof(IniData->Hour24)
    ) ;
  }

  if ( IniData->fHideControls )
  {
    PrfWriteProfileData
    (
      IniHandle,
      PSZ(PROGRAM_NAME),
      PSZ("HideControls"),
      &IniData->HideControls,
      (ULONG)sizeof(IniData->HideControls)
    ) ;
  }

  if ( IniData->fChime )
  {
    PrfWriteProfileData
    (
      IniHandle,
      PSZ(PROGRAM_NAME),
      PSZ("Chime"),
      &IniData->Chime,
      (ULONG)sizeof(IniData->Chime)
    ) ;
  }

  if ( IniData->fFloat )
  {
    PrfWriteProfileData
    (
      IniHandle,
      PSZ(PROGRAM_NAME),
      PSZ("Float"),
      &IniData->Float,
      (ULONG)sizeof(IniData->Float)
    ) ;
  }

  if ( IniData->fAnimate )
  {
    PrfWriteProfileData
    (
      IniHandle,
      PSZ(PROGRAM_NAME),
      PSZ("Animate"),
      &IniData->Animate,
      (ULONG)sizeof(IniData->Animate)
    ) ;
  }

  if ( IniData->fSeconds )
  {
    PrfWriteProfileData
    (
      IniHandle,
      PSZ(PROGRAM_NAME),
      PSZ("Seconds"),
      &IniData->Seconds,
      (ULONG)sizeof(IniData->Seconds)
    ) ;
  }

  if ( IniData->fAnalog )
  {
    PrfWriteProfileData
    (
      IniHandle,
      PSZ(PROGRAM_NAME),
      PSZ("Analog"),
      &IniData->Analog,
      (ULONG)sizeof(IniData->Analog)
    ) ;
  }

  if ( IniData->fAlertType )
  {
    PrfWriteProfileData
    (
      IniHandle,
      PSZ(PROGRAM_NAME),
      PSZ("AlertType"),
      &IniData->AlertType,
      (ULONG)sizeof(IniData->AlertType)
    ) ;
  }

  if ( IniData->fAlertLevels )
  {
    PrfWriteProfileData
    (
      IniHandle,
      PSZ(PROGRAM_NAME),
      PSZ("AlertLevels"),
      IniData->AlertLevels,
      (ULONG)sizeof(IniData->AlertLevels)
    ) ;
  }

 /***************************************************************************
  * Save the presentation parameters.                                       *
  ***************************************************************************/

  if ( IniData->fFontNameSize )
  {
    PrfWriteProfileData
    (
      IniHandle,
      PSZ(PROGRAM_NAME),
      PSZ("FontNameSize"),
      IniData->FontNameSize,
      sizeof(IniData->FontNameSize)
    ) ;
  }

  if ( IniData->fBackColor )
  {
    PrfWriteProfileData
    (
      IniHandle,
      PSZ(PROGRAM_NAME),
      PSZ("BackgroundColor"),
      &IniData->BackColor,
      sizeof(IniData->BackColor)
    ) ;
  }

  if ( IniData->fTextColor )
  {
    PrfWriteProfileData
    (
      IniHandle,
      PSZ(PROGRAM_NAME),
      PSZ("ForegroundColor"),
      &IniData->TextColor,
      sizeof(IniData->TextColor)
    ) ;
  }
}

/****************************************************************************
 *                                                                          *
 *      Perform point coordinate rotation.                                  *
 *                                                                          *
 ****************************************************************************/

static VOID RotatePoint ( POINTL aptl[],  SHORT sNum, SHORT sAngle )
{
  static SHORT sSin[60] = {
      0,  105,  208,  309,  407,  500,  588,  669,  743,  809,
    866,  914,  951,  978,  995, 1000,  995,  978,  951,  914,
    866,  809,  743,  669,  588,  500,  407,  309,  208,  105,
      0, -104, -207, -308, -406, -499, -587, -668, -742, -808,
   -865, -913, -950, -977, -994, -999, -994, -977, -950, -913,
   -865, -808, -742, -668, -587, -499, -406, -308, -207, -104 };

  POINTL ptlTemp ;
  SHORT sIndex ;

  for ( sIndex = 0; sIndex < sNum ; sIndex++ )
  {
    ptlTemp.x = (aptl[sIndex].x * sSin[(sAngle+15)%60] +
      aptl[sIndex].y * sSin[sAngle]) / 1000 ;
    ptlTemp.y = (aptl[sIndex].y * sSin[(sAngle+15)%60] -
      aptl[sIndex].x * sSin[sAngle]) / 1000 ;
    aptl[sIndex] = ptlTemp ;
  }
}

/****************************************************************************
 *                                                                          *
 *      Perform point coordinate scaling.                                   *
 *                                                                          *
 ****************************************************************************/

static VOID ScalePoint ( POINTL aptl[], SHORT sNum, PWINDOWINFO pwi )
{
  SHORT sIndex ;

  for ( sIndex=0; sIndex < sNum; sIndex++ )
  {
    aptl[sIndex].x = aptl[sIndex].x * pwi->cxPixelDiam / 200 ;
    aptl[sIndex].y = aptl[sIndex].y * pwi->cyPixelDiam / 200 ;
  }
}

/****************************************************************************
 *                                                                          *
 *      Perform point coordinate translation.                               *
 *                                                                          *
 ****************************************************************************/

static VOID TranslatePoint ( POINTL aptl[], SHORT sNum, PWINDOWINFO pwi )
{
  SHORT sIndex ;

  for ( sIndex=0; sIndex<sNum; sIndex++ )
  {
    aptl[sIndex].x += pwi->cxClient / 2 - 1 ;
    aptl[sIndex].y += pwi->cyClient / 2 - 1 ;
  }
}

/****************************************************************************
 *                                                                          *
 *      Draw a clock hand.                                                  *
 *                                                                          *
 ****************************************************************************/

static VOID DrawHand ( HPS hPS, POINTL aptlIn[], SHORT sNum, SHORT sAngle,
  PWINDOWINFO pwi )
{
  POINTL aptl[5] ;
  SHORT sIndex ;

  for ( sIndex=0; sIndex<sNum; sIndex++ )
  {
    aptl [ sIndex ] = aptlIn [ sIndex ] ;
  }

  RotatePoint ( aptl, sNum, sAngle ) ;
  ScalePoint ( aptl, sNum, pwi ) ;
  TranslatePoint ( aptl, sNum, pwi ) ;

  GpiMove ( hPS, aptl ) ;
  GpiPolyLine ( hPS, sNum-1L, aptl+1 ) ;
}

/****************************************************************************
 *                                                                          *
 *                   Paint Background (all but hands)                       *
 *                                                                          *
 ****************************************************************************/

static void PaintBackground ( HWND hwnd, HPS hPS, PDATA Data, BOOL MustPaint ) {

 /***************************************************************************
  * Clear the window.                                                       *
  ***************************************************************************/

  RECTL Rectangle ;
  WinQueryWindowRect ( hwnd, &Rectangle ) ;

  GpiMove ( hPS, (PPOINTL) &Rectangle.xLeft ) ;
  GpiSetColor ( hPS, Data->IniData.BackColor ) ;
  GpiBox ( hPS, DRO_FILL, (PPOINTL) &Rectangle.xRight, 0L, 0L ) ;

 /***************************************************************************
  * Paint the appropriate border color.                                     *
  ***************************************************************************/

  PaintBorder ( hwnd, hPS, Data, MustPaint ) ;

 /***************************************************************************
  * If analog or minimized . . .                                            *
  ***************************************************************************/

  if ( Data->IniData.Analog OR ( Data->IniData.Position.fl & SWP_MINIMIZE ) ) {

   /*************************************************************************
    * Draw hour and minute marks around the dial.                           *
    *************************************************************************/

    GpiSetColor ( hPS, Data->IniData.TextColor ) ;

    POINTL Points [3] ;
    for ( int Angle=0; Angle<60; Angle++ ) {
      Points[0].x = 0 ;
      Points[0].y = 90 ;
      RotatePoint ( Points, 1, short(Angle) ) ;
      ScalePoint ( Points, 1, &Data->wi ) ;
      TranslatePoint ( Points, 1, &Data->wi ) ;
      Points[2].x = Points[2].y = Angle % 5 ? 2 : 10 ;

      ScalePoint ( Points + 2, 1, &Data->wi ) ;

      Points[0].x -= Points[2].x / 2 ;
      Points[0].y -= Points[2].y / 2 ;

      Points[1].x = Points[0].x + Points[2].x ;
      Points[1].y = Points[0].y + Points[2].y ;

      GpiMove ( hPS, Points ) ;

      if ( ( Angle % 5 == 0 ) OR
        ( ( Data->wi.cxClient ) > 50 ) AND ( Data->wi.cyClient > 50 ) )
      {
        GpiBox ( hPS, DRO_OUTLINEFILL, Points+1, Points[2].x, Points[2].y ) ;
      }
    }
  }
}

/****************************************************************************
 *                                                                          *
 *                            Paint Border                                  *
 *                                                                          *
 ****************************************************************************/

static void PaintBorder ( HWND hwnd, HPS hPS, PDATA Data, BOOL MustPaint ) {

 /***************************************************************************
  * Determine level of the current alert type.                              *
  ***************************************************************************/

  USHORT Level ;
  if ( Data->IniData.AlertType == ALERT_TASKCOUNT ) {
    Level = USHORT ( WinQuerySwitchList ( Data->Proc->QueryAnchor(), 0, 0 ) ) ;
  } else {
    Data->MaxCount = ULONG ( max ( Data->MaxCount, Data->IdleCount ) ) ;
    Level = USHORT ( ( ( Data->MaxCount - Data->IdleCount ) * 100 ) / Data->MaxCount ) ;
  } /* endif */

 /***************************************************************************
  * Determine alert class.                                                  *
  ***************************************************************************/

  USHORT Alert ;
  if ( Level < Data->IniData.AlertLevels[Data->IniData.AlertType][0] )
    Alert = 0 ;
  else if ( Level < Data->IniData.AlertLevels[Data->IniData.AlertType][1] )
    Alert = 1 ;
  else
    Alert = 2 ;

 /***************************************************************************
  * If border must be painted, or the alert level has changed, paint it.    *
  ***************************************************************************/

  if ( MustPaint OR ( Alert != Data->Alert ) ) {
    RECTL Rectangle ;
    WinQueryWindowRect ( hwnd, &Rectangle ) ;

    GpiSetColor ( hPS, Alert > 1 ? RGB_RED : ( Alert ? RGB_YELLOW : RGB_GREEN ) ) ;

    POINTL Point = { 0, 0 } ;
    GpiMove ( hPS, &Point ) ;

    Point.x = Rectangle.xRight - 1 ;
    Point.y = Rectangle.yTop - 1 ;
    GpiBox ( hPS, DRO_OUTLINE, &Point, 0L, 0L ) ;

    Point.x = 1 ;
    Point.y = 1 ;
    GpiMove ( hPS, &Point ) ;

    Point.x = Rectangle.xRight - 2 ;
    Point.y = Rectangle.yTop - 2 ;
    GpiBox ( hPS, DRO_OUTLINE, &Point, 0L, 0L ) ;

    Data->Alert = Alert ;
  }
}

/****************************************************************************
 *                                                                          *
 *                          Paint Digital Time                              *
 *                                                                          *
 ****************************************************************************/

static void PaintDigitalTime ( HWND hwnd, HPS hPS, PDATA Data, PDATETIME DateTime ) {

 /***************************************************************************
  * Determine the window rectangle, less the border.                        *
  ***************************************************************************/

  RECTL Rectangle ;
  WinQueryWindowRect ( hwnd, &Rectangle ) ;

  Rectangle.xLeft += 2 ;
  Rectangle.xRight -= 2 ;
  Rectangle.yBottom += 2 ;
  Rectangle.yTop -= 2 ;

 /***************************************************************************
  * Draw the new time within the rectangle.                                 *
  ***************************************************************************/

  CHAR Text [20] ;

  if ( ( DateTime->minutes == 0 ) AND ( DateTime->hours % 12 == 0 ) ) {
    if ( DateTime->hours == 0 ) {
      ResourceString Midnight ( Data->Library->QueryHandle(), IDS_MIDNIGHT ) ;
      strcpy ( Text, PCHAR(Midnight) ) ;
    } else if ( DateTime->hours == 12 ) {
      ResourceString Noon ( Data->Library->QueryHandle(), IDS_NOON ) ;
      strcpy ( Text, PCHAR(Noon) ) ;
    }
  } else {
    if ( Data->IniData.Hour24 ) {

      sprintf ( Text, "%u%s%02u",
        DateTime->hours, Data->CountryInfo.szTimeSeparator, DateTime->minutes ) ;

      if ( Data->IniData.Seconds ) {
         sprintf ( Text+strlen(Text), "%s%02u", Data->CountryInfo.szTimeSeparator, DateTime->seconds ) ;
      } /* endif */

    } else {

      USHORT Hour = USHORT ( DateTime->hours % 12 ) ;

      if ( Hour == 0 )
        Hour = 12 ;

      sprintf ( Text, "%u%s%02u",
        Hour, Data->CountryInfo.szTimeSeparator, DateTime->minutes ) ;

      if ( Data->IniData.Seconds ) {
         sprintf ( Text+strlen(Text), "%s%02u", Data->CountryInfo.szTimeSeparator, DateTime->seconds ) ;
      } /* endif */

      sprintf ( Text+strlen(Text), "%s",
        (DateTime->hours>=12) ? PSZ(Data->szPm) : PSZ(Data->szAm) ) ;
    }
  }

  WinDrawText ( hPS, strlen(Text), PSZ(Text), &Rectangle,
    Data->IniData.TextColor, Data->IniData.BackColor,
    DT_CENTER | DT_VCENTER | DT_ERASERECT ) ;
}

/****************************************************************************
 *                                                                          *
 *                      Hide Window Controls                                *
 *                                                                          *
 ****************************************************************************/

static void HideControls (
  BOOL fHide,
  HWND FrameWindow,
  HWND hwndSysMenu,
  HWND hwndTitleBar,
  HWND hwndMinMax
) {

 /***************************************************************************
  * Get original window position and state.                                 *
  ***************************************************************************/

  SWP OldPosition ;
  WinQueryWindowPos ( FrameWindow, &OldPosition ) ;

 /***************************************************************************
  * Restore and hide the window.                                            *
  ***************************************************************************/

  WinSetWindowPos ( FrameWindow, HWND(NULL), 0, 0, 0, 0, SWP_RESTORE | SWP_HIDE ) ;

 /***************************************************************************
  * Determine client window and location.                                   *
  ***************************************************************************/

  SWP Position ;
  WinQueryWindowPos ( FrameWindow, &Position ) ;

  RECTL Rectangle ;
  Rectangle.xLeft   = Position.x ;
  Rectangle.xRight  = Position.x + Position.cx ;
  Rectangle.yBottom = Position.y ;
  Rectangle.yTop    = Position.y + Position.cy ;

  WinCalcFrameRect ( FrameWindow, &Rectangle, TRUE ) ;

 /***************************************************************************
  * Hide or reveal the controls windows by changing their parentage.        *
  ***************************************************************************/

  if ( fHide )
  {
    WinSetParent ( hwndSysMenu,  HWND_OBJECT, FALSE ) ;
    WinSetParent ( hwndTitleBar, HWND_OBJECT, FALSE ) ;
    WinSetParent ( hwndMinMax,   HWND_OBJECT, FALSE ) ;
  }
  else
  {
    WinSetParent ( hwndSysMenu,  FrameWindow, TRUE ) ;
    WinSetParent ( hwndTitleBar, FrameWindow, TRUE ) ;
    WinSetParent ( hwndMinMax,   FrameWindow, TRUE ) ;
  }

 /***************************************************************************
  * Tell the frame that things have changed.  Let it update the window.     *
  ***************************************************************************/

  WinSendMsg ( FrameWindow, WM_UPDATEFRAME,
    MPFROMSHORT ( FCF_TITLEBAR | FCF_SYSMENU | FCF_MINBUTTON ), 0L ) ;

 /***************************************************************************
  * Reposition the frame around the client window, which is left be.        *
  ***************************************************************************/

  WinCalcFrameRect ( FrameWindow, &Rectangle, FALSE ) ;

  WinSetWindowPos ( FrameWindow, HWND(NULL),
    (SHORT) Rectangle.xLeft,  (SHORT) Rectangle.yBottom,
    (SHORT) (Rectangle.xRight-Rectangle.xLeft),
    (SHORT) (Rectangle.yTop-Rectangle.yBottom),
    SWP_SIZE | SWP_MOVE ) ;

 /***************************************************************************
  * If window was maximized, put it back that way.                          *
  ***************************************************************************/

  if ( OldPosition.fl & SWP_MAXIMIZE )
  {
    WinSetWindowPos ( FrameWindow, HWND(NULL),
      (SHORT) Rectangle.xLeft,  (SHORT) Rectangle.yBottom,
      (SHORT) (Rectangle.xRight-Rectangle.xLeft),
      (SHORT) (Rectangle.yTop-Rectangle.yBottom),
      SWP_SIZE | SWP_MOVE |
      ( OldPosition.fl & SWP_MAXIMIZE ) ) ;
  }

 /***************************************************************************
  * Reveal the window to the curious world.                                 *
  ***************************************************************************/

  WinShowWindow ( FrameWindow, TRUE ) ;
}

/****************************************************************************
 *                                                                          *
 *    Monitor Loop Thread                                                   *
 *                                                                          *
 ****************************************************************************/

static void _Optlink MonitorThread ( PVOID Parameter ) {

  /**************************************************************************
   * Register the exception handler.                                        *
   **************************************************************************/

   EXCEPTIONREGISTRATIONRECORD ExceptionRecord = { 0, ExceptionHandler } ;
   DosSetExceptionHandler ( &ExceptionRecord ) ;

  /**************************************************************************
   * Get the thread parameter.                                              *
   **************************************************************************/

   PMONITOR_PARMS Parms = PMONITOR_PARMS ( Parameter ) ;

  /**************************************************************************
   * Start up the high resolution timer, if it is available.                *
   **************************************************************************/

   BOOL HiResTimer = OpenTimer ( ) ;

  /**************************************************************************
   * Loop while active . . .                                                *
   **************************************************************************/

   while ( Parms->Active ) {

     /***********************************************************************
      * Reset the last time and count seen.                                 *
      ***********************************************************************/

      ULONG LastMilliseconds ;
      TIMESTAMP Time [2] ;

      if ( HiResTimer )
         GetTime ( &Time[0] ) ;
      else
         DosQuerySysInfo ( QSV_MS_COUNT, QSV_MS_COUNT, &LastMilliseconds, sizeof(LastMilliseconds) ) ;

      ULONG LastCounter = *Parms->Counter ;

     /***********************************************************************
      * Let the counter count.                                              *
      ***********************************************************************/

      DosSleep ( 1000 ) ;

     /***********************************************************************
      * Find out how much time and counts went by.                          *
      ***********************************************************************/

      ULONG CurrentCounter = *Parms->Counter ;

      ULONG DeltaMilliseconds ;

      if ( HiResTimer ) {
         GetTime ( &Time[1] ) ;

         ULONG Nanoseconds ;
         DeltaMilliseconds = ComputeElapsedTime ( &Time[0], &Time[1], &Nanoseconds ) ;

         if ( Nanoseconds >= 500000L )
            DeltaMilliseconds ++ ;
      } else {
         ULONG Milliseconds ;
         DosQuerySysInfo ( QSV_MS_COUNT, QSV_MS_COUNT, &Milliseconds, sizeof(Milliseconds) ) ;
         DeltaMilliseconds = Milliseconds - LastMilliseconds ;
      } /* endif */

     /***********************************************************************
      * Find out how much idle time was counted.  Adjust it to persecond.   *
      ***********************************************************************/

      ULONG Counter = (ULONG) ( ( (double)(CurrentCounter-LastCounter) * 1000L ) / (double)DeltaMilliseconds ) ;

     /***********************************************************************
      * Tell the owner window to refresh its statistics.                    *
      ***********************************************************************/

      WinPostMsg ( Parms->Owner, WM_REFRESH, MPFROMLONG(Counter), 0L ) ;

   } /* endwhile */

  /*************************************************************************
   * Deregister the exception handler.                                     *
   *************************************************************************/

   DosUnsetExceptionHandler ( &ExceptionRecord ) ;
}

/****************************************************************************
 *                                                                          *
 *                       Calibrate the Load Meter                           *
 *                                                                          *
 ****************************************************************************/

static ULONG CalibrateLoadMeter ( void ) {

 /***************************************************************************
  * Set result to zero as a default.                                        *
  ***************************************************************************/

  double AdjustedMaxLoad = 0.0 ;

 /***************************************************************************
  * If HRTIMER.SYS has been installed . . .                                 *
  ***************************************************************************/

  if ( OpenTimer ( ) ) {

   /*************************************************************************
    * Increase this thread's priority to the maximum.                       *
    *************************************************************************/

    DosSetPriority ( PRTYS_THREAD, PRTYC_TIMECRITICAL, PRTYD_MAXIMUM, 0 ) ;

   /*************************************************************************
    * Create the calibration thread and set its priority next highest.      *
    *************************************************************************/

    COUNTER_PARMS Parms ;
    ULONG MaxLoad ;
    Parms.Active = TRUE ;
    Parms.Counter = &MaxLoad ;
    TID tidCalibrate = _beginthread ( CounterThread, NULL, 0x3000, &Parms ) ;
    DosSetPriority ( PRTYS_THREAD, PRTYC_TIMECRITICAL, PRTYD_MAXIMUM-1, tidCalibrate ) ;
    DosSuspendThread ( tidCalibrate ) ;

   /*************************************************************************
    * Reset the calibration count, get the time, and let the counter go.    *
    *************************************************************************/

    MaxLoad = 0 ;
    TIMESTAMP Time[2] ;
    GetTime ( &Time[0] ) ;
    DosResumeThread ( tidCalibrate ) ;

   /*************************************************************************
    * Sleep for one second.                                                 *
    *************************************************************************/

    DosSleep ( 1000 ) ;

   /*************************************************************************
    * Suspend the calibration counter and get the time.                     *
    *************************************************************************/

    Parms.Active = FALSE ;
    GetTime ( &Time[1] ) ;
    DosWaitThread ( &tidCalibrate, DCWW_WAIT ) ;

   /*************************************************************************
    * Return priorities to normal.                                          *
    *************************************************************************/

    DosSetPriority ( PRTYS_THREAD, PRTYC_REGULAR, 0, 0 ) ;

   /*************************************************************************
    * Get the elapsed time and adjust the calibration count.                *
    *************************************************************************/

    ULONG Milliseconds ;
    ULONG Nanoseconds ;
    Milliseconds = ComputeElapsedTime ( &Time[0], &Time[1], &Nanoseconds ) ;

    AdjustedMaxLoad = (double)MaxLoad * 1.0E9 ;
    AdjustedMaxLoad /= (double)Milliseconds*1.0E6L + (double)Nanoseconds ;

   /*************************************************************************
    * Close down the connection to HRTIMER.SYS.                             *
    *************************************************************************/

    CloseTimer ( ) ;
  }

 /***************************************************************************
  * Return the adjusted calibration count.  If HRTIMER was not there, it    *
  *   will be zero.                                                         *
  ***************************************************************************/

  return ( (ULONG)AdjustedMaxLoad ) ;
}

/****************************************************************************
 *                                                                          *
 *                    General Purpose Counter Thread                        *
 *                                                                          *
 ****************************************************************************/

static void _Optlink CounterThread ( PVOID Parameter ) {

  /*************************************************************************
   * Register the exception handler.                                       *
   *************************************************************************/

   EXCEPTIONREGISTRATIONRECORD ExceptionRecord = { 0, ExceptionHandler } ;
   DosSetExceptionHandler ( &ExceptionRecord ) ;

  /*************************************************************************
   * Count like mad.                                                       *
   *************************************************************************/

   PCOUNTER_PARMS Parms = PCOUNTER_PARMS ( Parameter ) ;
   while ( Parms->Active ) {
      Parms->Counter ++ ;
   } /* endwhile */

  /*************************************************************************
   * Deregister the exception handler.                                     *
   *************************************************************************/

   DosUnsetExceptionHandler ( &ExceptionRecord ) ;
}
