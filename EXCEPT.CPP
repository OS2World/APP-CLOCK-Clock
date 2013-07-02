/**************************************************************** EXCEPT.CPP
 *                                                                         *
 *                            Exception Handler                            *
 *                                                                         *
 ***************************************************************************/

#include <builtin.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define INCL_BASE
#define INCL_PM
#include <os2.h>

#include "clock.h"
#include "debug.h"
#include "restring.h"

#include "except.h"


/***************************************************************************
 *                                                                         *
 *  Exception Handler                                                      *
 *                                                                         *
 ***************************************************************************/

extern ULONG APIENTRY ExceptionHandler (
  PEXCEPTIONREPORTRECORD pExceptionReportRecord,
  PEXCEPTIONREGISTRATIONRECORD pExceptionRegistrationRecord,
  PCONTEXTRECORD pContextRecord,
  PVOID pDispatcherContext
) {
   static BOOL Active = FALSE ;
   static char Message [500] ;
   static PLONG Base ;

   static PTIB2 pTIB2 = PTIB2 ( _getTIBvalue ( offsetof(TIB,tib_ptib2) ) ) ;
   static ULONG ThreadID = pTIB2->tib2_ultid ;

   if ( pExceptionReportRecord->ExceptionNum == XCPT_ACCESS_VIOLATION ) {
      if ( Active ) {
         Log ( "ABORT: Recursion with exception handler." ) ;
         DosExit ( EXIT_PROCESS, 0 ) ;
      } /* endif */
      Active = TRUE ;
      Log
      (
        "ABORT: %s%s%s%s%s%sviolation trying to access address/selector %08lX.\n"
        "  Thread %i.\n"
        "  The registers were as follows:\n"
        "    AX:%08lX  BX:%08lX  CX:%08lX  DX:%08lX\n"
        "    FL:%08lX  DI:%08lX  SI:%08lX  BP:%08lX\n"
        "    CS:%08lX  IP:%08lX  SS:%08lX  SP:%08lX\n"
        "    DS:%08lX  ES:%08lX  FS:%08lX  GS:%08lX",
        ( pExceptionReportRecord->ExceptionInfo[0] & XCPT_READ_ACCESS ) ? "Read " : "",
        ( pExceptionReportRecord->ExceptionInfo[0] & XCPT_WRITE_ACCESS ) ? "Write " : "",
        ( pExceptionReportRecord->ExceptionInfo[0] & XCPT_EXECUTE_ACCESS ) ? "Execute " : "",
        ( pExceptionReportRecord->ExceptionInfo[0] & XCPT_SPACE_ACCESS ) ? "Space " : "",
        ( pExceptionReportRecord->ExceptionInfo[0] & XCPT_LIMIT_ACCESS ) ? "Limit " : "",
        ( pExceptionReportRecord->ExceptionInfo[0] & XCPT_UNKNOWN_ACCESS ) ? "Unknown " : "",
        pExceptionReportRecord->ExceptionInfo[1],
        ThreadID,
        pContextRecord->ctx_RegEax,
        pContextRecord->ctx_RegEbx,
        pContextRecord->ctx_RegEcx,
        pContextRecord->ctx_RegEdx,
        pContextRecord->ctx_EFlags,
        pContextRecord->ctx_RegEdi,
        pContextRecord->ctx_RegEsi,
        pContextRecord->ctx_RegEbp,
        pContextRecord->ctx_SegCs,
        pContextRecord->ctx_RegEip,
        pContextRecord->ctx_SegSs,
        pContextRecord->ctx_RegEsp,
        pContextRecord->ctx_SegDs,
        pContextRecord->ctx_SegEs,
        pContextRecord->ctx_SegFs,
        pContextRecord->ctx_SegGs
      ) ;

      ResourceString Exception ( LibraryHandle, IDS_EXCEPTION ) ;
      Debug ( HWND_DESKTOP, PCHAR(Exception), _fullpath(PCHAR(NULL),LOGFILE,0) ) ;

      Base = (PLONG)pContextRecord->ctx_RegEbp ;
      while ( Base && ( *Base > (LONG)Base ) ) {
        Log ( "  Calling function was at %08lX.", *(Base+1) ) ;
        Base = (PLONG)*Base ;
      } /* endwhile */

      DosExit ( EXIT_PROCESS, 0 ) ;
   }

   if ( pExceptionReportRecord->ExceptionNum == XCPT_PRIVILEGED_INSTRUCTION ) {
      if ( Active ) {
         Log ( "ABORT: Recursion with exception handler." ) ;
         DosExit ( EXIT_PROCESS, 0 ) ;
      } /* endif */
      Active = TRUE ;
      Log
      (
        "ABORT: A privileged instruction was encountered.\n"
        "  Thread %i.\n"
        "  The registers were as follows:\n"
        "    AX:%08lX  BX:%08lX  CX:%08lX  DX:%08lX\n"
        "    FL:%08lX  DI:%08lX  SI:%08lX  BP:%08lX\n"
        "    CS:%08lX  IP:%08lX  SS:%08lX  SP:%08lX\n"
        "    DS:%08lX  ES:%08lX  FS:%08lX  GS:%08lX",
        ThreadID,
        pContextRecord->ctx_RegEax,
        pContextRecord->ctx_RegEbx,
        pContextRecord->ctx_RegEcx,
        pContextRecord->ctx_RegEdx,
        pContextRecord->ctx_EFlags,
        pContextRecord->ctx_RegEdi,
        pContextRecord->ctx_RegEsi,
        pContextRecord->ctx_RegEbp,
        pContextRecord->ctx_SegCs,
        pContextRecord->ctx_RegEip,
        pContextRecord->ctx_SegSs,
        pContextRecord->ctx_RegEsp,
        pContextRecord->ctx_SegDs,
        pContextRecord->ctx_SegEs,
        pContextRecord->ctx_SegFs,
        pContextRecord->ctx_SegGs
      ) ;

      ResourceString Exception ( LibraryHandle, IDS_EXCEPTION ) ;
      Debug ( HWND_DESKTOP, PCHAR(Exception), _fullpath(PCHAR(NULL),LOGFILE,0) ) ;

      Base = (PLONG)pContextRecord->ctx_RegEbp ;
      while ( Base && ( *Base > (LONG)Base ) ) {
        Log ( "  Calling function was at %08lX.", *(Base+1) ) ;
        Base = (PLONG)*Base ;
      } /* endwhile */

      DosExit ( EXIT_PROCESS, 0 ) ;
   }

   if ( pExceptionReportRecord->ExceptionNum == XCPT_ILLEGAL_INSTRUCTION ) {
      if ( Active ) {
         Log ( "ABORT: Recursion with exception handler." ) ;
         DosExit ( EXIT_PROCESS, 0 ) ;
      } /* endif */
      Active = TRUE ;
      Log
      (
        "ABORT: An illegal instruction was encountered.\n"
        "  Thread %i.\n"
        "  The registers were as follows:\n"
        "    AX:%08lX  BX:%08lX  CX:%08lX  DX:%08lX\n"
        "    FL:%08lX  DI:%08lX  SI:%08lX  BP:%08lX\n"
        "    CS:%08lX  IP:%08lX  SS:%08lX  SP:%08lX\n"
        "    DS:%08lX  ES:%08lX  FS:%08lX  GS:%08lX",
        ThreadID,
        pContextRecord->ctx_RegEax,
        pContextRecord->ctx_RegEbx,
        pContextRecord->ctx_RegEcx,
        pContextRecord->ctx_RegEdx,
        pContextRecord->ctx_EFlags,
        pContextRecord->ctx_RegEdi,
        pContextRecord->ctx_RegEsi,
        pContextRecord->ctx_RegEbp,
        pContextRecord->ctx_SegCs,
        pContextRecord->ctx_RegEip,
        pContextRecord->ctx_SegSs,
        pContextRecord->ctx_RegEsp,
        pContextRecord->ctx_SegDs,
        pContextRecord->ctx_SegEs,
        pContextRecord->ctx_SegFs,
        pContextRecord->ctx_SegGs
      ) ;

      ResourceString Exception ( LibraryHandle, IDS_EXCEPTION ) ;
      Debug ( HWND_DESKTOP, PCHAR(Exception), _fullpath(PCHAR(NULL),LOGFILE,0) ) ;

      Base = (PLONG)pContextRecord->ctx_RegEbp ;
      while ( Base && ( *Base > (LONG)Base ) ) {
        Log ( "  Calling function was at %08lX.", *(Base+1) ) ;
        Base = (PLONG)*Base ;
      } /* endwhile */

      DosExit ( EXIT_PROCESS, 0 ) ;
   }

   if ( pExceptionReportRecord->ExceptionNum == XCPT_INTEGER_DIVIDE_BY_ZERO ) {
      if ( Active ) {
         Log ( "ABORT: Recursion with exception handler." ) ;
         DosExit ( EXIT_PROCESS, 0 ) ;
      } /* endif */
      Active = TRUE ;
      Log
      (
        "ABORT: An integer divide-by-zero error has occurred.\n"
        "  Thread %i.\n"
        "  The registers were as follows:\n"
        "    AX:%08lX  BX:%08lX  CX:%08lX  DX:%08lX\n"
        "    FL:%08lX  DI:%08lX  SI:%08lX  BP:%08lX\n"
        "    CS:%08lX  IP:%08lX  SS:%08lX  SP:%08lX\n"
        "    DS:%08lX  ES:%08lX  FS:%08lX  GS:%08lX",
        ThreadID,
        pContextRecord->ctx_RegEax,
        pContextRecord->ctx_RegEbx,
        pContextRecord->ctx_RegEcx,
        pContextRecord->ctx_RegEdx,
        pContextRecord->ctx_EFlags,
        pContextRecord->ctx_RegEdi,
        pContextRecord->ctx_RegEsi,
        pContextRecord->ctx_RegEbp,
        pContextRecord->ctx_SegCs,
        pContextRecord->ctx_RegEip,
        pContextRecord->ctx_SegSs,
        pContextRecord->ctx_RegEsp,
        pContextRecord->ctx_SegDs,
        pContextRecord->ctx_SegEs,
        pContextRecord->ctx_SegFs,
        pContextRecord->ctx_SegGs
      ) ;

      ResourceString Exception ( LibraryHandle, IDS_EXCEPTION ) ;
      Debug ( HWND_DESKTOP, PCHAR(Exception), _fullpath(PCHAR(NULL),LOGFILE,0) ) ;

      Base = (PLONG)pContextRecord->ctx_RegEbp ;
      while ( Base && ( *Base > (LONG)Base ) ) {
        Log ( "  Calling function was at %08lX.", *(Base+1) ) ;
        Base = (PLONG)*Base ;
      } /* endwhile */

      DosExit ( EXIT_PROCESS, 0 ) ;
   }

   if ( pExceptionReportRecord->ExceptionNum == XCPT_INTEGER_OVERFLOW ) {
      if ( Active ) {
         Log ( "ABORT: Recursion with exception handler." ) ;
         DosExit ( EXIT_PROCESS, 0 ) ;
      } /* endif */
      Active = TRUE ;
      Log
      (
        "ABORT: An integer overflow has occurred.\n"
        "  Thread %i.\n"
        "  The registers were as follows:\n"
        "    AX:%08lX  BX:%08lX  CX:%08lX  DX:%08lX\n"
        "    FL:%08lX  DI:%08lX  SI:%08lX  BP:%08lX\n"
        "    CS:%08lX  IP:%08lX  SS:%08lX  SP:%08lX\n"
        "    DS:%08lX  ES:%08lX  FS:%08lX  GS:%08lX",
        ThreadID,
        pContextRecord->ctx_RegEax,
        pContextRecord->ctx_RegEbx,
        pContextRecord->ctx_RegEcx,
        pContextRecord->ctx_RegEdx,
        pContextRecord->ctx_EFlags,
        pContextRecord->ctx_RegEdi,
        pContextRecord->ctx_RegEsi,
        pContextRecord->ctx_RegEbp,
        pContextRecord->ctx_SegCs,
        pContextRecord->ctx_RegEip,
        pContextRecord->ctx_SegSs,
        pContextRecord->ctx_RegEsp,
        pContextRecord->ctx_SegDs,
        pContextRecord->ctx_SegEs,
        pContextRecord->ctx_SegFs,
        pContextRecord->ctx_SegGs
      ) ;

      ResourceString Exception ( LibraryHandle, IDS_EXCEPTION ) ;
      Debug ( HWND_DESKTOP, PCHAR(Exception), _fullpath(PCHAR(NULL),LOGFILE,0) ) ;

      Base = (PLONG)pContextRecord->ctx_RegEbp ;
      while ( Base && ( *Base > (LONG)Base ) ) {
        Log ( "  Calling function was at %08lX.", *(Base+1) ) ;
        Base = (PLONG)*Base ;
      } /* endwhile */

      DosExit ( EXIT_PROCESS, 0 ) ;
   }

   if ( pExceptionReportRecord->ExceptionNum == XCPT_FLOAT_DIVIDE_BY_ZERO ) {
      if ( Active ) {
         Log ( "ABORT: Recursion with exception handler." ) ;
         DosExit ( EXIT_PROCESS, 0 ) ;
      } /* endif */
      Active = TRUE ;
      Log
      (
        "ABORT: A floating point divide by zero error has occurred.\n"
        "  Thread %i.\n"
        "  The registers were as follows:\n"
        "    AX:%08lX  BX:%08lX  CX:%08lX  DX:%08lX\n"
        "    FL:%08lX  DI:%08lX  SI:%08lX  BP:%08lX\n"
        "    CS:%08lX  IP:%08lX  SS:%08lX  SP:%08lX\n"
        "    DS:%08lX  ES:%08lX  FS:%08lX  GS:%08lX",
        ThreadID,
        pContextRecord->ctx_RegEax,
        pContextRecord->ctx_RegEbx,
        pContextRecord->ctx_RegEcx,
        pContextRecord->ctx_RegEdx,
        pContextRecord->ctx_EFlags,
        pContextRecord->ctx_RegEdi,
        pContextRecord->ctx_RegEsi,
        pContextRecord->ctx_RegEbp,
        pContextRecord->ctx_SegCs,
        pContextRecord->ctx_RegEip,
        pContextRecord->ctx_SegSs,
        pContextRecord->ctx_RegEsp,
        pContextRecord->ctx_SegDs,
        pContextRecord->ctx_SegEs,
        pContextRecord->ctx_SegFs,
        pContextRecord->ctx_SegGs
      ) ;

      ResourceString Exception ( LibraryHandle, IDS_EXCEPTION ) ;
      Debug ( HWND_DESKTOP, PCHAR(Exception), _fullpath(PCHAR(NULL),LOGFILE,0) ) ;

      Base = (PLONG)pContextRecord->ctx_RegEbp ;
      while ( Base && ( *Base > (LONG)Base ) ) {
        Log ( "  Calling function was at %08lX.", *(Base+1) ) ;
        Base = (PLONG)*Base ;
      } /* endwhile */

      DosExit ( EXIT_PROCESS, 0 ) ;
   }

   if ( pExceptionReportRecord->ExceptionNum == XCPT_FLOAT_OVERFLOW ) {
      if ( Active ) {
         Log ( "ABORT: Recursion with exception handler." ) ;
         DosExit ( EXIT_PROCESS, 0 ) ;
      } /* endif */
      Active = TRUE ;
      Log
      (
        "ABORT: A floating point overflow error has occurred.\n"
        "  Thread %i.\n"
        "  The registers were as follows:\n"
        "    AX:%08lX  BX:%08lX  CX:%08lX  DX:%08lX\n"
        "    FL:%08lX  DI:%08lX  SI:%08lX  BP:%08lX\n"
        "    CS:%08lX  IP:%08lX  SS:%08lX  SP:%08lX\n"
        "    DS:%08lX  ES:%08lX  FS:%08lX  GS:%08lX",
        ThreadID,
        pContextRecord->ctx_RegEax,
        pContextRecord->ctx_RegEbx,
        pContextRecord->ctx_RegEcx,
        pContextRecord->ctx_RegEdx,
        pContextRecord->ctx_EFlags,
        pContextRecord->ctx_RegEdi,
        pContextRecord->ctx_RegEsi,
        pContextRecord->ctx_RegEbp,
        pContextRecord->ctx_SegCs,
        pContextRecord->ctx_RegEip,
        pContextRecord->ctx_SegSs,
        pContextRecord->ctx_RegEsp,
        pContextRecord->ctx_SegDs,
        pContextRecord->ctx_SegEs,
        pContextRecord->ctx_SegFs,
        pContextRecord->ctx_SegGs
      ) ;

      ResourceString Exception ( LibraryHandle, IDS_EXCEPTION ) ;
      Debug ( HWND_DESKTOP, PCHAR(Exception), _fullpath(PCHAR(NULL),LOGFILE,0) ) ;

      Base = (PLONG)pContextRecord->ctx_RegEbp ;
      while ( Base && ( *Base > (LONG)Base ) ) {
        Log ( "  Calling function was at %08lX.", *(Base+1) ) ;
        Base = (PLONG)*Base ;
      } /* endwhile */

      DosExit ( EXIT_PROCESS, 0 ) ;
   }

   if ( pExceptionReportRecord->ExceptionNum == XCPT_FLOAT_UNDERFLOW ) {
      if ( Active ) {
         Log ( "ABORT: Recursion with exception handler." ) ;
         DosExit ( EXIT_PROCESS, 0 ) ;
      } /* endif */
      Active = TRUE ;
      Log
      (
        "ABORT: A floating point underflow error has occurred.\n"
        "  Thread %i.\n"
        "  The registers were as follows:\n"
        "    AX:%08lX  BX:%08lX  CX:%08lX  DX:%08lX\n"
        "    FL:%08lX  DI:%08lX  SI:%08lX  BP:%08lX\n"
        "    CS:%08lX  IP:%08lX  SS:%08lX  SP:%08lX\n"
        "    DS:%08lX  ES:%08lX  FS:%08lX  GS:%08lX",
        ThreadID,
        pContextRecord->ctx_RegEax,
        pContextRecord->ctx_RegEbx,
        pContextRecord->ctx_RegEcx,
        pContextRecord->ctx_RegEdx,
        pContextRecord->ctx_EFlags,
        pContextRecord->ctx_RegEdi,
        pContextRecord->ctx_RegEsi,
        pContextRecord->ctx_RegEbp,
        pContextRecord->ctx_SegCs,
        pContextRecord->ctx_RegEip,
        pContextRecord->ctx_SegSs,
        pContextRecord->ctx_RegEsp,
        pContextRecord->ctx_SegDs,
        pContextRecord->ctx_SegEs,
        pContextRecord->ctx_SegFs,
        pContextRecord->ctx_SegGs
      ) ;

      ResourceString Exception ( LibraryHandle, IDS_EXCEPTION ) ;
      Debug ( HWND_DESKTOP, PCHAR(Exception), _fullpath(PCHAR(NULL),LOGFILE,0) ) ;

      Base = (PLONG)pContextRecord->ctx_RegEbp ;
      while ( Base && ( *Base > (LONG)Base ) ) {
        Log ( "  Calling function was at %08lX.", *(Base+1) ) ;
        Base = (PLONG)*Base ;
      } /* endwhile */

      DosExit ( EXIT_PROCESS, 0 ) ;
   }

   return ( XCPT_CONTINUE_SEARCH ) ;
}
