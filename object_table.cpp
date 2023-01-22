#include "object_table.h"
#include "node.h"
#include "string_utils.h"

int ObjectTable::TYPE_ID = HTMLObject::object_type_id_count++;

ObjectTable::ObjectTable( const Ref<HTMLObject> & parent, const Ref<Node> & node )
    : HTMLObject( parent, node )
{
    object_type_id  = TYPE_ID;
    classname = "ObjectTable";

    cellspacing = 2;
    cellpadding = 5;
    border      = 0;
    rowspanned  = false;

    FXString value;
    
    if( node->getAttribute( "cellspacing", value ) )
	value.scan( "%d", &cellspacing );

    value.clear();
    if( node->getAttribute( "cellpadding", value ) )
	value.scan( "%d", &cellpadding );

    value.clear();
    if( node->getAttribute( "border", value ) )
	value.scan( "%d", &border );

    value.clear();
    if( node->getAttribute( "valign", value ) )
	setStyle( "table-valign", value );
    else
	setStyle( "table-valign", "middle" );

    bgcolor = -1;
    value.clear();
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


    setStyle( "table-border", border );
    setStyle( "table-cellpadding", cellpadding );
    setStyle( "table-cellspacing", cellspacing );
    setStyle( "align", "left" );

    delLayoutHint( LAYOUT_MANAGER );
}

void ObjectTable::layout()
{
    HTMLObject::layout();

    int mwidth = 0;   

    if( getLayoutHint( LAYOUT_FIX_SIZE ) )
	mwidth = width;

    width = cellspacing;
    height = 0;
    min_width = cellspacing;
    min_height = 0;

    std::vector<int> cols_w, cols_mw;

    calcRows( cols_w, cols_mw );

    for( unsigned i = 0; i < cols_w.size(); ++i )
    {
	width += cols_w[i] + cellspacing;
	min_width += cols_mw[i] + cellspacing;
    }

    width += border * 2;
    min_width += border * 2;
    max_width = width;

    if( mwidth )
    {
	width = mwidth;
	min_width = mwidth;
    }

    setWidth( width );
}

