#include "object_ul.h"
#include "node.h"
#include "string_utils.h"

int ObjectUL::TYPE_ID = HTMLObject::object_type_id_count++;

ObjectUL::ObjectUL( const Ref<HTMLObject> & parent, const Ref<Node> & node )
    : HTMLObject( parent, node )
{
    object_type_id = TYPE_ID;
    classname = "ObjectUL";

    setLayoutHint( LAYOUT_FORCE_LNBR_BEFORE );
    setLayoutHint( LAYOUT_FORCE_LNBR_AFTER );

    font = 0;    
    child_indent = 10;
}

void ObjectUL::layout()
{
    if( !layouted )    
    {
	font = getFont( getStyle( "font-face",           getFont( 0 )->getName() ), 
			string2uint( getStyle( "font-size",     getFont( 0 )->getSize() / 10 ) ) ,
			string2uint( getStyle( "font-weight",   getFont( 0 )->getWeight() ) ),
			string2uint( getStyle( "font-slant",    getFont( 0 )->getSlant() ) ),
			string2uint( getStyle( "font-encoding", getFont( 0 )->getEncoding() ) ),
			string2uint( getStyle( "font-width",    getFont( 0 )->getSetWidth() ) ),
			string2uint( getStyle( "font-hints",    getFont( 0 )->getHints() ) ) ,
			current_font );

	getStyle( "ul-enum-type", "0" ).scan( "%d", &enum_type );
	enum_type++;

	if( enum_type > 2 )
	    enum_type = 0;

	setStyle( "ul-enum-type", enum_type );

	child_indent = font->getFontHeight();

	color = string2color( getStyle( "color", "#000000" ), 0 );
    }

    HTMLObject::layout();

    width = 0;
    min_width = 0;
    height = 0;
    min_height = 0;

    for( unsigned i = 0; i < children.size(); ++i )
    {
	if( width < children[i]->getWidth() )
	    width = children[i]->getWidth();

	if( min_width < children[i]->getMinWidth() )
	    min_width = children[i]->getMinWidth();

	min_height += children[i]->getMinHeight();
	
	height += children[i]->getHeight();
    }

    width += child_indent * 2;
    max_width = width;
    min_width += child_indent * 2;
}

void ObjectUL::paint()
{
    if( markup )
    {
	dc.setColor( 0 );
	dc.drawRectangle( 0, 0, width, height );
    }

    int y = 0;
    int x = child_indent;
    int w = font->getFontHeight() / 3;

    for( unsigned i = 0; i < children.size(); ++i )
    {
	dc.setColor( color );

	if( enum_type == 1 )
	    dc.fillArc( x,  font->getFontDescent() + y + w / 2, w, w, 0, 360 * 64 );
	else if( enum_type == 2 )
	    dc.drawArc( x,  font->getFontDescent() + y + w / 2, w, w, 0, 360 * 64 );
	else
	    dc.fillRectangle( x, font->getFontDescent() + y + w / 2, w, w );

//	Ref<HTMLDC> cdc = dc->getDC( children[i], x * 2, y, width - x * 2, children[i]->getHeight() );
	dc.addChild( children[i]->getDC() );
	children[i]->getDC()->setRect( &dc, x * 2, y, width - x * 2, children[i]->getHeight() );
	children[i]->paint();

	y += children[i]->getHeight();
    }
}

void ObjectUL::setWidth( int w )
{
    width = w;

    w -= child_indent * 2;

    height = 0;

    for( unsigned i = 0; i < children.size(); ++i )
    {
	if( children[i]->getWidth() > w )
	    children[i]->setWidth( w );
	else if( children[i]->getMaxWidth() > children[i]->getWidth() )
	    children[i]->setWidth( w );

	height += children[i]->getHeight();
    }
}
