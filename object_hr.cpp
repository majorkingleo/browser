#include "node.h"
#include "object_hr.h"

int ObjectHR::TYPE_ID = HTMLObject::object_type_id_count++;

ObjectHR::ObjectHR(  const Ref<HTMLObject> & parent, const Ref<Node> & node )
    : HTMLObject( parent, node )
{
    object_type_id = TYPE_ID;
    classname = "ObjectHR";
    
    height = 5;
    width = 10;
    min_height = 5;
    min_width = 10;

    setLayoutHint( LAYOUT_FORCE_LNBR_BEFORE );
    setLayoutHint( LAYOUT_FORCE_LNBR_AFTER );
    setLayoutHint( LAYOUT_MANAGER );
}

void ObjectHR::layout()
{
    // do nothing (no childs,.. etc)
}

void ObjectHR::paint()
{
    dc.setColor( 0xaaaaaa );
    dc.drawLine( 0, 1, width, 1 );
    dc.setColor( 0x888888 );
    dc.drawLine( 0, 2, width, 2 );
    dc.setColor( 0xffffff );    
    dc.drawLine( 0, 3, width, 3 );
}
