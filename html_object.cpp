#include "html_object.h"
#include "node.h"

int HTMLObject::object_type_id_count = 0;

HTMLObject::HTMLObject( const Ref<HTMLObject> & parent )
    : parent( parent ),
      current_font( 0 ),
      dc( this )
{
    for( int i = 0; i < LAYOUT_LAST; ++i )
	layout_hints[i] = LAYOUT_NO_HINT;

    width = 0;
    height = 0;
    min_width = 0;
    min_height = 0;
    max_width = 0;
    layouted = false;
    markup = false;
    nolayout = false;
}

HTMLObject::HTMLObject( const Ref<HTMLObject> & parent, const Ref<Node> & node )
    : parent( parent ),
      current_font( 0 ),
      node( node ),
      dc ( this )
{
    for( int i = 0; i < LAYOUT_LAST; ++i )
	layout_hints[i] = LAYOUT_NO_HINT;

    width = 0;
    height = 0;
    min_width = 0;
    min_height = 0;
    max_width = 0;
    layouted = false;
    markup = false;
    nolayout = false;
}


HTMLObject::~HTMLObject()
{

}

Ref<Node> HTMLObject::getNode()
{
    return node;
}

void HTMLObject::layout()
{
    layouted = true;

    for( unsigned i = 0; i < children.size(); ++i )
	if( children[i]->relayout() )
	    children[i]->layout();
}

void HTMLObject::paint()
{
    for( unsigned i = 0; i < children.size(); ++i )
    {
	dc.addChild( children[i]->getDC() );
	children[i]->getDC()->setRect( &dc, 0, 0, dc.getWidth(), dc.getHeight() );
	children[i]->paint();
    }
}


FXString HTMLObject::getStyle( const FXString & name, const FXString & default_style )
{
    for( unsigned i = 0; i < styles.size(); ++i )
	if( name == styles[i].name )
	    return styles[i].value;

    if( parent.valid() )
	return parent->getStyle( name, default_style );

    return default_style;
}

void HTMLObject::setStyle( const FXString & name, const FXString & value )
{
    for( unsigned i = 0; i < styles.size(); ++i )
	if( name == styles[i].name )
	{
	    styles[i].value = value;
	    return;
	}

    styles.push_back( Style( name, value ) );
}

void HTMLObject::setLayoutHint( int hint )
{
    if( hint >= LAYOUT_LAST )
    {
	printf( "hint > LAYOUT_LAST\n" );
	return;
    }

    layout_hints[hint] = hint;
}


void HTMLObject::delLayoutHint( int hint )
{
    if( hint >= LAYOUT_LAST )
    {
	printf( "hint > LAYOUT_LAST\n" );
	return;
    }

    layout_hints[hint] = LAYOUT_NO_HINT;
}

void HTMLObject::setWidth( int w )
{
    width = w;
}

void HTMLObject::setHeight( int h )
{
    height = h;
}

FXFont* HTMLObject::getFont( int font )
{
    return parent->getFont( font );
}

FXFont* HTMLObject::getFont( const FXString & fontname,  unsigned size, unsigned font_weight, unsigned font_slant,
			     unsigned font_encoding, unsigned font_width, unsigned hints, int & id )
{
    return parent->getFont( fontname, size, font_weight, font_slant, font_encoding, font_width, hints, id );
}

void HTMLObject::insertChild( Ref<HTMLObject> c, int where )
{
    std::vector< Ref<HTMLObject> >::iterator it;
    unsigned i;

    for( i = 0, it = children.begin(); i < children.size(); ++i, ++it )
	if( i == (unsigned)where )
	{
	    children.insert( it, c );
	    return;
	}

    children.push_back( c );
}

void HTMLObject::loadURL( const FXString &url )
{
    parent->loadURL( url );
}

void HTMLObject::loadImage( const FXString &file )
{
    parent->loadImage( file );
}

void HTMLObject::getImage( const FXString &file, Ref<Image> &image )
{
    parent->getImage( file, image );
}

void HTMLObject::onMouseEnter( int x, int y ) {}
void HTMLObject::onMouseLeave( int x, int y ) {}
void HTMLObject::onMouseClick( int x, int y ) {}
void HTMLObject::onMouseDoubleClick( int x, int y ) {}
void HTMLObject::onMouseDown( int x, int y ) {}
void HTMLObject::onMouseUp( int x, int y ) {}
void HTMLObject::onMouseMove( int x, int y ) {}

void HTMLObject::beginLinkCursor( const FXString &url )
{
    parent->beginLinkCursor( url );
}

void HTMLObject::endLinkCursor()
{
    parent->endLinkCursor();
}

void HTMLObject::recache()
{
    layouted = false;
    nolayout = false;
    
    for( unsigned i = 0; i < children.size(); ++i )
	children[i]->recache();
}

void HTMLObject::doMarkup()
{
    markup = true;
}

void HTMLObject::clearMarkup()
{
    markup = false;
}
