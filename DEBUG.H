/******************************************************************** DEBUG.H
 *                                                                          *
 *  Debugging Aids                                                          *
 *                                                                          *
 ****************************************************************************/

#ifndef DEBUG_H
#define DEBUG_H

#include <stdlib.h>

#include "hrtimer.h"

extern HFILE Timer ;
extern BOOL Trace ;

extern VOID Debug ( HWND hwnd, char *Message, ... ) ;
extern VOID Log ( char *Message, ... ) ;

extern BOOL OpenTimer ( VOID ) ;
extern VOID CloseTimer ( VOID ) ;
extern BOOL GetTime ( PTIMESTAMP pts ) ;
extern ULONG ComputeElapsedTime ( PTIMESTAMP ptsStart, PTIMESTAMP ptsStop, PULONG pulNs ) ;

extern PVOID AllocateMemory ( size_t ByteCount ) ;
extern VOID FreeMemory ( PVOID Memory ) ;

extern ULONG APIENTRY ExceptionHandler
(
  PEXCEPTIONREPORTRECORD pExceptionReportRecord,
  PEXCEPTIONREGISTRATIONRECORD pExceptionRegistrationRecord,
  PCONTEXTRECORD pContextRecord,
  PVOID pDispatcherContext
) ;

#endif
