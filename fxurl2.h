#ifndef FXURL2_H
#define FXURL2_H

#include <fox/fx.h>
#include "ref.h"

class FXURL2
{
 public:

    enum {	
	URL_ABSOLUTE = 0,
	URL_RELATIVE
    };

    enum {	
	PROTOCOL_HTTP = 0,
	PROTOCOL_FTP,
	PROTOCOL_LOCAL_FILE
    };

 protected:
    FXString url;
    FXString base;
    FXString hostname;

    int protocol;
    int type;

    Ref<FXURL2> old_base;

 public:

    FXURL2();
    FXURL2( const FXURL2 &url );
    FXURL2( const FXString & url );
    FXURL2( const FXString & url, const FXURL2 & base );
    ~FXURL2();

    const FXString & getUrl() { return url; }
    const FXString & getBase() { return base; }
    const FXString & getHostname() { return hostname; }
    
    int getProtocol() { return protocol; }
    int getType() { return type; }

    FXURL2 getBaseUrl(); 

    bool empty() const { return !url.length(); }

 protected:
    
    
};

#endif
