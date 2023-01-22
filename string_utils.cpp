#include "string_utils.h"
#include "html_object.h"

#include <string>

int string2color( FXString color, int def )
{
    if( !color.length() )
	return def;

    if( color[0] == '#' )
    {
	int ret = 0;
	if( color.scan( "#%x", &ret ) == 0 )
	    return def;


	// support #RGB
	if( color.length() == 4 )
	{
	    int r = ret & 0xf00; r = r  >> 8;
	    int g = ret & 0x0f0; g = g  >> 4;
	    int b = ret & 0x00f; 

	    r = ( r << 4 ) | r;
	    g = ( g << 4 ) | g;
	    b = ( b << 4 ) | b;
	    
	    return ( r << 16 ) | ( g << 8 ) | b;
	}

	return ret;
    }

    color.lower();
    
    if( color == "aqua" )
	return 0x00ffff;
    if( color == "black" )
	return 0x000000;
    if( color == "blue" )
	return 0x0000ff;
    if( color == "fuchsia" )
	return 0xff00ff;
    if( color == "gray" )
	return 0x808080;
    if( color == "green" )
	return 0x008000;
    if( color == "lime" )
	return 0x00ff00;
    if( color == "maroon" )
	return 0x800000;
    if( color == "navy" )
	return 0x000080;
    if( color == "olive" )
	return 0x808000;
    if( color == "purple" )
	return 0x800080;
    if( color == "red" )
	return 0xff0000;
    if( color == "silver" )
	return 0xc0c0c0;
    if( color == "teal" )
	return 0x008080;
    if( color == "white" )
	return 0xffffff;
    if( color == "yellow" )
	return 0xffff00;

    return def;
}

int string2uint( const FXString & num , int def )
{
    int ret = 0;
    if( num.scan( "%d", &ret ) == 0 )
	return def;

    if( ret < 0 )
	return def;

    return ret;
}

#define CASE( x ) case HTMLObject::x: return #x

FXString layouthint2string( int layouthint )
{
    switch( layouthint )
    {
	CASE( LAYOUT_NO_HINT );
	CASE( LAYOUT_FORCE_LNBR_AFTER );
	CASE( LAYOUT_FORCE_LNBR_BEFORE );
	CASE( LAYOUT_AUTOSPACE );
	CASE( LAYOUT_FIX_SIZE );
	CASE( LAYOUT_MANAGER );
	CASE( LAYOUT_ADOPT_MY_CHILDS );
	CASE( LAYOUT_DEAMONIZE );
	CASE( LAYOUT_WIDTH_PERCENTAGE );
	CASE( LAYOUT_HEIGHT_PERCENTAGE );
	CASE( LAYOUT_LAST );
    }

    return FXString().format( "UNKNOWN: %d", layouthint );
}

#undef CASE

FXString layouthint2string( Ref<HTMLObject> obj )
{
    FXString ret;

    for( unsigned i = 1; i < HTMLObject::LAYOUT_LAST; ++i )
	if( obj->getLayoutHint( i ) )
	    ret += layouthint2string( i ) + ", ";

    return ret;
}

FXString decodeEntities( const FXString & text )
{
    std::string s( text.text() );
    std::string ret;
    
    unsigned pos = 0, pos2 = 0, start = 0;

    while( (pos = s.find( '&', pos ) ) != std::string::npos )
    {
	if( ( pos2 = s.find( ';', pos ) ) != std::string::npos )
	{
	    std::string s2 = s.substr( pos, pos2 - pos + 1);
	    std::string r;
	    
	    if( s2 == "&lt;" ) r = '<';
	    if( s2 == "&gt;" ) r = '>';
	    if( s2 == "&nbsp;" ) r = ' ';
	    // fixme other entities
	    
	    if( !r.empty() )
	    {
		ret += s.substr( start, pos - start );
		ret += r;
		start = pos2 + 1;
		pos = pos2;

	    } else {
		ret += s.substr( start, pos2 );
		start = pos2 + 1;
		pos = pos2;
	    }
	} else {
	    pos = std::string::npos;
	    break;
	}
    }

    if( pos == std::string::npos )
    {
	ret += s.substr( start );
    }

    return ret.c_str();
}
