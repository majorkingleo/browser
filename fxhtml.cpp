#include "fxhtml.h"
#include "node.h"
#include "html_object.h"
#include "cppdir.h"
#include "build_tree.h"
#include "image.h"
#include "image_size.h"

#include <fox/FXPNGImage.h>
#include <fox/FXJPGImage.h>


FXDEFMAP( FXHTMLWindow ) FXHTMLWindowMap[] = {
};

FXIMPLEMENT( FXHTMLWindow, FXScrollWindow, FXHTMLWindowMap, ARRAYNUMBER( FXHTMLWindowMap ) );

FXHTMLWindow::FXHTMLWindow( FXComposite *p, FXObject *tgt, FXSelector sel, FXuint opts, FXint x, FXint y, FXint w, FXint h )
    : FXScrollWindow( p, opts, x, y, w, h )
{
    target = tgt;
    selector = sel;
    html = new FXHTML( this );
}

void FXHTMLWindow::layout()
{
    html->layout();
    FXScrollWindow::layout();
}

void FXHTMLWindow::loadFile( const FXString &file )
{
    html->loadFile( file );
}

void FXHTMLWindow::doUpdate()
{
    html->doUpdate();
    recalc();
}

int FXHTMLWindow::getInnerWidth()
{
    return width - vertical->getDefaultWidth();
}

int FXHTMLWindow::getInnerHeight()
{
    return height - horizontal->getDefaultHeight();
}

void FXHTMLWindow::inform( int message, void *data )
{
    if( !target )
	return;

    switch( message )
    {
	case HTML_LOAD_URL: target->handle( this, MKUINT( selector, SEL_COMMAND ), 
					    (void*)new Message( HTML_LOAD_URL, new FXString( *((FXString*)data) ) ) ); 
	case HTML_LINK_INFO: target->handle( this, MKUINT( selector, SEL_COMMAND ), 
					    (void*)new Message( HTML_LINK_INFO, new FXString( *((FXString*)data) ) ) ); 
	    break;
    }
}

FXDEFMAP( FXHTML ) FXHTMLMap[] = {
    FXMAPFUNC( SEL_PAINT,             0, FXHTML::onPaint ),
    FXMAPFUNC( SEL_LEFTBUTTONPRESS,   0, FXHTML::onMouseDown ),
    FXMAPFUNC( SEL_LEFTBUTTONRELEASE, 0, FXHTML::onMouseUp ),
    FXMAPFUNC( SEL_MOTION,            0, FXHTML::onMouseMove ),
    FXMAPFUNC( SEL_CLICKED,           0, FXHTML::onMouseClick ),
    FXMAPFUNC( SEL_DOUBLECLICKED,     0, FXHTML::onMouseDoubleClick ),
    FXMAPFUNC( SEL_TIMEOUT,           FXHTML::ID_IMAGE_TIMER, FXHTML::onCheckImages )
};

FXIMPLEMENT( FXHTML, FXCanvas, FXHTMLMap, ARRAYNUMBER( FXHTMLMap ) );

FXHTML::FXHTML() {}

FXHTML::FXHTML( FXHTMLWindow *p )
    : FXCanvas( p, 0, 0 )
{
    image_timer = getApp()->addTimeout( 500, this, ID_IMAGE_TIMER );
}

FXHTML::~FXHTML()
{
    deleteImages();
}

Ref<HTMLObject> FXHTMLWindow::getDOM()
{
    return html->getDOM();
}

void FXHTML::layout()
{
//    printf( "FXHTML::layout()\n" );
    
    FXScrollArea *area = dynamic_cast<FXScrollArea*>( getParent() );

    if( !area )   
	printf( "parent is not a FXScrollArea\n" );

    width = area->getViewportWidth();
    height= area->getViewportHeight();

//    printf( "fxhtml width: %d height: %d\n", width, height );

    if( root.valid() )
    {
	root->layout();   

//	printf( "width: %d  root width: %d\n", width, root->getWidth() );
//	printf( "height: %d  root height: %d\n", height, root->getHeight() );

	if( height < root->getHeight() )
	    height = root->getHeight();
	
	if( width < root->getWidth() )
	    width = root->getWidth();
    }
}

