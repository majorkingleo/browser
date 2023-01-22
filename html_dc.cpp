#include "html_dc.h"
#include "html_object.h"

void HTMLDC::fillRectangle( int w_x, int w_y, int w_width, int w_height )
{
    dc->fillRectangle( x + w_x, y + w_y, w_width, w_height );
}

void HTMLDC::drawText( int w_x, int w_y, const FXString & text, unsigned length )
{
    dc->drawText( x + w_x, y + w_y, text.text(), length ? length : text.length() );
}

void HTMLDC::drawLine( int x1, int y1, int x2, int y2 )
{
    dc->drawLine( x + x1, y + y1, x + x2, y + y2 );
}

void HTMLDC::drawRectangle( int w_x, int w_y, int w_width, int w_height )
{
    dc->drawRectangle( x + w_x, y + w_y, w_width, w_height );
}

std::vector< HTMLObject* > HTMLDC::getObject( int w_x, int w_y )
{
    std::vector< HTMLObject* > objects;

//    printf( "x: %d y: %d width: %d height: %d w_x: %d w_y: %d\n", x, y, width, height, w_x, w_y );

//    if( w_x > width || w_y > height || w_x < 0 || w_y < 0 )
//	return obj;

    if( x < w_x &&
	y < w_y &&
	x + width > w_x &&
	y + height > w_y )
    {
	for( unsigned i = 0; i < children.size(); ++i )
	{
	    if( children[i]->x <= w_x && 
		children[i]->y <= w_y && 
		children[i]->x + children[i]->width > w_x &&
		children[i]->y + children[i]->height > w_y && 
		visible()
		)
	    {
		if( !children[i]->children.size() )
		    objects.push_back( children[i]->object );
		
		std::vector< HTMLObject* > obj;
		if( (obj = children[i]->getObject( w_x, w_y ) ).size() )
		{
		    objects.insert( objects.end(),  obj.begin(), obj.end() );
		}
	    }
	}
    }

    return objects;
}

HTMLDC* HTMLDC::getDC( Ref<HTMLObject> obj )
{
    for( unsigned i = 0; i < children.size(); ++i )
    {
	if( children[i]->object == obj.address() )
	    return children[i];

	HTMLDC *dc = children[i]->getDC( obj );

	if( dc )
	    return dc;
    }

    return 0;
}
