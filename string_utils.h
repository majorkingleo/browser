#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <fox/fx.h>
#include "ref.h"

class HTMLObject;

int string2color( FXString color, int def );
int string2uint( const FXString & num , int def = 0);
FXString layouthint2string( int layouthint );
FXString layouthint2string( Ref<HTMLObject> obj );

FXString decodeEntities( const FXString & text );

#endif