long FXHTML::onPaint( FXObject *obj, FXSelector sel, void *p )
{
    FXEvent *ev = (FXEvent*) p;
    
    FXDCWindow* dc;
    
    if( ev )
	dc = new FXDCWindow( this, ev );
    else
	dc = new FXDCWindow( this );

    dc->setTextFont( getApp()->getNormalFont() );

    if( root.valid() )
    {
	dc->setTextFont( getApp()->getNormalFont() );
	
	// html_dc = new HTMLDC( dc, 0, 0, width, height );    
	
	root->getDC()->setRect( dc, 0, 0, width, height );

	FXScrollArea *area = dynamic_cast<FXScrollArea*>( getParent() );

	root->getDC()->setViewPort( getX() * -1, getY() * -1, area->getViewportWidth(), area->getViewportHeight() );
//	printf( "Viewport: %d %d %d %d\n", getX(), getY(), area->getViewportWidth(), area->getViewportHeight() );
	root->paint();
    } else {
	
	dc->setForeground( FXRGB( 100, 0, 0 ) );
	dc->fillRectangle( 0, 0, width, height );
	dc->setForeground( FXRGB( 50, 50, 200 ) );

	FXString text = "<FXHTML> ";
	int twidth = dc->getTextFont()->getTextWidth( text.text(), text.length() );
	int theight = dc->getTextFont()->getFontHeight();
	int init = 0;
	
	for( int i = theight / 2; i < height; i += theight + 10 )
	{
	    for( int j = init; j < width; j += twidth )
		dc->drawText( j, i + 10, text.text(), text.length() );

	    if( init )
		init = 0;
	    else
		init = twidth / 2 * -1;
	}
    }

    delete dc;

    return 1;
}

long FXHTML::onMouseDown( FXObject *obj, FXSelector sel, void *p )
{
    FXEvent *ev = (FXEvent*)p;

    if( root.valid() )
    {
	std::vector< HTMLObject* > objects = root->getDC()->getObject( ev->win_x, ev->win_y );

	for( unsigned i = 0; i < objects.size(); ++i )
	    objects[i]->onMouseDown( ev->win_x, ev->win_y );	    	
    }


    return 1;
}

long FXHTML::onMouseUp( FXObject *obj, FXSelector sel, void *p )
{
    FXEvent *ev = (FXEvent*)p;

    if( root.valid() )
    {
	std::vector< HTMLObject* > objects = root->getDC()->getObject( ev->win_x, ev->win_y );

	for( unsigned i = 0; i < objects.size(); ++i )
	    objects[i]->onMouseUp( ev->win_x, ev->win_y );	    	
    }

    return 1;
}


long FXHTML::onMouseClick( FXObject *obj, FXSelector sel, void *p )
{
    FXEvent *ev = (FXEvent*)p;

    if( root.valid() )
    {
	std::vector< HTMLObject* > objects = root->getDC()->getObject( ev->win_x, ev->win_y );

	for( unsigned i = 0; i < objects.size(); ++i )
	    objects[i]->onMouseClick( ev->win_x, ev->win_y );	    	
    }

    return 1;
}


long FXHTML::onMouseDoubleClick( FXObject *obj, FXSelector sel, void *p )
{
    FXEvent *ev = (FXEvent*)p;

    if( root.valid() )
    {
	std::vector< HTMLObject* > objects = root->getDC()->getObject( ev->win_x, ev->win_y );

	for( unsigned i = 0; i < objects.size(); ++i )
	    objects[i]->onMouseDoubleClick( ev->win_x, ev->win_y );	    	
    }

    return 1;
}


long FXHTML::onMouseMove( FXObject *obj, FXSelector sel, void *p )
{
    FXEvent *ev = (FXEvent*)p;

    if( root.valid() )
    {
	std::vector< HTMLObject* > objects = root->getDC()->getObject( ev->win_x, ev->win_y );

	for( unsigned i = 0; i < objects.size(); ++i )
	{
	    bool found = false;
	    for( unsigned j = 0; j < last_objects.size(); ++j )
		if( objects[i] == last_objects[j] )
		{
		    found = true;
		    break;
		}

	    if( !found )
		objects[i]->onMouseEnter( ev->win_x, ev->win_y );

	    objects[i]->onMouseMove( ev->win_x, ev->win_y );
	}

	for( unsigned j = 0; j < last_objects.size(); ++j )
	{
	    bool found = false;

	    for( unsigned i = 0; i < objects.size(); ++i )
		if( last_objects[j] == objects[i] )
		{
		    found = true;
		    break;
		}

	    if( !found )
		last_objects[j]->onMouseLeave( ev->last_x, ev->last_y );
	}

	last_objects = objects;

    }

    return 1;
}

