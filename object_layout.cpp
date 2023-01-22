#include "object_layout.h"
#include "object_hr.h"
#include "node.h"
#include "string_utils.h"

int ObjectLayout::TYPE_ID = HTMLObject::object_type_id_count++;

ObjectLayout::ObjectLayout( const Ref<HTMLObject> & parent, const Ref<Node> & node )
    : HTMLObject( parent, node )      
{
    object_type_id = TYPE_ID;
    classname = "ObjectLayout";

    spacer = -1;

    FXString name = node->getName();
    name.lower();

    setLayoutHint( LAYOUT_MANAGER );

    if( name == "div" ||
	name == "center" ||
	name == "pre" )
    {
	setLayoutHint( LAYOUT_FORCE_LNBR_BEFORE );
	setLayoutHint( LAYOUT_FORCE_LNBR_AFTER );
    }

    if( name == "br" )
	setLayoutHint( LAYOUT_FORCE_LNBR_BEFORE );

    if( name == "div" )
    {
	FXString value;
	if( node->getAttribute( "align", value ) )
	    setStyle( "align", value.lower() );
    }

    if( name == "h1" )
    {
	setStyle( "font-size", "21" );
	setStyle( "font-weight", FONTWEIGHT_BOLD );
	setLayoutHint( LAYOUT_FORCE_LNBR_BEFORE );
	setLayoutHint( LAYOUT_FORCE_LNBR_AFTER );
    }

    if( name == "h2" )
    {
	setStyle( "font-size", "18" );
	setStyle( "font-weight", FONTWEIGHT_BOLD );
	setLayoutHint( LAYOUT_FORCE_LNBR_BEFORE );
	setLayoutHint( LAYOUT_FORCE_LNBR_AFTER );
    }


    if( name == "h3" )
    {
	setStyle( "font-size", "15" );
	setStyle( "font-weight", FONTWEIGHT_BOLD );
	setLayoutHint( LAYOUT_FORCE_LNBR_BEFORE );
	setLayoutHint( LAYOUT_FORCE_LNBR_AFTER );
    }


    if( name == "h4" )
    {
	setStyle( "font-size", "12" );
	setStyle( "font-weight", FONTWEIGHT_BOLD );
	setLayoutHint( LAYOUT_FORCE_LNBR_BEFORE );
	setLayoutHint( LAYOUT_FORCE_LNBR_AFTER );
    }

    if( name == "h5" )
    {
	setStyle( "font-size", "10" );
	setStyle( "font-weight", FONTWEIGHT_BOLD );
	setLayoutHint( LAYOUT_FORCE_LNBR_BEFORE );
	setLayoutHint( LAYOUT_FORCE_LNBR_AFTER );
    }

    if( name == "b" )
	setStyle( "font-weight", FONTWEIGHT_BOLD );

    if( name == "i" )
	setStyle( "font-slant", FONTSLANT_ITALIC );

    if( name == "code" )
    {
	setStyle( "font-face", "courier" );
	setLayoutHint( LAYOUT_ADOPT_MY_CHILDS );
    }

    if( name == "hr" )
    {
	addChield( new ObjectHR( Ref<HTMLObject>( this, false ), node ) );
	setLayoutHint( LAYOUT_FORCE_LNBR_BEFORE );
	setLayoutHint( LAYOUT_FORCE_LNBR_AFTER );
    }

    align = ALIGN_LEFT;

    FXString salign = getStyle( "align" , "left" );

    if( salign == "center" )
	align = ALIGN_CENTER;
    
    if( salign == "right" )
	align = ALIGN_RIGHT;    
}

