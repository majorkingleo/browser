#ifndef FXHTML_H
#define FXHTML_H

#include <fox/fx.h>
#include "ref.h"
#include <vector>
#include "url_loader.h"
#include "fxurl2.h"


class FXHTML;
class Node;
class HTMLObject;
class HTMLDC;
class Image;

class FXHTMLWindow : public FXScrollWindow
{
    FXDECLARE( FXHTMLWindow );
 private:

    FXHTML *html;

    FXObject   *target;
    FXSelector selector;

    FXHTMLWindow() {}
 public:

    enum {
	ID_HTML = FXScrollWindow::ID_LAST,
	ID_LAST
    };

    // messages
    enum {
	HTML_LOAD_URL,   // data will be a FXString and has to be deleted by the receiver 
	HTML_LINK_INFO
    };

    struct Message
    {
	int message;
	void *data;

	Message( int m, void *d ) : message( m ), data( d ) {}
    };

 public:
    FXHTMLWindow( FXComposite *p, FXObject *tgt, FXSelector sel, FXuint opts, FXint x = 0, FXint y = 0, FXint w = 0, FXint h = 0 );

    void layout();

    void loadFile( const FXString &file );

    Ref<HTMLObject> getDOM();

    void doUpdate();

    int getInnerWidth();
    int getInnerHeight();

    void inform( int message, void *data );
};

class FXHTML : public FXCanvas
{
    FXDECLARE( FXHTML );
 public:
    enum {
	ID_HORIZONTAL = FXCanvas::ID_LAST,
	ID_VERTICAL,
	ID_IMAGE_TIMER,
	ID_LAST
    };

 private:
    FXHTML();

    Ref<Node>       html_root;
    Ref<HTMLObject> root;
    std::vector< HTMLObject* > last_objects;
    std::vector< Ref<Image> >  images;

    FXString        base_dir;
    FXURL2          base_url;

    URLLoader loader;

    FXTimer *image_timer;

 public:
    FXHTML( FXHTMLWindow *p );
    ~FXHTML();

    void layout();
    long onPaint( FXObject *obj, FXSelector sel, void *p );

    long onMouseDown( FXObject *obj, FXSelector sel, void *p );
    long onMouseUp( FXObject *obj, FXSelector sel, void *p );
    long onMouseClick( FXObject *obj, FXSelector sel, void *p );
    long onMouseDoubleClick( FXObject *obj, FXSelector sel, void *p );
    long onMouseMove( FXObject *obj, FXSelector sel, void *p );
    long onCheckImages( FXObject *obj, FXSelector sel, void *p );

    void requestLoadingUrl( const FXString &url );
    void loadFile( const FXString &file );

    void loadImage( const FXString &file );
    void getImage( const FXString &file, Ref<Image> &image );


    FXint getDefaultWidth();
    FXint getDefaultHeight();

    int getInnerWidth() { return ((FXHTMLWindow*)getParent())->getInnerWidth(); }
    int getInnerHeight() { return ((FXHTMLWindow*)getParent())->getInnerHeight(); }

    Ref<HTMLObject> getDOM();

    void doUpdate();

    void setLinkInfo( const FXString &url );

 private:
    void deleteImages();
};

#endif