void ObjectTable::setWidth( int w_width )
{
    if( w_width < min_width )
	printf( "w < min_width => %d < %d\n", w_width, min_width );

/*
    if( w_width - border * 2 < min_width )
	printf( "w - border * 2 < min_width => %d < %d\n", w_width - border * 2, min_width );
*/

    width = w_width;
    w_width -= border * 2;
    height = cellspacing;

    for( unsigned i = 0; i < children.size(); ++i )
    {
	for( int k = 0; k < children[i]->getChildNum(); ++k )
	{
	    if( children[i]->getChild(k)->getLayoutHint( LAYOUT_WIDTH_PERCENTAGE ) )
	    {
		double p = w_width / 100.0 * children[i]->getChild(k)->getPWidth();

		if( children[i]->getChild(k)->getMinWidth() <= p )
		    children[i]->getChild(k)->setWidth( (int)p );
		else
		    children[i]->getChild(k)->setWidth( children[i]->getChild(k)->getMinWidth() );
	    }
	}
    }

    std::vector<int> cols_w, cols_mw;

    calcRows( cols_w, cols_mw );

    std::vector<int> rcols_w = cols_w; // required for colspan

    int cw = cellspacing;
    int mw = cellspacing;

    for( unsigned i = 0; i < cols_w.size(); ++i )
    {
	cw += cols_w[i] + cellspacing;
	mw += cols_mw[i] + cellspacing;
    }

    bool found_colspan = false;
    std::vector< std::vector<int> > rcells;

    for( unsigned i = 0; i < children.size(); ++i )    
    {
	int r_width = 0;
	int smaller = 0;
	int grow  = 0;	
	bool found_rowspan = false;
	std::vector<int> rc;

	if( cw > w_width )
	    smaller = cw - w_width;		

	if( mw > w_width )
	{
	    printf( "ObjectTable::setWidth mw > width => %d > %d\n", mw, w_width );
	    smaller = cw - mw;
	}

       
	if( cw < w_width )
	    grow = (w_width - cw) / cols_w.size();	

	for( int k = 0; k < children[i]->getChildNum(); ++k )
	{

//	    printf( "i: %d k: %d num: %d\n", i, k, children[i]->getChildNum() );
	    int colspan = 0;
	    children[i]->getChild(k)->getStyle( "cell-colspan", "0" ).scan( "%d", &colspan );

	    if( colspan )
		found_colspan = true;

	    int rowspan = 0;	    
	    children[i]->getChild(k)->getStyle( "cell-rowspan", "0" ).scan( "%d", &rowspan );

	    if( rowspan )
	    {
		found_rowspan = true;
		rc.push_back( k );
	    }


	    if( smaller > 0 )
	    {
		if( cols_mw[k] <= cols_w[k] )
		{
		    if( cols_w[k] - cols_mw[k] > smaller )
		    {
			children[i]->getChild(k)->setWidth( cols_w[k] - smaller );
			rcols_w[k] = cols_w[k] - smaller;
			r_width += cols_w[k] - smaller;
			smaller = 0;
		    } else {
			children[i]->getChild(k)->setWidth( cols_mw[k] );
			rcols_w[k] = cols_mw[k];
			smaller -= cols_w[k] -  cols_mw[k];
			r_width += cols_mw[k] + cellspacing;

/*
			printf( "w: %d mw: %d smaller: %d MinWidth: %d i: %d k: %d num: %d\n", cols_w[k], cols_mw[k], smaller,
				children[i]->getChild(k)->getMinWidth(), i, k, children[i]->getChildNum() );
			printf( "min_width: %d width: %d w_width: %d cw: %d mw: %d\n", min_width, width, w_width, cw, mw );
*/
		    }
		} else {
		    printf( "ObjectTable::setWidth ASSERT! mw > w => %d > %d\n", cols_mw[k], cols_w[k] );
		}
	    } else {		
		children[i]->getChild(k)->setWidth( cols_w[k] + grow );
		rcols_w[k] = cols_w[k] + grow;
		r_width += cols_w[k] + cellspacing + grow;	   
	    }
	}
	rcells.push_back( rc );

	children[i]->setWidth( r_width - cellspacing );
	height += children[i]->getHeight() + cellspacing;
    }

    if( found_colspan )
    {
	height = cellspacing;

	for( unsigned i = 0; i < children.size(); ++i )    
	{
	    int r_width = 0;
	    
	    for( int k = 0; k < children[i]->getChildNum(); ++k )
	    {
		int colspan = 0;
		children[i]->getChild(k)->getStyle( "cell-colspan", "0" ).scan( "%d", &colspan );
		
		int colspan_space = 0;
		unsigned ik = k;
		
		while( colspan > 0 )
		{
		    ++ik;
		    if( ik < cols_w.size() )
			colspan_space += rcols_w[ik] + cellspacing;
		    else
			break;
		    
		    colspan--;
		}	    
		
		children[i]->getChild(k)->setWidth( children[i]->getChild(k)->getWidth() + colspan_space );
		r_width += rcols_w[k] + cellspacing;	   
	    }
	    
	    children[i]->setWidth( r_width - cellspacing );
	    height += children[i]->getHeight() + cellspacing;
	}	
    }

    for( unsigned i = 0; i < children.size(); ++i )    
	for( int k = 0; k < children[i]->getChildNum(); ++k )
	{
/*
	    if( children[i]->getChild( k )->getMinHeight() > children[i]->getChild( k )->getHeight() )
		printf( "here\n" );
*/

	    children[i]->getChild( k )->setHeight( children[i]->getHeight() );
	}


    for( unsigned i = 0; i < rcells.size(); ++i )
    {
	for( unsigned j = 0; j < rcells[i].size(); ++j )
	{
	    unsigned k = i + 1;
	    int rowspan_height = children[i]->getChild( rcells[i][j] )->getHeight();

	    int rowspan = 0;	    
	    children[i]->getChild(rcells[i][j])->getStyle( "cell-rowspan", "0" ).scan( "%d", &rowspan );

	    while( k < rcells.size() && rowspan > 0 )
	    {
		rowspan_height += children[k]->getHeight() + cellspacing;
		++k;
		rowspan--;
	    }

	    children[i]->getChild( rcells[i][j] )->setHeight( rowspan_height );
	}
    }


    height += border * 2;
}

