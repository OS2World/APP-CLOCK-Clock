/****************************************************************** DEBUG.CPP
 *                                                                          *
 *  Debugging Aids                                                          *
 *                                                                          *
 ****************************************************************************/

#define INCL_BASE
#define INCL_WIN
#include <os2.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "debug.h"
#include "mutex.h"

extern HFILE Timer = 0 ;
extern BOOL Trace = FALSE ;


/****************************************************************************
 *                                                                          *
 *                       Display Debug Message                              *
 *                                                                          *
 ****************************************************************************/

extern VOID Debug ( HWND hwnd, char *Message, ... )
{
 /***************************************************************************
  * Local Declarations                                                      *
  ***************************************************************************/

  va_list Marker ;
  char Text [500] ;

 /***************************************************************************
  * Format the debug message.                                               *
  ***************************************************************************/

  va_start ( Marker, Message ) ;
  vsprintf ( Text, Message, Marker ) ;
  va_end ( Marker ) ;

 /***************************************************************************
  * Display the log message and wait for the user to press ENTER.           *
  ***************************************************************************/

  WinMessageBox ( HWND_DESKTOP, hwnd, PSZ(Text), PSZ("Debug"), 0, MB_ENTER ) ;
}

/****************************************************************************
 *                                                                          *
 *                         Log Debug Message                                *
 *                                                                          *
 ****************************************************************************/

Mutex LogSemaphore ( PSZ(NULL) ) ;

extern VOID Log ( char *Message, ... )
{
 /***************************************************************************
  * Serialize this function by requesting its semaphore.                    *
  ***************************************************************************/

  LogSemaphore.Request ( SEM_INDEFINITE_WAIT ) ;

 /***************************************************************************
  * Format the message.                                                     *
  ***************************************************************************/

  va_list Marker ;
  va_start ( Marker, Message ) ;

  static char Buffer [1024] ;
  vsprintf ( Buffer, Message, Marker ) ;

  va_end ( Marker ) ;

 /***************************************************************************
  * Open the log file.                                                      *
  ***************************************************************************/

  FILE *File = fopen ( "CLOCK.LOG", "a" ) ;

 /***************************************************************************
  * If the file got opened, write the message to the log file and close it. *
  ***************************************************************************/

  if ( File ) {
    char Time [9], Date [9] ;
    fprintf ( File, "%s %s %s\n", _strtime(Time), _strdate(Date), Buffer ) ;
    fclose ( File ) ;
  } /* endif */

 /***************************************************************************
  * Release the function semaphore and return.                              *
  ***************************************************************************/

  LogSemaphore.Release ( ) ;
}

/****************************************************************************
 *                                                                          *
 *                          Open Timer for Use                              *
 *                                                                          *
 ****************************************************************************/

extern BOOL OpenTimer ( VOID )
{
  if ( Timer )
    DosClose ( Timer ) ;

  ULONG Action ;
  if ( DosOpen ( (PSZ)"TIMER$", &Timer, &Action, 0, FILE_NORMAL, FILE_OPEN, OPEN_SHARE_DENYNONE, 0 ) )
  {
    return ( FALSE ) ;
  }

  return ( TRUE ) ;
}

/****************************************************************************
 *                                                                          *
 *                              Close Timer                                 *
 *                                                                          *
 ****************************************************************************/

extern VOID CloseTimer ( VOID )
{
  DosClose ( Timer ) ;
}

/****************************************************************************
 *                                                                          *
 *                       Read Time from HRTIMER.SYS                         *
 *                                                                          *
 ****************************************************************************/

extern BOOL GetTime ( PTIMESTAMP pts )
{
  ULONG ByteCount ;

  if ( DosRead ( Timer, pts, sizeof(*pts), &ByteCount ) )
    return ( FALSE ) ;

  return ( TRUE ) ;
}

/****************************************************************************
 *                                                                          *
 *                         Calculate Elapsed Time                           *
 *                                                                          *
 ****************************************************************************/

extern ULONG ComputeElapsedTime ( PTIMESTAMP ptsStart, PTIMESTAMP ptsStop, PULONG pulNs )
{
  ULONG ulMsecs, ulNsecs;
  TIMESTAMP tsStart, tsStop ;

  tsStart = *ptsStart ;                       // De-reference timestamp
                                              //     structures for speed
  tsStop  = *ptsStop ;

  ulMsecs = tsStop.ulMs - tsStart.ulMs ;      // Elapsed milliseconds

  if( tsStart.ulNs > tsStop.ulNs )            // If nanosecond overflow ...
  {
    ulNsecs = (1000000 + tsStop.ulNs) - tsStart.ulNs; // Adjust nanoseconds
    ulMsecs--;                                        // Adjust milliseconds
  }
  else
    ulNsecs = tsStop.ulNs - tsStart.ulNs ;    // No overflow..Elapsed nanos

  *pulNs = ulNsecs ;

  return ( ulMsecs ) ;
}

/****************************************************************************
 *                                                                          *
 *  Allocate Memory                                                         *
 *                                                                          *
 ****************************************************************************/

//#define ALLOCATE_THROUGH_DOS

extern PVOID AllocateMemory ( size_t ByteCount ) {

  #ifdef ALLOCATE_THROUGH_DOS

    PVOID Memory ;
    DosAllocMem ( &Memory, ByteCount, PAG_READ | PAG_WRITE | PAG_COMMIT ) ;
    return ( Memory ) ;

  #else

    return ( malloc ( ByteCount ) ) ;

  #endif
}

/****************************************************************************
 *                                                                          *
 *  Free Memory                                                             *
 *                                                                          *
 ****************************************************************************/

extern VOID FreeMemory ( PVOID Memory )
{
  #ifdef ALLOCATE_THROUGH_DOS
  {
    DosFreeMem ( Memory ) ;
  }
  #else
  {
    free ( Memory ) ;
  }
  #endif
}
