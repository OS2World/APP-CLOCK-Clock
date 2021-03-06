// Class PROCESS: Encapsulates the startup/shutdown logic for a OS/2-PM process.

#define INCL_BASE
#define INCL_WIN
#include <os2.h>

#include "debug.h"
#include "except.h"

#include "process.h"


// Constructor

Process::Process ( LONG QueueSize )
{
  DosError ( FERR_DISABLEEXCEPTION ) ;

  ExceptionRecord.prev_structure = 0 ;
  ExceptionRecord.ExceptionHandler = ExceptionHandler ;
  DosSetExceptionHandler ( &ExceptionRecord ) ;

  Anchor = WinInitialize ( 0 ) ;
  if ( Anchor == 0 )
  {
//  Log ( "ERROR: Unable to initialize for windowing.\r\n" ) ;
    DosExit ( EXIT_PROCESS, 1 ) ;
  }

  Queue = WinCreateMsgQueue ( Anchor, QueueSize ) ;
  if ( Queue == 0 )
  {
//  Log ( "ERROR: Unable to create process message queue.\r\n" ) ;
    DosExit ( EXIT_PROCESS, 1 ) ;
  }
}


// Destructor

Process::~Process ( )
{
  WinDestroyMsgQueue ( Queue ) ;
  WinTerminate ( Anchor ) ;
  DosUnsetExceptionHandler ( &ExceptionRecord ) ;
}
