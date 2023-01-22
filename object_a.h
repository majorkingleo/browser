#ifndef OBJECT_A
#define OBJECT_A

#include "object_layout.h"

class ObjectA : public ObjectLayout
{
    FXString url;

 public:
    static int TYPE_ID;

 public:
    ObjectA( const Ref<HTMLObject> & parent, const Ref<Node> & node );

    void onMouseEnter( int x, int y );
    void onMouseLeave( int x, int y );
    void onMouseDown( int x, int y );
};

#endif
