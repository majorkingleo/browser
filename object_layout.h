#ifndef OBJECT_LAYOUT_H
#define OBJECT_LAYOUT_H

#include "html_object.h"

class ObjectLayout : public HTMLObject
{
 private:

    enum Align {
	ALIGN_LEFT,
	ALIGN_CENTER,
	ALIGN_RIGHT
	};

    Align align;

    int spacer;

    std::vector< std::vector<int> > lines;

    std::vector< Ref<HTMLObject> > deamons;

 public:
    static int TYPE_ID;

 public:
    ObjectLayout( const  Ref<HTMLObject> & parent, const Ref<Node> & node );

    void layout();
    void paint();

    void setWidth( int w );
    void setHeight( int h );

 protected:
    void calcLines( std::vector< std::vector<int> > & lines, int & width, int & height );
};

#endif
