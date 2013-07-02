// Class RESTRING: Encapsulates the load/discard
//   logic for a resource String Table entry.

#define INCL_BASE
#include <os2.h>

class ResourceString
{
  private:
    HMODULE SavedModule ;
    ULONG SavedId ;

    PVOID BlockPointer ;
    PSZ StringPointer ;

  public:
    // Constructor
    ResourceString ( HMODULE Module, ULONG Id ) ;

    // Copy Constructor
    ResourceString ( const ResourceString & Object )
    {
      SavedModule   = Object.SavedModule ;
      SavedId       = Object.SavedId ;
      BlockPointer  = Object.BlockPointer ;
      StringPointer = Object.StringPointer ;
    }

    // Typecast Operators
    operator unsigned char * ()
    {
      return ( (unsigned char *) StringPointer ) ;
    }

    operator char * ()
    {
      return ( (char*) StringPointer ) ;
    }

    // Assignment Operator
    ResourceString & operator= ( const ResourceString & Object )
    {
      SavedModule   = Object.SavedModule ;
      SavedId       = Object.SavedId ;
      BlockPointer  = Object.BlockPointer ;
      StringPointer = Object.StringPointer ;
      return ( *this ) ;
    }
} ;