void ObjectLayout::layout()
{
    bool ll = layouted;

//    printf( "ObjectLayout::layout()\n" );
    HTMLObject::layout();

    if( !ll )
    {
	FXFont *f = getFont( getStyle( "font-face",     getFont( 0 )->getName() ), 
		 string2uint( getStyle( "font-size",     getFont( 0 )->getSize() / 10 ) ) ,
		 string2uint( getStyle( "font-weight",   getFont( 0 )->getWeight() ) ),
		 string2uint( getStyle( "font-slant",    getFont( 0 )->getSlant() ) ),
		 string2uint( getStyle( "font-encoding", getFont( 0 )->getEncoding() ) ),
		 string2uint( getStyle( "font-width",    getFont( 0 )->getSetWidth() ) ),
		 string2uint( getStyle( "font-hints",    getFont( 0 )->getHints() ) ),
		 current_font );
	spacer = f->getTextWidth( " ", 1 );    

	for( unsigned i = 0; i < children.size(); ++i )
	{
	    if( children[i]->getLayoutHint( LAYOUT_ADOPT_MY_CHILDS ) && !children[i]->getLayoutHint( LAYOUT_DEAMONIZE ) )
	    {
		children[i]->setLayoutHint( LAYOUT_DEAMONIZE );
		deamons.push_back( children[i] );

		int size = children[i]->getChildNum();
		Ref<HTMLObject> o = children[i];

		for( int j = 0; j < size; j++ )
		{
		    if( j == 0 )
			children[i] = o->getChild( 0 );
		    else
			insertChild( o->getChild( j ), i + j );
		}
	    }
	}
    }

    width = 0;
    height = 0;
    
    int max_width = 0;
    int max_height = 0;

    for( unsigned i = 0; i < children.size(); ++i )
    {
	if( children[i]->getLayoutHint( LAYOUT_FORCE_LNBR_BEFORE ) )
	{	  
	    if( max_width > width )
		width = max_width;
	    
	    max_width = 0;

	    height += max_height;

	    max_height = 0;
	}
	    
	max_width += children[i]->getWidth();

	if( children[i]->getHeight() > max_height )
	    max_height = children[i]->getHeight();

	if( children[i]->getLayoutHint( LAYOUT_FORCE_LNBR_AFTER ) )
	{
	    if( max_width > width )
		width = max_width;

	    max_width = 0;

	    height += max_height;
	    max_height = 0;
	}
	    	
	if( children[i]->getLayoutHint( LAYOUT_AUTOSPACE ) && max_width )
	    max_width += spacer;	
    }
    
    if( max_width > width )
	width = max_width;

    HTMLObject::max_width = width;

    height += max_height;

    min_width = 0;
    min_height = 0;
    
    for( unsigned i = 0; i < children.size(); ++i )
    {
	if( children[i]->getMinWidth() > min_width )
	    min_width = children[i]->getMinWidth();

	if( children[i]->getMinHeight() > min_height )
	    min_height = children[i]->getMinHeight();
    }    
}

void ObjectLayout::paint()
{
    int x = 0, y = 0;

    lines.clear(); // if all calculation is done correct this line can be commented
    int h = -1;

    if( lines.empty() )
	calcLines( lines, width, h );

    if( h > height )
	printf( "h > height => %d > %d min_height: %d\n", h, height, min_height );

    x = 0;

    if( markup )
    {
	dc.setColor( 0 );
	dc.drawRectangle( 0, 0, width, height );
    }

    for( unsigned k = 0; k < lines.size(); ++k )
    {
	x = 0;
	int max_height = 0;

	if( align != ALIGN_LEFT )
	{
	    int ww = 0;

	    for( unsigned j = 0; j < lines[k].size(); ++j )
	    {
		int i = lines[k][j];		
		ww += children[i]->getWidth();
		if( children[i]->getLayoutHint( LAYOUT_AUTOSPACE ) && j > 0 )
		    ww += spacer;
	    }
	    
	    if( align == ALIGN_CENTER )
		x = width / 2 - ww / 2;
	    
	    if( align == ALIGN_RIGHT )
		x = width - ww;
	}

	for( unsigned j = 0; j < lines[k].size(); ++j )
	{
	    int i = lines[k][j];		
	    
	    int w = children[i]->getWidth();
	    int h = children[i]->getHeight();
	    
	    if( h > max_height )
		max_height = h;
	    
	    int next_x = x + w;	    	    

	    // Ref<HTMLDC> cdc = dc->getDC( children[i], x, y, w, h );
	    dc.addChild( children[i]->getDC() );
	    children[i]->getDC()->setRect( &dc, x, y, w, h );

	    if( children[i]->getDC()->visible() )
		children[i]->paint();

	    for( unsigned ii = 0; ii < deamons.size(); ++ii )
	    {
		for( int jj = 0; jj < deamons[ii]->getChildNum(); ++jj )
		{
		    if( children[i].address() == deamons[ii]->getChild( jj ).address() )
		    {
			// dc->getDC( deamons[ii], x, y, w, h );
			dc.addChild( deamons[ii]->getDC() );
			deamons[ii]->getDC()->setRect( &dc, x, y, w, h );
		    }
		}
	    }

	    if( children[i]->getLayoutHint( LAYOUT_AUTOSPACE ) && next_x > 0 )
		next_x += spacer;
	    
	    x = next_x;
	}   
	
	y += max_height;
    }
}

