/****************************************************************** PROFILE.H
 *                                                                          *
 *                      Profile Object definitions                          *
 *                                                                          *
 ****************************************************************************/

#ifndef PROFILE_H
#define PROFILE_H

class Profile
{
  private:
    HINI   Handle ;               // INI file handle.
    PSZ    Name ;                 // Application Name.

  public:
    Profile
    (
      PSZ     Name,
      HAB     Anchor,
      HMODULE Library,
      int     DialogID,
      HWND    HelpWindow,
      BOOL    ResetFlag
    ) ;

    ~Profile ( ) ;

    HINI QueryHandle ( )
    {
      return ( Handle ) ;
    }

    PSZ QueryName ( )
    {
      return ( Name ) ;
    }

    VOID Reset ( )
    {
      PrfWriteProfileData ( Handle, Name, PSZ(NULL), PSZ(NULL), 0 ) ;
    }
} ;

#endif
