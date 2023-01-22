#include "object_root.h"

int ObjectRoot::TYPE_ID = HTMLObject::object_type_id_count++;

ObjectRoot::ObjectRoot( FXHTML *window )
    : HTMLObject( Ref<HTMLObject>() ),
      window( window )
{
    object_type_id  = TYPE_ID;
    classname = "ObjectRoot";

    FXFont *f = new FXFont( window->getApp(), "nimbus", 12 );

    f->create();

    Font ff;

    ff.name     = f->getName();
    ff.font     = f;
    ff.size     = f->getSize();
    ff.weight   = f->getWeight();
    ff.slant    = f->getSlant();
    ff.encoding = f->getEncoding();
    ff.hints    = f->getHints();
    ff.width    = f->getSetWidth();
	
    fonts.push_back( ff );

    link_cursor_count = 0;
}

ObjectRoot::~ObjectRoot()
{
    for( unsigned i = 0; i < fonts.size(); ++i )
	delete fonts[i].font;
}

void ObjectRoot::layout()
{
    width = window->getWidth();
    height = window->getHeight();
    
    min_width = window->getWidth();
    min_height = window->getHeight();

    HTMLObject::layout();

    if( width >= body->getMinWidth() )
	body->setWidth( width );
    else
	body->setWidth( body->getMinWidth() );
    
    width  = body->getWidth();
    height = body->getHeight();    
}

FXFont* ObjectRoot::getFont( int font )
{
    if( font < (int)fonts.size() )
	return fonts[font].font;

    return fonts[0].font;
}

FXFont* ObjectRoot::getFont( const FXString & fontname,  unsigned size, unsigned font_weight, unsigned font_slant,
			     unsigned font_encoding, unsigned font_width, unsigned hints, int & id )
{
    for( unsigned i = 0; i < fonts.size(); ++i )
    {
	if( fonts[i].name     == fontname && 
	    fonts[i].size     == size * 10  &&
	    fonts[i].weight   == font_weight &&
	    fonts[i].slant    == font_slant &&
	    fonts[i].encoding == font_encoding &&
	    fonts[i].width    == font_width &&
	    fonts[i].hints    == hints
	    )
	{
	    id = i;
	    return fonts[i].font;
	}
    }

    FXFont *f = new FXFont( window->getApp(), fontname, size, font_weight, font_slant, font_encoding, font_width, hints );

    f->create();

    Font ff;

    ff.name     = fontname;
    ff.size     = size * 10;
    ff.weight   = font_weight;
    ff.slant    = font_slant;
    ff.encoding = font_encoding;
    ff.width    = font_width;
    ff.hints    = hints;
    ff.font     = f;

    fonts.push_back( ff );

    printf( "%s size: %d\n", fontname.text(), ff.size );
    
    return f;
}

void ObjectRoot::loadImage( const FXString &file )
{
    // tell url loader to load it
    window->loadImage( file );
}

void ObjectRoot::getImage( const FXString &file, Ref<Image> &image )
{
    window->getImage( file, image );
}

void ObjectRoot::beginLinkCursor( const FXString &url )
{
    link_cursor_count++;

    if( link_cursor_count == 1 )
    {
	FXCursor *def = window->getApp()->getDefaultCursor( DEF_RARROW_CURSOR );       
	window->setDefaultCursor( def );
    }

    window->setLinkInfo( url );
}

void ObjectRoot::endLinkCursor()
{
    link_cursor_count--;

    if( link_cursor_count <= 0 )
    {
	link_cursor_count = 0;

	FXCursor *def = window->getApp()->getDefaultCursor( DEF_ARROW_CURSOR );
	window->setDefaultCursor( def );
    }
}

void ObjectRoot::loadURL( const FXString &url )
{
    window->requestLoadingUrl( url );
}
