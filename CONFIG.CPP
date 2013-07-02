/***************************************************************** CONFIG.CPP
 *                                                                          *
 *                        Clock Configuration Dialog                        *
 *                                                                          *
 ****************************************************************************/

#define INCL_BASE
#define INCL_PM
#define INCL_WINSTDSPIN
#include <os2.h>

#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "support.h"
#include "config.h"


/****************************************************************************
 *                                                                          *
 *                     Definitions & Declarations                           *
 *                                                                          *
 ****************************************************************************/

  // Constants

enum
{
  IDD_ANALOG,
  IDD_DIGITAL,
  IDD_HOUR24,
  IDD_HIDECONTROLS,
  IDD_CHIME,
  IDD_FLOAT,
  IDD_ANIMATE,
  IDD_SECONDS,
  IDD_TASKCOUNT,
  IDD_CPULOAD,
  IDD_TASKCOUNTRED,
  IDD_TASKCOUNTYELLOW,
  IDD_CPULOADRED,
  IDD_CPULOADYELLOW
} ;


  // Type Definitions

typedef struct {
  int    Id ;
  BOOL   Analog ;
  BOOL   Hour24 ;
  BOOL   HideControls ;
  BOOL   Chime ;
  BOOL   Float ;
  BOOL   Animate ;
  BOOL   Seconds ;
  USHORT AlertType ;
  USHORT AlertLevels [2] [2] ;
  PCONFIG_PARMS Parms ;
} DATA, *PDATA ;


  // Function Prototypes

static FNWP InitDlg ;
static FNWP Control ;
static FNWP Command ;
static FNWP OK ;
static FNWP Cancel ;


  // Global Data (local to this module)

static DATA Data ;


/****************************************************************************
 *                                                                          *
 *      "Configure" Dialog Processor                                        *
 *                                                                          *
 ****************************************************************************/

extern MRESULT EXPENTRY ConfigureProcessor ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

 /***************************************************************************
  * Dispatch the message according to the method table and return the       *
  *   result.  Any messages not defined above get handled by the system     *
  *   default dialog processor.                                             *
  ***************************************************************************/

  static METHOD Methods [] = {
    { WM_INITDLG, InitDlg },
    { WM_CONTROL, Control },
    { WM_COMMAND, Command }
  } ;

  return ( DispatchMessage ( hwnd, msg, mp1, mp2, Methods, sizeof(Methods)/sizeof(Methods[0]), WinDefDlgProc ) ) ;
}

