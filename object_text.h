#ifndef OBJECT_TEXT_H
#define OBJECT_TEXT_H

#include "html_object.h"

class ObjectText : public HTMLObject
{
    FXString text;    
    FXFont   *font;

    int color;   
    int decore;

 public:
    static int TYPE_ID;

 public:
    ObjectText( const Ref<HTMLObject> & parent, const FXString & text );

    void setWidth( int w );
    void setHeight( int h );
	

    void layout();
    void paint();

    FXString getText() { return text; }
};

#endif
