#include <fox/fx.h>
#include "fxhtml.h"
#include "debug_window.h"

class Main : public FXMainWindow
{
    FXDECLARE( Main );

    FXHTMLWindow *html;

    DebugWindow *dw;

    FXTextField *location;
    FXLabel     *info;
public:

    enum {
	ID_LOCATION = FXMainWindow::ID_LAST,
	ID_HTML,
	ID_LAST
    };

private:
    Main() {}

public:
    Main( FXApp *app );

    void create();
    void loadFile( const FXString &file );

    long onLocation( FXObject *o, FXSelector sel, void *ptr );
    long onHTML( FXObject *o, FXSelector sel, void *ptr );
};

FXDEFMAP( Main ) MainMap[] = {
    FXMAPFUNC( SEL_COMMAND, Main::ID_LOCATION, Main::onLocation ),
    FXMAPFUNC( SEL_COMMAND, Main::ID_HTML, Main::onHTML )
};

FXIMPLEMENT( Main, FXMainWindow, MainMap, ARRAYNUMBER( MainMap ) );

Main::Main( FXApp *app )
    : FXMainWindow( app, "Browser", NULL, NULL, DECOR_ALL, 0, 0, 500, 600 )
{
    FXVerticalFrame *content = new FXVerticalFrame( this, LAYOUT_FILL_X | LAYOUT_FILL_Y );

    location = new FXTextField( content, 20, this, ID_LOCATION, TEXTFIELD_NORMAL | LAYOUT_FILL_X );
   
    html = new FXHTMLWindow( content, this, ID_HTML, LAYOUT_FILL_X | LAYOUT_FILL_Y );    

    info = new FXLabel( content, "", NULL, LAYOUT_FILL_X | JUSTIFY_LEFT );

    dw = new DebugWindow( app, html );
}

void Main::loadFile( const FXString &file )
{
    location->setText( file );
    info->setText( FXString() );
    html->loadFile( file );
}

void Main::create()
{
    FXMainWindow::create();

    show( PLACEMENT_SCREEN );        
    dw->show();
}

long Main::onLocation( FXObject *o, FXSelector sel, void *ptr )
{
    html->loadFile( location->getText() );
    info->setText( FXString() );
    return 1;
}

long Main::onHTML( FXObject *o, FXSelector sel, void *ptr )
{
    FXHTMLWindow::Message *msg = (FXHTMLWindow::Message*)ptr;

    switch( msg->message )
    {
	case FXHTMLWindow::HTML_LOAD_URL: 
	    location->setText( *((FXString*)msg->data) ); 
	    delete (FXString*)msg->data;
	    delete msg;
	    info->setText( FXString() );
	    break;

	case FXHTMLWindow::HTML_LINK_INFO:
	    info->setText( *((FXString*)msg->data ) );
	    delete (FXString*)msg->data;
	    delete msg;
	    break;

    }
    return 1;
}

int main( int argc, char **argv )
{
    FXApp app( "Browser", "kingleo" );

    app.init( argc, argv );

    Main main( &app );

    app.create();

    if( argc >= 2 )
	main.loadFile( argv[1] );

    return app.run();
}
