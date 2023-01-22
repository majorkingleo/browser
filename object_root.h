#ifndef OBJECT_ROOT
#define OBJECT_ROOT

#include "fxhtml.h"
#include "html_object.h"

class ObjectRoot : public HTMLObject
{
    FXHTML *window;

    struct Font
    {
	FXString name;
	FXFont   *font;

	unsigned size;
	unsigned weight;
	unsigned slant;
	unsigned encoding;
	unsigned width;
	unsigned hints;
    };

    std::vector<Font> fonts;    

    int link_cursor_count;

    Ref<HTMLObject> body;

 public:
    static int TYPE_ID;

 public:
    ObjectRoot( FXHTML *window );
    ~ObjectRoot();

    void layout();

    FXFont* getFont( int font );
    FXFont* getFont( const FXString & fontname,  unsigned size, unsigned font_weight, unsigned font_slant,
			     unsigned font_encoding, unsigned font_width, unsigned hints, int & id ); 

    void loadURL( const FXString &url );
    void loadImage( const FXString &file );
    void getImage( const FXString &file, Ref<Image> &image );

    void beginLinkCursor( const FXString &url );
    void endLinkCursor();

    void setBody( Ref<HTMLObject> body_ ) { body = body_; }
};

#endif
