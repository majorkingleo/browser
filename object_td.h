#ifndef OBJECT_TD_H
#define OBJECT_TD_H

#include "html_object.h"

class ObjectTD : public HTMLObject
{
 protected:
    int border;
    int cellspacing;
    int cellpadding;

    int available_height;
    
    enum {
	VALIGN_TOP,
	VALIGN_MIDDLE,
	VALIGN_BOTTOM
    };

    int valign;

    bool nowrap;

    int bgcolor;

 public:
    static int TYPE_ID;

 public:
    ObjectTD( const  Ref<HTMLObject> & parent , const Ref<Node> & node );

    void setWidth( int w );
    void setHeight( int h );

    void layout();
    void paint();
};

#endif
