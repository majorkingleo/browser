#ifndef IMAGE_H
#define IMAGE_H

#include <fox/fx.h>

class ObjectImage;

class Image
{
    FXImage *image;
    FXString src;
    ObjectImage *parent;

 public:
    Image( const FXString &src, ObjectImage *parent ) : image(0), src( src ) {}
    ~Image() { delete image; }

    FXImage* getImage() { return image; }
    void setImage( FXImage *image_ ) { image = image_; }
    void destroy() { delete image; }
    bool valid() { return image; }
    int getWidth() { return image->getWidth(); }
    int getHeight() { return image->getHeight(); }
    void scale( int w, int h ) { image->scale( w, h ); }

    const FXString& getSrc() { return src; }

    void notify();
};

#endif
