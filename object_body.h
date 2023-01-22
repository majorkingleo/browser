#ifndef OBJECT_BODY_H
#define OBJECT_BODY_H

#include "object_layout.h"

class ObjectBody : public ObjectLayout
{
    int bgcolor;

 public:
    static int TYPE_ID;

 public:
    ObjectBody( const  Ref<HTMLObject> & parent, const Ref<Node> & node );

    void layout();
    void paint();
};

#endif
