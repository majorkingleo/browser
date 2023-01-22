#ifndef URL_LOADER_H
#define URL_LOADER_H

#include <fox/fx.h>
#include "thread.h"
#include "ref.h"
#include <vector>

class URLLoader
{
 protected:

    enum {
	STATE_WAITING,
	STATE_IN_PROGRESS,
	STATE_READY
    };

    class URL;

    class Downloader : public Thread
    {
	URL *url;
    public:
	Downloader( URL *url );

	void run();
    };


    struct URL {
	FXString url;
	FXString file;

	int state;

	Ref<Downloader> loader;

	URL( const FXString &url, const FXString & file = "" ) : url( url ), state( STATE_WAITING ) {}
	~URL();

	void download();
	void done();
	void wait();
    };

    std::vector< Ref<URL> > urls;

    FXString cache_dir;

    int file_count;

 public:
    URLLoader();

    void loadUrl( const FXString &url );
    bool getUrl( const FXString &url, FXString & file );
    bool downloadedUrl( const FXString &url );

 protected:
    bool findUrl( Ref<URL> & url );
};

#endif
