/****************************************************************** ABOUT.CPP
 *                                                                          *
 *                        Generic "About" Dialog                            *
 *                                                                          *
 ****************************************************************************/

#define INCL_BASE
#define INCL_PM
#include <os2.h>

#include "About.h"


/****************************************************************************
 *                                                                          *
 *      "About" Dialog Processor                                            *
 *                                                                          *
 ****************************************************************************/

extern MRESULT EXPENTRY AboutProcessor ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

   switch ( msg ) {

      case WM_INITDLG:
         WinAssociateHelpInstance ( WinQueryHelpInstance(WinQueryWindow(hwnd,QW_OWNER)), hwnd ) ;
         return ( 0 ) ;

      case WM_COMMAND:
         switch ( SHORT1FROMMP(mp1) ) {
            case DID_OK:
               WinDismissDlg ( hwnd, TRUE ) ;
               break ;
            case DID_CANCEL:
               WinDismissDlg ( hwnd, FALSE ) ;
               break ;
         } /* endswitch */
         return ( 0 ) ;

   } /* endswitch */

   return ( WinDefDlgProc ( hwnd, msg, mp1, mp2 ) ) ;
}