/****************************************************************************
 *                                                                          *
 *      Initialize Dialog                                                   *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY InitDlg ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

 /***************************************************************************
  * Get initial parameters.                                                 *
  ***************************************************************************/

  PCONFIG_PARMS Parms = (PCONFIG_PARMS) ( PVOIDFROMMP ( mp2 ) ) ;

  Data.Id           = Parms->Id ;
  Data.Analog       = Parms->Analog ;
  Data.Hour24       = Parms->Hour24 ;
  Data.HideControls = Parms->HideControls ;
  Data.Chime        = Parms->Chime ;
  Data.Float        = Parms->Float ;
  Data.Animate      = Parms->Animate ;
  Data.Seconds      = Parms->Seconds ;
  Data.AlertType    = Parms->AlertType ;

  memcpy ( Data.AlertLevels, Parms->AlertLevels, sizeof(Data.AlertLevels) ) ;

  Data.Parms = Parms ;

 /***************************************************************************
  * Associate the help instance.                                            *
  ***************************************************************************/

  WinAssociateHelpInstance ( WinQueryHelpInstance(WinQueryWindow(hwnd,QW_OWNER)), hwnd ) ;

 /***************************************************************************
  * Set the radio button and checkbox values.                               *
  ***************************************************************************/

  WinSendDlgItemMsg ( hwnd, Data.Parms->Id+IDD_ANALOG,
    BM_SETCHECK, MPFROMSHORT(Data.Analog==TRUE), 0 ) ;

  WinSendDlgItemMsg ( hwnd, Data.Parms->Id+IDD_DIGITAL,
    BM_SETCHECK, MPFROMSHORT(Data.Analog==FALSE), 0 ) ;

  WinSendDlgItemMsg ( hwnd, Data.Parms->Id+IDD_HOUR24,
    BM_SETCHECK, MPFROMSHORT(Data.Hour24), 0 ) ;

  WinSendDlgItemMsg ( hwnd, Data.Parms->Id+IDD_HIDECONTROLS,
    BM_SETCHECK, MPFROMSHORT(Data.HideControls), 0 ) ;

  WinSendDlgItemMsg ( hwnd, Data.Parms->Id+IDD_CHIME,
    BM_SETCHECK, MPFROMSHORT(Data.Chime), 0 ) ;

  WinSendDlgItemMsg ( hwnd, Data.Parms->Id+IDD_FLOAT,
    BM_SETCHECK, MPFROMSHORT(Data.Float), 0 ) ;

  WinSendDlgItemMsg ( hwnd, Data.Parms->Id+IDD_ANIMATE,
    BM_SETCHECK, MPFROMSHORT(Data.Animate), 0 ) ;

  WinSendDlgItemMsg ( hwnd, Data.Parms->Id+IDD_SECONDS,
    BM_SETCHECK, MPFROMSHORT(Data.Seconds), 0 ) ;

  WinSendDlgItemMsg ( hwnd, Data.Parms->Id+IDD_TASKCOUNT,
    BM_SETCHECK, MPFROMSHORT(Data.AlertType==0), 0 ) ;

  WinSendDlgItemMsg ( hwnd, Data.Parms->Id+IDD_CPULOAD,
    BM_SETCHECK, MPFROMSHORT(Data.AlertType==1), 0 ) ;

 /***************************************************************************
  * Set the limits and initial values of the spin-button controls.          *
  ***************************************************************************/

  WinSendDlgItemMsg ( hwnd, Data.Parms->Id+IDD_TASKCOUNTRED,
    SPBM_SETLIMITS, (MPARAM)100, (MPARAM)(Data.AlertLevels[0][0]+1) ) ;

  WinSendDlgItemMsg ( hwnd, Data.Parms->Id+IDD_TASKCOUNTYELLOW,
    SPBM_SETLIMITS, (MPARAM)(Data.AlertLevels[0][1]-1), (MPARAM)0 ) ;

  WinSendDlgItemMsg ( hwnd, Data.Parms->Id+IDD_CPULOADRED,
    SPBM_SETLIMITS, (MPARAM)100, (MPARAM)(Data.AlertLevels[1][0]+1) ) ;

  WinSendDlgItemMsg ( hwnd, Data.Parms->Id+IDD_CPULOADYELLOW,
    SPBM_SETLIMITS, (MPARAM)(Data.AlertLevels[1][1]-1), (MPARAM)0 ) ;

  WinSendDlgItemMsg ( hwnd, Data.Parms->Id+IDD_TASKCOUNTRED,
    SPBM_SETCURRENTVALUE, (MPARAM)Data.AlertLevels[0][1], NULL ) ;

  WinSendDlgItemMsg ( hwnd, Data.Parms->Id+IDD_TASKCOUNTYELLOW,
    SPBM_SETCURRENTVALUE, (MPARAM)Data.AlertLevels[0][0], NULL ) ;

  WinSendDlgItemMsg ( hwnd, Data.Parms->Id+IDD_CPULOADRED,
    SPBM_SETCURRENTVALUE, (MPARAM)Data.AlertLevels[1][1], NULL ) ;

  WinSendDlgItemMsg ( hwnd, Data.Parms->Id+IDD_CPULOADYELLOW,
    SPBM_SETCURRENTVALUE, (MPARAM)Data.AlertLevels[1][0], NULL ) ;

 /***************************************************************************
  * Set input focus to the first group.                                     *
  ***************************************************************************/

  if ( Data.Analog )
    WinSetFocus ( HWND_DESKTOP, WinWindowFromID ( hwnd, Data.Parms->Id+IDD_ANALOG ) ) ;
  else
    WinSetFocus ( HWND_DESKTOP, WinWindowFromID ( hwnd, Data.Parms->Id+IDD_DIGITAL ) ) ;

 /***************************************************************************
  * Return without error.                                                   *
  ***************************************************************************/

  return ( MRFROMSHORT ( FALSE ) ) ;
}

