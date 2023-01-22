#ifndef HTML_DC_H
#define HTML_DC_H

#include <fox/fx.h>
#include "ref.h"
#include <vector>

class HTMLObject;

class HTMLDC
{
 private:
    FXDCWindow *dc;

    int x;
    int y;
    int width;
    int height;

    int vx;
    int vy;
    int vwidth;
    int vheight;

    std::vector< HTMLDC* > children;
    HTMLObject*  object;

 public:
    HTMLDC( HTMLObject *object, FXDCWindow *dc = 0 ) : dc( dc ), object( object ) {}

/*
    HTMLDC( FXDCWindow *dc, int x, int y, int width, int height )
	: dc( dc ), x(x), y(y), width(width), height( height ) {}
*/

    int getWidth() { return width; }
    int getHeight() { return height; }
/*
    Ref<HTMLDC> getDC( Ref<HTMLObject> obj, int w_x, int w_y, int w_width, int w_height )
	{ 
	    Ref<HTMLDC> cdc = new HTMLDC( dc, x + w_x, y + w_y, w_width, w_height ); 
	    cdc->object = obj;
	    children.push_back( cdc );
	    return cdc;
	}
*/

    void setColor( unsigned color ) 
	{ dc->setForeground( FXRGB( (color & 0xff0000) >> 16 , (color & 0x00ff00) >> 8, color & 0x0000ff) ); }
    void setColor( int red, int green, int blue ) { dc->setForeground( FXRGB( red, green, blue ) ); }
    void drawRectangle( int x, int y, int width, int height );
    void fillRectangle( int x, int y, int width, int height );
    void drawText( int x, int y, const FXString & text, unsigned length = 0 );
    void drawLine( int x1, int y1, int x2, int y2 );    
    void drawArc( int w_x, int w_y, int w, int h, int ang1, int ang2 )
	{ dc->drawArc( x + w_x, y + w_y, w, h, ang1, ang2 ); }
    void fillArc( int w_x, int w_y, int w, int h, int ang1, int ang2 )
	{ dc->fillArc( x + w_x, y + w_y, w, h, ang1, ang2 ); }

    void drawImage( FXImage *image, int w_x, int w_y ) { dc->drawImage( image, x + w_x, y + w_y ); }

    int getFontHeight();
    int getTextWidth( const FXString & text );

    void setFont( FXFont *font ) { dc->setTextFont( font ); }

    std::vector< HTMLObject* > getObject( int x, int y );
    HTMLDC* getDC( Ref<HTMLObject> obj );

    int getX() { return x; }
    int getY() { return y; }

    void setRect( FXDCWindow *dc_, int x_, int y_, int width_, int height_ ) 
	{ dc = dc_; x = x_; y = y_; width = width_; height = height_; clear(); }

    void setRect( HTMLDC *pdc, int x_, int y_, int width_, int height_ ) 
	{ 
	    dc = pdc->dc; x = pdc->x + x_; y = pdc->y + y_; width = width_; height = height_; clear(); 
	    vx = pdc->vx; vy = pdc->vy; vwidth = pdc->vwidth; vheight = pdc->vheight;
	}

    void setViewPort( int x, int y, int width, int height ) { vx = x; vy = y; vwidth = width; vheight = height; }
    
    bool visible() 
	{ 
	    return 
		( x <= vx && y <= vy && x + width > vx && y + height > vy ) ||
		( vx <= x && vy <= y && vx + vwidth > x && vy + vheight > y ) ||
		( x <= vx && y >= vy && x + width > vx && vy + vheight > y ) ||
		( vx <= x && vy >= y && vx + vwidth > x && y + height > vy );
	}

    void clear() { children.clear(); }
    void addChild( HTMLDC *cdc ) { children.push_back( cdc ); }
};


#endif
