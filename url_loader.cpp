#include "url_loader.h"
#include "cppdir.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

void mysystem( const FXString & url, const FXString & file )
{
    pid_t pid = fork();

    if( pid > 0 )
    {
	waitpid( pid, NULL, 0 );
    }  else {
	printf( "=================\n" );
	printf( FXString().format( "execl: /usr/bin/wget wget -o %s %s\n", file.text(), url.text() ).text() );
	printf( "=================\n" );
	execl( "/usr/bin/wget", "wget", "-O", file.text(), url.text() ); 
    }
}

URLLoader::URL::~URL()
{
    done();
}

void URLLoader::URL::download()
{
    if( !loader.valid() )
    {
	loader = new Downloader( this );
	loader->start();
    }
}

void URLLoader::URL::done()
{
    state = STATE_READY;
}

void URLLoader::URL::wait()
{
    if( loader.valid() )
	loader->wait();
}

URLLoader::Downloader::Downloader( URL *url )
    : Thread(), url( url )
{

}

void URLLoader::Downloader::run()
{
    url->state = STATE_IN_PROGRESS;
    // system( FXString().format( "wget -O %s %s", url->file.text() , url->url.text() ).text() );
    mysystem( url->url, url->file );
    url->done();
}

URLLoader::URLLoader()
{
    cache_dir = CppDir::get_full_path( "cache" ).c_str();

    CppDir::File file( cache_dir.text() );
    
    if( !file )
	mkdir( cache_dir.text(), 0777 );

    file_count = 1;
}

void URLLoader::loadUrl( const FXString & url )
{
    printf( "load url %s\n", url.text() );

    Ref<URL> u = new URL( url );

    if( !findUrl( u ) )
    {
	u->file = CppDir::concat_dir( cache_dir.text(), FXString().format( "%d.dat", file_count++ ).text() ).c_str();
	u->download();
	urls.push_back( u );    
    }
}

bool URLLoader::getUrl( const FXString &url, FXString & file )
{
    Ref<URL> u = new URL( url );

    if( findUrl( u ) )
    {
	if( u->state == STATE_READY )
	{
	    file = u->file;
	    return true;
	} else {
	    u->wait();
	    file = u->file;
	    return true;
	}
    } else {

	loadUrl( url );
	return getUrl( url, file );

    }

    return false;
}

bool URLLoader::findUrl( Ref<URL> & url )
{
    for( unsigned i = 0; i < urls.size(); ++i )
    {
	if( url->url == urls[i]->url )
	{
	    url = urls[i];
	    return true;
	}
    }

    return false;
}

bool URLLoader::downloadedUrl( const FXString &url )
{
    Ref<URL> u = new URL( url );

    if( findUrl( u ) )
	return u->state == STATE_READY;

    return false;
}
