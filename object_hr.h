#ifndef OBJECT_HR_H
#define OBJECT_HR_H

#include "html_object.h"

class ObjectHR : public HTMLObject
{
 public:
    static int TYPE_ID;

 public:
    ObjectHR( const Ref<HTMLObject> & parent, const Ref<Node> & node );

    void layout();
    void paint();
};

#endif
