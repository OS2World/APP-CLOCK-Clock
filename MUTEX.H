/******************************************************************** MUTEX.H
 *                                                                          *
 *  MUTEX Semaphore Class                                                   *
 *                                                                          *
 ****************************************************************************/

#ifndef MUTEX_H
#define MUTEX_H

class Mutex
{
  private:
    HMTX Handle ;

  public:
    Mutex ( PSZ Name )
    {
      APIRET Status = DosCreateMutexSem ( Name, &Handle, 0, FALSE ) ;
      if ( Status )
      {
         extern VOID Log ( char *Message, ... ) ;
         Log ( "MUTEX: Unable to create semaphore '%s'.\r\n", Name ) ;
      }
    }

    ~Mutex ( )
    {
      DosCloseMutexSem ( Handle ) ;
    }

    APIRET Request ( int Timeout )
    {
      return ( DosRequestMutexSem ( Handle, Timeout ) ) ;
    }

    APIRET Release ( )
    {
      return ( DosReleaseMutexSem ( Handle ) ) ;
    }
} ;

#endif
