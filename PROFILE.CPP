/**************************************************************** PROFILE.CPP
 *                                                                          *
 *                     Profile (INI) File Object Class                      *
 *                                                                          *
 ****************************************************************************/

#define INCL_BASE
#define INCL_PM
#include <os2.h>

#include <string.h>

#include "debug.h"
#include "support.h"

#include "profile.h"


/****************************************************************************
 *                                                                          *
 *                     Definitions & Declarations                           *
 *                                                                          *
 ****************************************************************************/

  // Constants

enum { ENTRY, ERR } ;


  // Type Definitions

typedef struct {
  SHORT   id ;
  HWND    hwndHelp ;
  PBYTE   Path ;
  int     PathSize ;
} PROFILE_PARMS, *PPROFILE_PARMS ;


  // Function Prototypes

static FNWP ProfileProcessor ;

static FNWP InitDlg ;
static FNWP Command ;
static FNWP OK ;
static FNWP Cancel ;


/****************************************************************************
 *                                                                          *
 *      Profile Class Constructor                                           *
 *                                                                          *
 ****************************************************************************/

Profile::Profile
(
  PSZ     name,
  HAB     Anchor,
  HMODULE Library,
  int     DialogID,
  HWND    HelpInstance,
  BOOL    ResetFlag
) {

  /**************************************************************************
   * Save the names.                                                        *
   **************************************************************************/

   Name = new BYTE [ strlen(PCHAR(name)) + 1 ] ;
   strcpy ( PCHAR(Name), PCHAR(name) ) ;

  /**************************************************************************
   * If resetting the profile, clear the system profile information now.    *
   **************************************************************************/

   if ( ResetFlag ) {
      PrfWriteProfileData ( HINI_USERPROFILE, Name, PSZ(NULL), PSZ(NULL), 0 ) ;
   }

  /**************************************************************************
   * Query the system INI for the profile file's path.                      *
   **************************************************************************/

   PSZ ProfilePath = PSZ(NULL) ;
   ULONG Size ;

   if ( PrfQueryProfileSize ( HINI_USERPROFILE, Name, PSZ("INIPATH"), &Size ) ) {

      // The info exists.  Fetch it.
      ProfilePath = new BYTE [ Size ] ;
      PrfQueryProfileData ( HINI_USERPROFILE, Name,
         PSZ("INIPATH"), ProfilePath, &Size ) ;

      // Build the profile file name.
      BYTE FullPath [_MAX_PATH] ;
      strcpy ( PCHAR(FullPath), PCHAR(ProfilePath) ) ;
      strcat ( PCHAR(FullPath), "\\" ) ;
      strcat ( PCHAR(FullPath), PCHAR(Name) ) ;
      strcat ( PCHAR(FullPath), ".INI" ) ;

      // Clean the name up and expand it to a full path.
      BYTE Path [256] ;
      DosQueryPathInfo ( FullPath, FIL_QUERYFULLNAME, Path, sizeof(Path) ) ;

      // Does the file exist?  If not, discard the name.
      FILESTATUS3 Status ;
      if ( DosQueryPathInfo ( Path, FIL_STANDARD, &Status, sizeof(Status) ) ) {
         delete [] ProfilePath ;
         ProfilePath = PSZ(NULL) ;
      }
   }

  /**************************************************************************
   * If the profile file couldn't be found, ask the user for a path.        *
   **************************************************************************/

   if ( ProfilePath == NULL ) {
      // Set the default path.
      BYTE Path [256] ;
      DosQueryPathInfo ( PSZ("."), FIL_QUERYFULLNAME, Path, sizeof(Path) ) ;

      // Call up the entry dialog.
      PROFILE_PARMS Parms ;
      Parms.id = short ( DialogID ) ;
      Parms.hwndHelp = HelpInstance ;
      Parms.Path = Path ;
      Parms.PathSize = sizeof(Path) ;
      if ( WinDlgBox ( HWND_DESKTOP, HWND_DESKTOP, PFNWP(ProfileProcessor), Library, DialogID, &Parms ) ) {
         // If OK, save the approved path in the system profile.
         ProfilePath = new BYTE [ strlen(PCHAR(Path)) + 1 ] ;
         strcpy ( PCHAR(ProfilePath), PCHAR(Path) ) ;

         PrfWriteProfileData ( HINI_USERPROFILE, Name, PSZ("INIPATH"),
            ProfilePath, strlen(PCHAR(ProfilePath))+1 ) ;
      }
   }

  /**************************************************************************
   * Reset profile handle.  If the path could be determined . . .           *
   **************************************************************************/

   Handle = 0 ;

   if ( ProfilePath ) {

     /***********************************************************************
      * Build the full profile file name.                                   *
      ***********************************************************************/

      BYTE ProfileName [_MAX_PATH] ;
      strcpy ( PCHAR(ProfileName), PCHAR(ProfilePath) ) ;
      strcat ( PCHAR(ProfileName), "\\"  ) ;
      strcat ( PCHAR(ProfileName), PCHAR(Name) ) ;
      strcat ( PCHAR(ProfileName), ".INI" ) ;

     /***********************************************************************
      * Release the memory previously allocated to store the path.          *
      ***********************************************************************/

      if ( ProfilePath ) {
         delete [] ProfilePath ;
      }

     /***********************************************************************
      * Open/Create the profile file and return the resultant handle.       *
      ***********************************************************************/

      Handle = PrfOpenProfile ( Anchor, ProfileName ) ;

     /***********************************************************************
      * If resetting, then clean this profile out.                          *
      ***********************************************************************/

      if ( ResetFlag ) {
         PrfWriteProfileData ( Handle, Name, PSZ(NULL), PSZ(NULL), 0 ) ;
      }
   }
}

