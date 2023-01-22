#include "fxurl2.h"
#include <string>

FXURL2::FXURL2()
{
    protocol = PROTOCOL_LOCAL_FILE;
    type = URL_RELATIVE;
}

FXURL2::FXURL2( const FXURL2 &url )
    : url( url.url ),
      base( url.base ),
      hostname( url.hostname ),
      protocol( url.protocol ),
      type( url.type ),
      old_base( url.old_base )
{

}

FXURL2::FXURL2( const FXString & url )
    : url( url )
{
    FXString u = url;
    u.lower();

    std::string su( u.text(), u.length() );

    if( su.find( "http://" ) == 0 )
    {
	protocol = PROTOCOL_HTTP;
	type = URL_ABSOLUTE;	
    }
    else if( su.find( "ftp://" ) == 0 )
    {
	protocol = PROTOCOL_FTP;
	type = URL_ABSOLUTE;
    }
    else
    {
	protocol = PROTOCOL_LOCAL_FILE;
	
	if( url[0] == '/' )
	    type = URL_ABSOLUTE;
	else
 	    type = URL_RELATIVE;
    }

    if( ( protocol == PROTOCOL_HTTP || protocol == PROTOCOL_FTP ) && type == URL_ABSOLUTE)
    {
	// get hostname
	su = std::string( url.text(), url.length() );

	// printf( "su: %s\n", su.c_str() );

	unsigned pos = 0;
	    
	if( protocol == PROTOCOL_HTTP )
	    pos = 7;
	else
	    pos = 6;
	
	unsigned p2 = su.find( '/', pos );

	unsigned length = (p2 == std::string::npos ) ? p2 : p2 - pos;

	std::string erg = su.substr( pos, length );

	hostname = FXString( erg.c_str(), erg.size() );
    }

    // basename
    {
	su = std::string( url.text(), url.length() );

	unsigned pos = su.find( '?' );	

	pos = su.rfind( '/', pos );

	if( protocol == PROTOCOL_HTTP && hostname.length() )
	    if( pos <= 7 + (unsigned) base.length() )
	    {
		base = "http://" + hostname;
	    }
	else if( protocol == PROTOCOL_FTP && hostname.length() )
	    if( pos <= 6 + (unsigned) base.length() )
	    {
		base = "ftp://" + hostname;
	    }
	else if( pos != std::string::npos )
	{
	    std::string erg = su.substr( 0, pos );
	    base = FXString( erg.c_str(), erg.size() );
	}
    }
}

FXURL2::FXURL2( const FXString & surl, const FXURL2 & sbase )
{
    FXURL2 u( surl );

    if( u.type == URL_ABSOLUTE || sbase.empty() )
    {
	url       = u.url;
	type      = u.type;
	protocol  = u.protocol;
	hostname  = u.hostname;
	base      = u.base;
	old_base  = new FXURL2( u );
	return;
    }

    type = URL_RELATIVE;
    protocol = sbase.protocol;
    base = sbase.base;

    if( surl[0] != '/' && base.length() && base[base.length()-1] != '/' )
	url = base + '/' + surl;
    else
	url = base + surl;

    old_base = new FXURL2( sbase );
}

FXURL2::~FXURL2()
{
}

FXURL2 FXURL2::getBaseUrl()
{
    if( type == URL_ABSOLUTE )
	return *this;

    if( old_base.valid() )
	return *(old_base.address());

    return *this;
}