void FXHTML::loadFile( const FXString &mfile )
{
    FXString file( mfile );

    if( !file.length() )
	return;
    
    FXURL2 url( file, base_url );
    base_url = url.getBaseUrl();

    printf( "url: %s protocol: %d type: %d hostname: %s base: %s file: %s\n", 
	    url.getUrl().text(), url.getProtocol(), url.getType(), url.getHostname().text(), url.getBase().text(), file.text() );


    if( url.getProtocol() != FXURL2::PROTOCOL_LOCAL_FILE )
    {
	if( loader.getUrl( url.getUrl(), file ) )
	    printf( "%s downloaded as %s\n", url.getUrl().text(), file.text() );	    

    } else {
	file = url.getUrl();
    }
/*
    if( std::string(file.text()).find( "http://" ) == 0 || 
	std::string(base_dir.text()).find( "http://" ) == 0
	)
    {
	FXString f2;
	FXString f = file;

	if( base_dir.length() && std::string(file.text()).find( "http://" ) != 0 )
	    f = base_dir + file;

	if( loader.getUrl( f, f2 ) )
	{
	    printf( "%s downloaded as %s\n", f.text(), f2.text() );
	    
	    if( std::string(file.text()).find( "http://" ) == 0 )
	    {
		if( file[file.length()-1] == '/' )
		    base_dir = file;
		else
		{
		    std::string path;
		    std::string name;
		    CppDir::split_name( file.text(), path, name );
		    base_dir = path.c_str();		   

		    printf( "base_dir: %s\n", base_dir.text() );		    
		}
	    }
		
	    file = f2;
	    
	    done = true;
	}
    } 
    
    if( std::string(file.text()).find( "ftp://" ) == 0 )
    {
	printf( "not implmented yet %s\n", file.text() );
	return;
    }
    
    if( !done )
    {
	if( file[0] != '/' )
	{
	    if( !base_dir.length() )
	    {
		std::string path;
		std::string name;
		std::string f = CppDir::get_full_path( file.text() );
		
		CppDir::split_name( f, path, name );
		
		base_dir = path.c_str();
		file = f.c_str();
	    } else {
		
		CppDir::File ff( file.text() );
		
		if( ff.is_valid() )
		{
		    std::string path;
		    std::string name;
		    CppDir::split_name( file.text(), path, name );
		    base_dir = path.c_str();
		    
		} else {
		    
		    std::string f = CppDir::concat_dir( base_dir.text(), file.text() );
		    file = f.c_str();
		    
		    std::string path;
		    std::string name;
		    CppDir::split_name( f, path, name );
		    base_dir = path.c_str();
		}
	    }	

	} else  {
	    std::string path;
	    std::string name;
	    
	    CppDir::split_name( file.text(), path, name );
	    base_dir = path.c_str();
	}
    }
*/

    Ref<Node> node = parse_file( file );

    if( node.valid() )
    {
	deleteImages();
	last_objects.clear();
	html_root = node;
//	dump_tree( html_root );    

	if( root.valid() )
	    root->endLinkCursor();

	root = build_object_tree( html_root, this );    
	root->layout();   
	onPaint( 0, 0, 0 );
	getParent()->recalc();
    }
}

FXint FXHTML::getDefaultWidth()
{
    return width;
}

FXint FXHTML::getDefaultHeight()
{
    return height;
}


Ref<HTMLObject> FXHTML::getDOM()
{
    return root;
}

void FXHTML::doUpdate()
{
    root->layout();
    recalc();
    update();
}


void FXHTML::loadImage( const FXString &file )
{
    FXString f;
/*
    if(  std::string(file.text()).find( "http://" ) == 0  )
	f = file;
    else
	f = base_dir + file;
*/
    FXURL2 url( file, base_url );
    f = url.getUrl();

    printf( "loadImage %s\n", f.text() );

    if( url.getProtocol() != FXURL2::PROTOCOL_LOCAL_FILE )
	loader.loadUrl( f );
}

