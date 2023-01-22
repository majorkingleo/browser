#ifndef OBJECT_IMAGE_H
#define OBJECT_IMAGE_H

#include "html_object.h"

class Image;

class ObjectImage : public HTMLObject
{
    FXString   file;
    Ref<Image> image;
    int        img_width;
    int        img_height;
    int        border;

    bool     failed_loading;

 public:
    static int TYPE_ID;

 public:
    ObjectImage( const Ref<HTMLObject> & parent, const Ref<Node> & node );

    void layout();
    void paint();

    void notify(); // image is now available
};

#endif
