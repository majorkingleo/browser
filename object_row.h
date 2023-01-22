#ifndef OBJECT_ROW_H
#define OBJECT_ROW_H

#include "html_object.h"

class ObjectRow : public HTMLObject
{
 protected:
    int border;
    int cellspacing;
    int cellpadding;

 public:
    static int TYPE_ID;

 public:
    ObjectRow( const Ref<HTMLObject> & parent, const Ref<Node> & node );

    void setWidth( int w );

    void layout();
    void paint();
};

#endif