void ObjectTable::paint()
{    
    for( int i = 0; i < border; ++i )
    {
	int w = width;

	dc.setColor( 0xeeeeee );
	dc.drawLine( i, i, w - i - 1, i );
	dc.drawLine( i, i, i, height - i - 1 );

	dc.setColor( 0x888888 );
	dc.drawLine( w - i - 1, i, w - i - 1, height - i  -1 );
	dc.drawLine( i, height - i - 1, w - i - 1, height - i - 1);	

    }

    int y = cellspacing + border;

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
	dc.fillRectangle( border, border, width - border, height - border );
    }

    for( unsigned i = 0; i < children.size(); ++i )
    {
//	Ref<HTMLDC> cdc = dc->getDC( children[i], border + cellspacing, y, children[i]->getWidth(), children[i]->getHeight() );
	dc.addChild( children[i]->getDC() );
	children[i]->getDC()->setRect( &dc, border + cellspacing, y, children[i]->getWidth(), children[i]->getHeight() );
	y += children[i]->getHeight() + cellspacing;

	children[i]->paint();
    }
}

void ObjectTable::calcRows( std::vector<int> & cols_w, std::vector<int> & cols_mw )
{
    std::vector< std::vector<int> > rcells;

    for( unsigned i = 0; i < children.size(); ++i )
    {
	int was_colspan = 0;
	int colspan_width = 0;
	bool found_rowspan = false;
	std::vector<int> rc;

	for( int k = 0; k < children[i]->getChildNum(); ++k )
	{
//	    children[i]->getChild(k)->layout();

	    int colspan = 0;

	    if( was_colspan <= 0 )
		children[i]->getChild(k)->getStyle( "cell-colspan", "0" ).scan( "%d", &colspan );

	    int rowspan = 0;	    

	    if( !rowspanned )
	    {
		children[i]->getChild(k)->getStyle( "cell-rowspan", "0" ).scan( "%d", &rowspan );

		if( rowspan )
		{
		    found_rowspan = true;
		    rc.push_back( k );
		}
	    }

	    if( colspan )
	    {
		colspan_width = children[i]->getChild( k )->getWidth() / (colspan + 1);
		was_colspan = colspan + 1;
	    }

	    if( cols_w.size() <= (unsigned)k )
	    {
		if( was_colspan > 0 )
		    cols_w.push_back( colspan_width );
		else
		    cols_w.push_back( children[i]->getChild(k)->getWidth() );

		cols_mw.push_back( children[i]->getChild(k)->getMinWidth() );
	    }

	    if( was_colspan > 0 )
	    {
		if( cols_w[k] < colspan_width )
		    cols_w[k] = colspan_width;
		
	    } else {

		if( cols_w[k] < children[i]->getChild(k)->getWidth() )
		    cols_w[k] = children[i]->getChild(k)->getWidth();		

	    }

	    if( cols_mw[k] < children[i]->getChild(k)->getMinWidth() )
		cols_mw[k] = children[i]->getChild(k)->getMinWidth();

	    was_colspan--;
	}

	if( !rowspanned )
	    rcells.push_back( rc );
    }

    if( !rowspanned )
    {
	for( unsigned i = 0; i < rcells.size(); ++i )
	{
	    for( unsigned j = 0; j < rcells[i].size(); ++j )
	    {
		unsigned k = i + 1;
		int rowspan = 0;	    
		children[i]->getChild(rcells[i][j])->getStyle( "cell-rowspan", "0" ).scan( "%d", &rowspan );
		
		while( k < rcells.size() && rowspan )
		{
		    children[k]->insertChild( new HTMLObject( children[k] ), rcells[i][j] );
		    ++k;
		    rowspan--;
		}
	    }
	}
    }

    rowspanned = true;
}
