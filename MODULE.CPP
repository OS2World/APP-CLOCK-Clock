// Class MODULE: Encapsulates the load/unload logic for a OS/2 resource module.

#define INCL_BASE
#define INCL_PM
#include <os2.h>

#include <stdlib.h>

#include "debug.h"

#include "module.h"

  // Constructor

Module::Module ( PSZ Name ) {
  Handle = 0 ;

  if ( *Name ) {
    if ( DosLoadModule ( PSZ(NULL), 0, Name, &Handle ) ) {
      Log ( "ERROR: Unable  to load module '%s'.\r\n", Name ) ;
      Debug ( HWND_DESKTOP, "ERROR: Unable to load module '%s'.", Name ) ;
      abort ( ) ;
    }
  }
}

  // Destructor

Module::~Module ( ) {
  if ( Handle ) {
    DosFreeModule ( Handle ) ;
  }
}
