#include "object_image.h"
#include "node.h"
#include "image.h"

int ObjectImage::TYPE_ID = HTMLObject::object_type_id_count++;

ObjectImage::ObjectImage( const Ref<HTMLObject> & parent, const Ref<Node> & node )
    : HTMLObject( parent, node )
{
    object_type_id = TYPE_ID;
    classname = "ObjectImage";

    image = 0;
    failed_loading = false;

    FXString value;    

    node->getAttribute( "src", file );
    
    img_width  = 0;
    img_height = 0;
    border     = 0;

    if( node->getAttribute( "width", value ) )
	value.scan( "%d", &img_width );

    value.clear();
    if( node->getAttribute( "height", value ) )
	value.scan( "%d", &img_height );

    value.clear();
    if( node->getAttribute( "border", value ) )
	value.scan( "%d", &border );

    setLayoutHint( LAYOUT_FIX_SIZE );

    loadImage( file ); // Tell Url loader to load this
}

void ObjectImage::layout()
{
    if( !layouted )
    {
	layouted = true;
	failed_loading = false;
	image = new Image( file, this );
    }

    if( !image->valid() )
    {
	if( !failed_loading )
	    getImage( file, image );

	if( !image->valid() )
	    failed_loading = true;

	if( image->valid() )
	{
	    if( !img_width )
		img_width = image->getWidth();

	    if( !img_height )
		img_height = image->getHeight();

	    if( ( img_width != image->getWidth() ) || ( img_height != image->getHeight() ) )
		image->scale( img_width, img_height );

	}
    }

    width = img_width + border;
    height = img_height + border;

    min_width = img_width + border;
    min_height = img_width + border;
}

void ObjectImage::paint()
{
    dc.setColor( 0 );

    for( int i = 0; i < border; ++i )
	dc.drawRectangle( i, i, width - i * 2, height -i * 2);

    if( markup )
    {
	dc.setColor( 0xff0000 );
	dc.fillRectangle( 0, 0, width, height );

    } else if( image->valid() ) {

	dc.drawImage( image->getImage(), border, border );

    } 
}

void ObjectImage::notify()
{
//     ObjectImage::layout();
}
