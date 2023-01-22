#ifndef OBJECT_TABLE_H
#define OBJECT_TABLE_H

#include "html_object.h"

class ObjectTable : public HTMLObject
{
 protected:
    int cellspacing;
    int cellpadding;
    int border;

    bool rowspanned;
    int  bgcolor;

 public:
    static int TYPE_ID;

 public:
    ObjectTable( const Ref<HTMLObject> & parent, const Ref<Node> & node );

    void layout();
    void paint();

    void setWidth( int w );

 protected:
    void calcRows( std::vector<int> & cols_w, std::vector<int> & cols_mw );
};

#endif
