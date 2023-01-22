#include "object_body.h"
#include "node.h"
#include "string_utils.h"

int ObjectBody::TYPE_ID = HTMLObject::object_type_id_count++;

ObjectBody::ObjectBody( const  Ref<HTMLObject> & parent, const Ref<Node> & node )
    : ObjectLayout( parent, node )
{
    object_type_id = TYPE_ID;
    classname = "ObjectBody";

    for( int i = 0; i < node->getAttributeNum(); ++i )
    {
	Node::Attribute attr = node->getAttribute( i );
	
	attr.name = attr.name.lower();

	if( attr.name == "bgcolor" )
	    setStyle( "background-color", attr.value.lower() );

	if( attr.name == "text" )
	    setStyle( "color", attr.value.lower() );	
    }

    min_width = 0;
    min_height = 0;

    bgcolor = 0xffffff;
}

void ObjectBody::layout()
{
    width = parent->getWidth();
    height = parent->getHeight();

    if( !layouted )
	bgcolor = string2color( getStyle( "background-color", "#ffffff" ), 0xffffff );

//    printf( "width1: %d height1: %d min_width: %d\n", width, height, min_width );

    ObjectLayout::layout();

    if( min_width > parent->getWidth() )
	setWidth( min_width );
    
//    printf( "width: %d height: %d min_width: %d\n", width, height, min_width );
}

void ObjectBody::paint()
{
    dc.setColor( bgcolor );
    dc.fillRectangle( 0, 0, dc.getWidth(), dc.getHeight() );

    ObjectLayout::paint();
}
