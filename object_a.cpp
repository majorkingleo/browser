#include "object_a.h"
#include "node.h"

int ObjectA::TYPE_ID = HTMLObject::object_type_id_count++;

ObjectA::ObjectA( const Ref<HTMLObject> & parent, const Ref<Node> & node )
    : ObjectLayout( parent, node )
{
    object_type_id = TYPE_ID;
    classname = "ObjectA";

    FXString value;

    if( node->getAttribute( "href", value ) && value.length() )
    {
	setStyle( "color", "#0000ff" );
	setStyle( "font-decore", FONT_DECORE_UNDERLINE );

	url = value;
    }
    
    setLayoutHint( LAYOUT_ADOPT_MY_CHILDS );
}

void ObjectA::onMouseEnter( int x, int y )
{
    if( url.length() )
	beginLinkCursor( url );
}

void ObjectA::onMouseLeave( int x, int y )
{
    if( url.length() )
	endLinkCursor();
}

void ObjectA::onMouseDown( int x, int y )
{
    if( url.length() )
	loadURL( url );
}
