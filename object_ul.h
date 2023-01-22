#ifndef OBJECT_UL_H
#define OBJECT_UL_H

#include "html_object.h"

class ObjectUL : public HTMLObject
{
    FXFont *font;
    
    int    enum_type;
    int    child_indent;
    int    color;

 public:
    static int TYPE_ID;

 public:
    ObjectUL( const Ref<HTMLObject> & parent, const Ref<Node> & node );

    void setWidth( int w );    

    void layout();
    void paint();
};

#endif
