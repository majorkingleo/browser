#include "object_text.h"
#include "string_utils.h"

int ObjectText::TYPE_ID = HTMLObject::object_type_id_count++;

ObjectText::ObjectText( const Ref<HTMLObject> & parent, const FXString & str )
    : HTMLObject( parent ),
      text( decodeEntities(str) ),
      font( 0 )
{
    object_type_id  = TYPE_ID;
    classname = "ObjectText";

    setLayoutHint( LAYOUT_AUTOSPACE );
    setLayoutHint( LAYOUT_FIX_SIZE );    

    color = -1;
}

void ObjectText::layout()
{   
    if( !layouted )    
    {
	layouted = true;

	font = getFont( getStyle( "font-face",     getFont( 0 )->getName() ), 
			string2uint( getStyle( "font-size",     getFont( 0 )->getSize() / 10 ) ) ,
			string2uint( getStyle( "font-weight",   getFont( 0 )->getWeight() ) ),
			string2uint( getStyle( "font-slant",    getFont( 0 )->getSlant() ) ),
			string2uint( getStyle( "font-encoding", getFont( 0 )->getEncoding() ) ),
			string2uint( getStyle( "font-width",    getFont( 0 )->getSetWidth() ) ),
			string2uint( getStyle( "font-hints",    getFont( 0 )->getHints() ) ) ,
			current_font );

	height = font->getFontHeight();
	width = font->getTextWidth( text.text(), text.length() );

	min_height = height;
	min_width = width;    

	color = string2color( getStyle( "color", "#000000" ), -1 );

	if( color == -1 )
	    color = 0;

	decore = string2uint( getStyle( "font-decore", FONT_DECORE_NONE ), FONT_DECORE_NONE );

	nolayout = true; // do not call layout again
    }
}

void ObjectText::paint()
{
    if( markup )
    {
	dc.setColor( 0 );
	dc.drawRectangle( 0, 0, width, height );
    }

    dc.setColor( color );

    dc.setFont( font );

    dc.drawText( 0, font->getFontAscent(), text );

    if( decore == FONT_DECORE_UNDERLINE )
	dc.drawLine( 0, font->getFontAscent() + 1, width, font->getFontAscent() + 1 );
}

void ObjectText::setWidth( int w )
{
    printf( "setWidth on ObjectText %s\n", text.text() );
}

void ObjectText::setHeight( int h )
{
    printf( "setHeight on ObjectText %s\n", text.text() );
}