void FXHTML::getImage( const FXString &mfile, Ref<Image> &image )
{
    if( !image )
	return;

    FXString file = image->getSrc();

    FXString f;
/*
    if(  std::string(file.text()).find( "http://" ) == 0  )
	f = file;
    else
	f = base_dir + file;
*/
    FXURL2 url( file, base_url );
    f = url.getUrl();

    if( url.getProtocol() != FXURL2::PROTOCOL_LOCAL_FILE )    
    {
	if( !loader.downloadedUrl( url.getUrl() ) )
	{
	    bool found = false;

	    for( unsigned i = 0; i < images.size(); ++i )
		if( images[i].address() == image.address() )
		{
		    found = true;
		    break;
		}

	    if( !found )
		images.push_back( image );

	    return;
	}

	loader.getUrl( url.getUrl(), file );
    }
    else
	file = url.getUrl();

    printf( "getImage: %s\n", f.text() );

/*
    if( std::string(file.text()).find( "http://" ) == 0  )	
	loader.getUrl( f, file );
*/
    ImageSize is( file.text() ); // we do not trust filenames

/*
    FXString ext = FXFile::extension( f );
    ext.lower();

    if( ext == "gif" )
	image->setImage( new FXGIFImage( getApp(), NULL, IMAGE_KEEP | IMAGE_SHMI | IMAGE_SHMP | IMAGE_ALPHAGUESS ) );
    else if( ext == "bmp" )
	image->setImage( new FXBMPImage( getApp(), NULL, IMAGE_KEEP | IMAGE_SHMI | IMAGE_SHMP ) );
    else if( ext == "bmp" )
	image->setImage( new FXBMPImage( getApp(), NULL, IMAGE_KEEP | IMAGE_SHMI | IMAGE_SHMP ) );
    else if( ext == "xpm" )
	image->setImage( new FXXPMImage( getApp(), NULL, IMAGE_KEEP | IMAGE_SHMI | IMAGE_SHMP | IMAGE_ALPHAGUESS ) );
    else if( ext == "pcx" )
	image->setImage( new FXPCXImage( getApp(), NULL, IMAGE_KEEP | IMAGE_SHMI | IMAGE_SHMP ) );
    else if( ext == "ico" )
	image->setImage( new FXICOImage( getApp(), NULL, IMAGE_KEEP | IMAGE_SHMI | IMAGE_SHMP | IMAGE_ALPHAGUESS ) );
    else if( ext == "tga" )
	image->setImage( new FXTGAImage( getApp(), NULL, IMAGE_KEEP | IMAGE_SHMI | IMAGE_SHMP ) );
    else if( ext == "rgb" )
	image->setImage( new FXRGBImage( getApp(), NULL, IMAGE_KEEP | IMAGE_SHMI | IMAGE_SHMP ) );
    else if( ext == "png" )
	image->setImage( new FXPNGImage( getApp(), NULL, IMAGE_KEEP | IMAGE_SHMI | IMAGE_SHMP | IMAGE_ALPHAGUESS ) );
    else if( ext == "jpg" )
	image->setImage( new FXJPGImage( getApp(), NULL, IMAGE_KEEP | IMAGE_SHMI | IMAGE_SHMP ) );
    else if( ext == "tif" )
	image->setImage( new FXTIFImage( getApp(), NULL, IMAGE_KEEP | IMAGE_SHMI | IMAGE_SHMP ) );
*/
    if( !is.is_valid() )
    {
	printf( "invalid image: %s %d\n", image->getSrc().text(), is.get_error() );
	return;
    }

    switch( is.get_type() )
    {
	case ImageSize::GIF:
	    image->setImage( new FXGIFImage( getApp(), NULL, IMAGE_KEEP | IMAGE_SHMI | IMAGE_SHMP | IMAGE_ALPHAGUESS ) );
	    break;

	case ImageSize::BMP:
	    image->setImage( new FXBMPImage( getApp(), NULL, IMAGE_KEEP | IMAGE_SHMI | IMAGE_SHMP | IMAGE_ALPHAGUESS ) );
	    break;

	case ImageSize::JPEG:
	    image->setImage( new FXJPGImage( getApp(), NULL, IMAGE_KEEP | IMAGE_SHMI | IMAGE_SHMP | IMAGE_ALPHAGUESS ) );
	    break;

	case ImageSize::PNG:
	    image->setImage( new FXPNGImage( getApp(), NULL, IMAGE_KEEP | IMAGE_SHMI | IMAGE_SHMP | IMAGE_ALPHAGUESS ) );
	    break;

	case ImageSize::UNKNOWN:
	    break;
    }

    if( !image->getImage() )
	return;

    FXFileStream stream;

    if( stream.open( file, FXStreamLoad ) )
    {
	image->getImage()->loadPixels( stream );
	stream.close();
	image->getImage()->create();

	if( url.getProtocol() != FXURL2::PROTOCOL_LOCAL_FILE )
	{
	    for( std::vector< Ref<Image> >::iterator it = images.begin(); it != images.end(); ++it )
		if( it->address() == image.address() )
		{
		    (*it)->notify();
		    images.erase( it );
		    doUpdate();
		    break;
		}
	}	    

    } else {
	image->destroy();
    }
}


void FXHTML::deleteImages()
{
    images.clear();
}

void FXHTML::requestLoadingUrl( const FXString &url )
{
    FXHTMLWindow *p = dynamic_cast<FXHTMLWindow*>( getParent() );
    if( p )
    {
	FXURL2 u( url, base_url );
	void *data = (void*)&u.getUrl();
	p->inform( FXHTMLWindow::HTML_LOAD_URL, data );
    }
    
    loadFile( url );
}

void FXHTML::setLinkInfo( const FXString &url )
{
    FXHTMLWindow *p = dynamic_cast<FXHTMLWindow*>( getParent() );
    if( p )
    {
	FXURL2 u( url, base_url );
	
	p->inform( FXHTMLWindow::HTML_LINK_INFO, (void*)&u.getUrl() );
    }    
}

long FXHTML::onCheckImages( FXObject *obj, FXSelector sel, void *p )
{
    for( unsigned i = 0; i < images.size(); ++i )
	getImage( images[i]->getSrc(), images[i] );

    image_timer = getApp()->addTimeout( 500, this, ID_IMAGE_TIMER );    
    return 1;
}