/****************************************************************************
 *                                                                          *
 *      Profile Class Destructor                                            *
 *                                                                          *
 ****************************************************************************/

Profile::~Profile ( ) {

 /***************************************************************************
  * Release allocated memory.                                               *
  ***************************************************************************/

  delete [] Name ;

 /***************************************************************************
  * Close the profile.                                                      *
  ***************************************************************************/

  if ( Handle )
     PrfCloseProfile ( Handle ) ;
}

/****************************************************************************
 *                                                                          *
 *      Dialog Message Processor                                            *
 *                                                                          *
 ****************************************************************************/

extern MRESULT EXPENTRY ProfileProcessor ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

 /***************************************************************************
  * Dispatch the message according to the method table and return the       *
  *   result.  Any messages not defined above get handled by the system     *
  *   default dialog processor.                                             *
  ***************************************************************************/

  static METHOD Methods [] = {
    { WM_INITDLG, InitDlg },
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
  * Get parameters from initialization message.                             *
  ***************************************************************************/

  PPROFILE_PARMS Parms = (PPROFILE_PARMS) ( PVOIDFROMMP ( mp2 ) ) ;

  WinSetWindowPtr ( hwnd, QWL_USER, Parms ) ;

 /***************************************************************************
  * Set the dialog help instance.                                           *
  ***************************************************************************/

  WinSetWindowUShort ( hwnd, QWS_ID, Parms->id ) ;
  if ( Parms->hwndHelp ) {
    WinAssociateHelpInstance ( Parms->hwndHelp, hwnd ) ;
  }

 /***************************************************************************
  * Set the entry field contents.                                           *
  ***************************************************************************/

  WinSetDlgItemText ( hwnd, Parms->id+ENTRY, Parms->Path ) ;

 /***************************************************************************
  * Return no error.                                                        *
  ***************************************************************************/

  return ( MRFROMSHORT ( FALSE ) ) ;
}

/****************************************************************************
 *                                                                          *
 *      Process commands received by the dialog.                            *
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
 *      Process the dialog's OK button being pressed.                       *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY OK ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

 /***************************************************************************
  * Find the instance data.                                                 *
  ***************************************************************************/

  PPROFILE_PARMS Parms = PPROFILE_PARMS ( WinQueryWindowPtr ( hwnd, QWL_USER ) ) ;

 /***************************************************************************
  * Verify the entered path.                                                *
  ***************************************************************************/

  BYTE Name [256] ;
  WinQueryDlgItemText ( hwnd, Parms->id+ENTRY, sizeof(Name), Name ) ;

  BYTE FullPath [256] ;
  if ( DosQueryPathInfo ( Name, FIL_QUERYFULLNAME, FullPath, sizeof(FullPath) ) ) {
    PSZ Message = PSZ ( "ERROR: Not a valid path." ) ;
    WinSetDlgItemText ( hwnd, Parms->id+ERR, Message ) ;
    WinAlarm ( HWND_DESKTOP, WA_ERROR ) ;
    WinSetFocus ( HWND_DESKTOP, WinWindowFromID ( hwnd, Parms->id+ENTRY ) ) ;
    return ( 0 ) ;
  }

  FILESTATUS3 Status ;
  if ( DosQueryPathInfo ( FullPath, FIL_STANDARD, &Status, sizeof(Status) ) ) {
    PSZ Message = PSZ ( "ERROR: Path does not exist." ) ;
    WinSetDlgItemText ( hwnd, Parms->id+ERR, Message ) ;
    WinAlarm ( HWND_DESKTOP, WA_ERROR ) ;
    WinSetFocus ( HWND_DESKTOP, WinWindowFromID ( hwnd, Parms->id+ENTRY ) ) ;
    return ( 0 ) ;
  }

  if ( ! ( Status.attrFile & FILE_DIRECTORY ) ) {
    PSZ Message = PSZ ( "ERROR: Specified path is not a directory." ) ;
    WinSetDlgItemText ( hwnd, Parms->id+ERR, Message ) ;
    WinAlarm ( HWND_DESKTOP, WA_ERROR ) ;
    WinSetFocus ( HWND_DESKTOP, WinWindowFromID ( hwnd, Parms->id+ENTRY ) ) ;
    return ( 0 ) ;
  }

 /***************************************************************************
  * Return the full path to the caller.                                     *
  ***************************************************************************/

  strncpy ( PCHAR(Parms->Path), PCHAR(FullPath), Parms->PathSize ) ;

 /***************************************************************************
  * Dismiss the dialog with a TRUE status.                                  *
  ***************************************************************************/

  WinDismissDlg ( hwnd, TRUE ) ;

  return ( 0 ) ;
}

/****************************************************************************
 *                                                                          *
 *      Process the dialog's being cancelled.                               *
 *                                                                          *
 ****************************************************************************/

static MRESULT APIENTRY Cancel ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

 /***************************************************************************
  * Dismiss the dialog with a TRUE status.                                  *
  ***************************************************************************/

  WinDismissDlg ( hwnd, FALSE ) ;

  return ( 0 ) ;
}