/****************************************************************************
 *                                                                          *
 *      Process control notifications                                       *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Control ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

 /***************************************************************************
  * Decode the message.  Find out what control sent it, and what the        *
  *   control had to say.                                                   *
  ***************************************************************************/

  USHORT id = SHORT1FROMMP ( mp1 ) ;
  USHORT Message = SHORT2FROMMP ( mp1 ) ;

 /***************************************************************************
  * Here we process messages from the spin buttons.                         *
  ***************************************************************************/

  LONG Long ;

  if ( id == (USHORT) ( Data.Parms->Id+IDD_TASKCOUNTRED ) ) {
    if ( ( Message == SPBN_CHANGE )
      OR ( Message == SPBN_UPARROW )
      OR ( Message == SPBN_DOWNARROW ) ) {
      WinSendDlgItemMsg ( hwnd, Data.Parms->Id+IDD_TASKCOUNTRED,
        SPBM_QUERYVALUE, &Long, MPFROM2SHORT(0,SPBQ_UPDATEIFVALID) ) ;
      WinSendDlgItemMsg ( hwnd, Data.Parms->Id+IDD_TASKCOUNTYELLOW,
        SPBM_SETLIMITS, (MPARAM)(Long-1), (MPARAM)0 ) ;
    }
  } else if ( id == (USHORT) ( Data.Parms->Id+IDD_TASKCOUNTYELLOW ) ) {
    if ( ( Message == SPBN_CHANGE )
      OR ( Message == SPBN_UPARROW )
      OR ( Message == SPBN_DOWNARROW ) ) {
      WinSendDlgItemMsg ( hwnd, Data.Parms->Id+IDD_TASKCOUNTYELLOW,
        SPBM_QUERYVALUE, &Long, MPFROM2SHORT(0,SPBQ_UPDATEIFVALID) ) ;
      WinSendDlgItemMsg ( hwnd, Data.Parms->Id+IDD_TASKCOUNTRED,
        SPBM_SETLIMITS, (MPARAM)100, (MPARAM)(Long+1) ) ;
    }
  } else if ( id == (USHORT) ( Data.Parms->Id+IDD_CPULOADRED ) ) {
    if ( ( Message == SPBN_CHANGE )
      OR ( Message == SPBN_UPARROW )
      OR ( Message == SPBN_DOWNARROW ) ) {
      WinSendDlgItemMsg ( hwnd, Data.Parms->Id+IDD_CPULOADRED,
        SPBM_QUERYVALUE, &Long, MPFROM2SHORT(0,SPBQ_UPDATEIFVALID) ) ;
      WinSendDlgItemMsg ( hwnd, Data.Parms->Id+IDD_CPULOADYELLOW,
        SPBM_SETLIMITS, (MPARAM)(Long-1), (MPARAM)0 ) ;
    }
  } else if ( id == (USHORT) ( Data.Parms->Id+IDD_CPULOADYELLOW ) ) {
    if ( ( Message == SPBN_CHANGE )
      OR ( Message == SPBN_UPARROW )
      OR ( Message == SPBN_DOWNARROW ) ) {
      WinSendDlgItemMsg ( hwnd, Data.Parms->Id+IDD_CPULOADYELLOW,
        SPBM_QUERYVALUE, &Long, MPFROM2SHORT(0,SPBQ_UPDATEIFVALID) ) ;
      WinSendDlgItemMsg ( hwnd, Data.Parms->Id+IDD_CPULOADRED,
        SPBM_SETLIMITS, (MPARAM)100, (MPARAM)(Long+1) ) ;
    }
  }

 /***************************************************************************
  * Return without error.                                                   *
  ***************************************************************************/

  return ( MRFROMSHORT ( FALSE ) ) ;
}

/****************************************************************************
 *                                                                          *
 *      Process commands received by the Configure Dialog                       *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Command ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

 /***************************************************************************
  * Dispatch the message without a default message processor.               *
  ***************************************************************************/

  static METHOD Methods [] = {
    { DID_OK,     OK     },
    { DID_CANCEL, Cancel },
  } ;

  return ( DispatchMessage ( hwnd, SHORT1FROMMP(mp1), mp1, mp2, Methods, sizeof(Methods)/sizeof(Methods[0]), 0 ) ) ;
}

