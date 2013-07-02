// Class MODULE: Encapsulates the load/unload logic for a OS/2 resource module.

#define INCL_BASE
#include <os2.h>

class Module
{
  private:
    HMODULE Handle ;

  public:
    Module ( PSZ Name = PSZ("") ) ;
    ~Module ( ) ;
    HMODULE QueryHandle () { return ( Handle ) ; }
} ;
