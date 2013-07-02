// Class PROCESS: Encapsulates the startup/shutdown logic for a OS/2-PM process.

#define INCL_WIN
#include <os2.h>

class Process
{
  private:
    EXCEPTIONREGISTRATIONRECORD ExceptionRecord ;
    HAB Anchor ;
    HMQ Queue ;

  public:
    Process ( LONG QueueSize = 0 ) ;
    ~Process ( ) ;
    inline HAB QueryAnchor () { return ( Anchor ) ; }
    inline HMQ QueryQueue () { return ( Queue ) ; }
} ;
