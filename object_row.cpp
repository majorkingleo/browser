#include "object_row.h"
#include "node.h"

int ObjectRow::TYPE_ID = HTMLObject::object_type_id_count++;

ObjectRow::ObjectRow( const Ref<HTMLObject> & parent, const Ref<Node> & node )
    : HTMLObject( parent, node )
{
    object_type_id = TYPE_ID;
    classname = "ObjectRow";
}

void ObjectRow::layout()
{
    bool ll = layouted;

    HTMLObject::layout();

    if( !ll )
    {
	getStyle( "table-border", "0" ).scan( "%d", &border );
	getStyle( "table-cellspacing", "10" ).scan( "%d", &cellspacing );
	getStyle( "table-cellpadding", "10" ).scan( "%d", &cellpadding );
    }

    height = 0;
    min_height = 0;

    for( unsigned i = 0; i < children.size(); ++i )
    {
	if( children[i]->getHeight() > height )
	    height = children[i]->getHeight();

	if( children[i]->getMinHeight() > min_height )
	    min_height = children[i]->getMinHeight();
    }
}

void ObjectRow::paint()
{
    if( markup )
    {
	dc.setColor( 0xff0000 );
	dc.fillRectangle( 0, 0, width, height );
    }

    int x = 0;

    for( unsigned i = 0; i < children.size(); ++i )
    {
/*
	Ref<HTMLDC> cdc = dc->getDC( children[i], x, 0, children[i]->getWidth(), height );
	children[i]->paint( cdc );
*/
	dc.addChild( children[i]->getDC() );
	children[i]->getDC()->setRect( &dc, x, 0, children[i]->getWidth(), height );
	children[i]->paint();

	x += children[i]->getWidth() + cellspacing;
    }   
}

void ObjectRow::setWidth( int w )
{
    width = w;
    height = 0;
    min_height = 0;

    for( unsigned i = 0; i < children.size(); ++i )    
    {
	if( children[i]->getHeight() > height )
	    height = children[i]->getHeight();

	if( children[i]->getMinHeight() > min_height )
	    min_height = children[i]->getMinHeight();
    }
    
/*
    if( min_height > height )
	printf( "ObjectRow::setWidth min_height > height => %d > %d\n", min_height, height );
*/
}
