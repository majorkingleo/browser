#include "object_td.h"
#include "node.h"
#include "string_utils.h"

int ObjectTD::TYPE_ID = HTMLObject::object_type_id_count++;

ObjectTD::ObjectTD( const Ref<HTMLObject> & parent , const Ref<Node> & node )
    : HTMLObject( parent, node )
{
    object_type_id = TYPE_ID;
    classname = "ObjectTD";

    FXString value;

    if( node->getAttribute( "colspan", value ) )
    {
	int d = 0;
	value.scan( "%d", &d );

	if( d )
	    d--;

	setStyle( "cell-colspan", d );
    }
    else
	setStyle( "cell-colspan", "0" );

    value.clear();

    if( node->getAttribute( "rowspan", value ) )
    {
	int d = 0;
	value.scan( "%d", &d );

	if( d )
	    d--;

	setStyle( "cell-rowspan", d );
    }
    else
	setStyle( "cell-rowspan", "0" );

    if( node->getAttribute( "valign", value ) )
	setStyle( "table-valign", value );

    value.clear();
    nowrap = node->getAttribute( "nowrap", value );	

    value.clear();
    bgcolor = -1;
    if( node->getAttribute( "bgcolor", value ) )
	bgcolor = string2color( value, -1 );

    value.clear();
    if( node->getAttribute( "width", value ) )
    {
	if( value.length() )
	{
	    if( value[value.length()-1] == '%' )
	    {
		pwidth = 0;
		setLayoutHint( LAYOUT_WIDTH_PERCENTAGE );
		value.scan( "%d%%", &pwidth );
	    } else {
		width = 0;
		setLayoutHint( LAYOUT_FIX_SIZE );
		value.scan( "%d", &width );
		min_width = width;
	    }
	}
    }

}

void ObjectTD::layout()
{
    bool ll = layouted;

    HTMLObject::layout();

    if( !ll )
    {
	getStyle( "table-border", "0" ).scan( "%d", &border );
	getStyle( "table-cellspacing", "10" ).scan( "%d", &cellspacing );
	getStyle( "table-cellpadding", "10" ).scan( "%d", &cellpadding );

	valign = VALIGN_MIDDLE;

	FXString va = getStyle( "table-valign", "middle" );

	if( va == "middle" )
	    valign = VALIGN_MIDDLE;
	
	if( va == "top" )
	    valign = VALIGN_TOP;
	
	
	if( va == "bottom" )
	    valign = VALIGN_BOTTOM;
    }

    int mwidth = 0;

    if( getLayoutHint( LAYOUT_FIX_SIZE ) )
	mwidth = width;

    width = children[0]->getWidth() + cellpadding *  2;
    height = children[0]->getHeight() + cellpadding * 2;
    max_width = width;

    min_width = children[0]->getMinWidth() + cellpadding * 2;
    min_height = children[0]->getMinHeight() + cellpadding * 2;

    available_height = height;

    if( nowrap )
	min_width = width;

    if( mwidth )
    {
	width = mwidth;
	min_width = mwidth; 
    }
}

void ObjectTD::setWidth( int w )
{
//    printf( "ObjectTD::setWidth: %d\n", w );

    if( w < min_width )
	printf( "ObjectTD::setWidth %d %d\n", min_width, w );
 
/*   
    if( w == min_width )
    {
	printf( "ObjectTD::setWidth %d == %d\n", min_width, w );
	printf( "Childnum %d\n", children.size() );
    }
*/
    children[0]->setWidth( w - cellpadding * 2 );

//    printf( "min_width: %d\n", min_width );

    width = w;
    height = children[0]->getHeight() + cellpadding * 2;
    min_height = children[0]->getMinHeight() + cellpadding * 2;    
}

void ObjectTD::setHeight( int h )
{
    available_height = h;
}

void ObjectTD::paint()
{
    if( border )
    {
	dc.setColor( 0x888888 );
	dc.drawLine( 0, 0, width - 1, 0 );
	dc.drawLine( 0, 0, 0, available_height - 1);

	dc.setColor( 0xeeeeee );
	dc.drawLine( 0, available_height - 1, width - 1, available_height -1 );
	dc.drawLine( width - 1, 0, width - 1, available_height - 1 );
    }

    int c_bgcolor = bgcolor;

    if( markup )
    {
	if( c_bgcolor == 0xff0000 )
	    c_bgcolor = 0x00ff00;
	else
	    c_bgcolor = 0xff0000;
	    
    }

    if( c_bgcolor != -1 )
    {
	dc.setColor( c_bgcolor );
	dc.fillRectangle( border, border, width - border, available_height - border );
    }

    int space = 0;

    switch( valign )
    {
	case VALIGN_TOP:    space = 0; break;
	case VALIGN_MIDDLE: space = (available_height - height) / 2; break;
	case VALIGN_BOTTOM: space = available_height - height; break;
    }

//    Ref<HTMLDC> cdc = dc->getDC( children[0], cellpadding, cellpadding + space, width - cellpadding *2 , height - cellpadding *2 );

    dc.addChild( children[0]->getDC() );
    children[0]->getDC()->setRect( &dc, cellpadding, cellpadding + space, width - cellpadding *2 , height - cellpadding *2 );

    return children[0]->paint();
}