/****************************************************************************
 *                                                                          *
 *      Process the Configure Dialog's OK button being pressed.             *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY OK ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

 /***************************************************************************
  * Query the buttons for their new settings.                               *
  ***************************************************************************/

  Data.Analog = (BOOL) SHORT1FROMMR ( WinSendDlgItemMsg ( hwnd,
    Data.Parms->Id+IDD_ANALOG, BM_QUERYCHECK, 0L, 0L ) ) ;

  Data.Hour24 = (BOOL) SHORT1FROMMR ( WinSendDlgItemMsg ( hwnd,
    Data.Parms->Id+IDD_HOUR24, BM_QUERYCHECK, 0L, 0L ) ) ;

  Data.HideControls = (BOOL) SHORT1FROMMR ( WinSendDlgItemMsg ( hwnd,
    Data.Parms->Id+IDD_HIDECONTROLS, BM_QUERYCHECK, 0L, 0L ) ) ;

  Data.Chime = (BOOL) SHORT1FROMMR ( WinSendDlgItemMsg ( hwnd,
    Data.Parms->Id+IDD_CHIME, BM_QUERYCHECK, 0L, 0L ) ) ;

  Data.Float = (BOOL) SHORT1FROMMR ( WinSendDlgItemMsg ( hwnd,
    Data.Parms->Id+IDD_FLOAT, BM_QUERYCHECK, 0L, 0L ) ) ;

  Data.Animate = (BOOL) SHORT1FROMMR ( WinSendDlgItemMsg ( hwnd,
    Data.Parms->Id+IDD_ANIMATE, BM_QUERYCHECK, 0L, 0L ) ) ;

  Data.Seconds = (BOOL) SHORT1FROMMR ( WinSendDlgItemMsg ( hwnd,
    Data.Parms->Id+IDD_SECONDS, BM_QUERYCHECK, 0L, 0L ) ) ;

  Data.AlertType = (USHORT) SHORT1FROMMR ( WinSendDlgItemMsg ( hwnd,
    Data.Parms->Id+IDD_CPULOAD, BM_QUERYCHECK, 0L, 0L ) ) ;

 /***************************************************************************
  * Query the spinbuttons for their new settings.                           *
  ***************************************************************************/

  LONG Long ;

  WinSendDlgItemMsg ( hwnd, Data.Parms->Id+IDD_TASKCOUNTRED,
    SPBM_QUERYVALUE, &Long, MPFROM2SHORT(0,SPBQ_UPDATEIFVALID) ) ;
  Data.AlertLevels[0][1] = (USHORT) Long ;

  WinSendDlgItemMsg ( hwnd, Data.Parms->Id+IDD_TASKCOUNTYELLOW,
    SPBM_QUERYVALUE, &Long, MPFROM2SHORT(0,SPBQ_UPDATEIFVALID) ) ;
  Data.AlertLevels[0][0] = (USHORT) Long ;

  WinSendDlgItemMsg ( hwnd, Data.Parms->Id+IDD_CPULOADRED,
    SPBM_QUERYVALUE, &Long, MPFROM2SHORT(0,SPBQ_UPDATEIFVALID) ) ;
  Data.AlertLevels[1][1] = (USHORT) Long ;

  WinSendDlgItemMsg ( hwnd, Data.Parms->Id+IDD_CPULOADYELLOW,
    SPBM_QUERYVALUE, &Long, MPFROM2SHORT(0,SPBQ_UPDATEIFVALID) ) ;
  Data.AlertLevels[1][0] = (USHORT) Long ;

 /***************************************************************************
  * Save the altered data to the original parameter block.                  *
  ***************************************************************************/

  Data.Parms->Analog       = Data.Analog ;
  Data.Parms->Hour24       = Data.Hour24 ;
  Data.Parms->HideControls = Data.HideControls ;
  Data.Parms->Chime        = Data.Chime ;
  Data.Parms->Float        = Data.Float ;
  Data.Parms->Animate      = Data.Animate ;
  Data.Parms->Seconds      = Data.Seconds ;
  Data.Parms->AlertType    = Data.AlertType ;

  memcpy ( Data.Parms->AlertLevels, Data.AlertLevels, sizeof(Data.AlertLevels) ) ;

 /***************************************************************************
  * Dismiss the dialog with a TRUE status.                                  *
  ***************************************************************************/

  WinDismissDlg ( hwnd, TRUE ) ;

  return ( 0 ) ;
}

/****************************************************************************
 *                                                                          *
 *      Process the Configure Dialog's being cancelled.                     *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Cancel ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

 /***************************************************************************
  * Dismiss the dialog with a TRUE status.                                  *
  ***************************************************************************/

  WinDismissDlg ( hwnd, FALSE ) ;

  return ( 0 ) ;
}