void ObjectLayout::setWidth( int w )
{
    if( w == width )
	return;

    width = w;

    if( width < min_width )
	printf( "ObjectLayout::setWidth width < min_width => %d < %d\n", width, min_width );

    int h = -1;

    lines.clear();
    calcLines( lines, width, h );

    height = h;
/*
    if( height < min_height )
	printf( "ObjectLayout::setWidth height < min_height => %d < % d\n", height, min_height );
*/
}

void ObjectLayout::setHeight( int h )
{
    if( h < height )
	printf( "setHeigth h < height => %d < %d\n", h, height );

    height = h;
}

void ObjectLayout::calcLines( std::vector< std::vector<int> > & lines, int & g_width, int & g_height )
{
    std::vector<int> line;
    
    int x = 0;

    for( unsigned i = 0; i < children.size(); ++i )
    {
	if( children[i]->getLayoutHint( LAYOUT_WIDTH_PERCENTAGE ) )
	{
	    double p = g_width / 100.0 * children[i]->getPWidth();

	    if( children[i]->getMinWidth() < p )
		children[i]->setWidth( (int)p );
	    else
		children[i]->setWidth( children[i]->getMinWidth() );
	}
    }

    for( unsigned i = 0; i < children.size(); ++i )
    {
	int w = children[i]->getWidth();
	int extra = 0;

	if( children[i]->getLayoutHint( LAYOUT_AUTOSPACE ) && x > 0 )
	    extra = spacer;

	if( children[i]->getLayoutHint( LAYOUT_FORCE_LNBR_BEFORE ) )
	{
	    if( !line.empty() )
		lines.push_back( line );

	    line = std::vector<int>();
	    line.push_back( i );
	    
	    if( children[i]->getLayoutHint( LAYOUT_FORCE_LNBR_AFTER ) )
	    {
		lines.push_back( line );
		line = std::vector<int>();
	    }

	    x = 0;
	    continue;
	} 


	if( x + w + extra > g_width )
	    if( ( g_width - x - extra ) >= children[i]->getMinWidth() && 
		!children[i]->getLayoutHint( LAYOUT_FIX_SIZE ) )
	    {
		children[i]->setWidth( g_width - x - extra );
		w = g_width - x - extra;
	    }


	if( children[i]->getLayoutHint( LAYOUT_FORCE_LNBR_AFTER ) )
	{
	    line.push_back( i );
	    lines.push_back( line );
	    line = std::vector<int>();

	    x = 0;
	    continue;
	}

	
	w += extra;

	if( x + w > g_width )
	{
	    if( line.empty() )
	    {
		line.push_back( i );
		lines.push_back( line );
		line = std::vector<int>();	    
		x = 0;
	    } else {
		lines.push_back( line );
		line = std::vector<int>();	    
		line.push_back( i );
		x = w;
	    }

	    continue;
	}

	line.push_back( i );
	x += w;
    }

    if( !line.empty() )
	lines.push_back( line );


    for( unsigned k = 0; k < lines.size(); ++k )
    {
	for( unsigned j = 0; j < lines[k].size(); ++j )
	{
	    int i = lines[k][j];		

	    if( lines[k].size() == 1 )
	    {
		if ( !children[i]->getLayoutHint( LAYOUT_FIX_SIZE ) )
		{
		    if( children[i]->getLayoutHint( LAYOUT_MANAGER ) ||
			children[i]->getWidth() > g_width ||
			children[i]->getMaxWidth() > children[i]->getWidth()
			)
		    {
			children[i]->setWidth( g_width );
		    }
		}
	    }
	}
    }


    if( g_height == -1 )
    {
	int h = 0;

	for( unsigned k = 0; k < lines.size(); ++k )
	{
	    int max_height = 0;
	    
	    for( unsigned j = 0; j < lines[k].size(); ++j )
	    {
		int i = lines[k][j];

		if( children[i]->getHeight() > max_height )
		    max_height = children[i]->getHeight();
	    }
	    
	    h += max_height;
	}

	g_height = h;
    }
}
