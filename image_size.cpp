#include "image_size.h"
#include <cctype>

#if __GNUC__ == 2
#define ios_base ios
#endif

using namespace std;

ImageSize::ImageSize( string file_name )
  : valid( false ), 
    in( file_name.c_str(), ios_base::in | ios_base::binary ),
    type( UNKNOWN ),
    file_name( file_name ),
    error( SUCCESS )
{
  if( !in )
    {
      error = INVALID_FILE;
      return;
    }

  if( !get_image_type() )
    return;

  in.seekg(0);

  switch( type )
    {
    case GIF:  get_gif_geometry();  break;
    case JPEG: get_jpeg_geometry(); break;
    case PNG:  get_png_geometry();  break;
    case BMP:  get_bmp_geometry();  break;
    case UNKNOWN: break;
    }
} 

bool ImageSize::compare( string a, string b )
{
  string::size_type size = a.size();
  for( string::size_type i = 0; i < size; ++i )
    a[i] = toupper( a[i] );

  return a == b;
}

bool ImageSize::get_image_type()
{
  string::size_type pos = file_name.rfind( '.' );

  if( pos == string::npos || pos == file_name.size() - 1)
    {
      error = INVALID_FILE_NAME;
      return false;
    }

  string file = file_name.substr( pos+1 ); // get file extension

  if( compare( file, "GIF" ) )
    {
      type = GIF;
      return true;
    }

  if( compare( file, "PNG" ) )
    {
      type = PNG;
      return true;
    }

  if( compare( file, "JPG" ) || compare( file, "JPEG" ) )
    {
      type = JPEG;
      return true;
    }

  if( compare( file, "BMP" ) )
    {
      type = BMP;
      return true;
    }


  // check the signatures
  in.seekg(0); 
  if( check_gif_signature() )
  {
      type = GIF;
      return true;
  }

  in.seekg(0); 
  if( check_jpeg_signature() )
  {
      type = JPEG;
      return true;
  }

  in.seekg(0); 
  if( check_png_signature() )
  {
      type = PNG;
      return true;
  }

  in.seekg(0); 
  if( check_bmp_signature() )
  {
      type = BMP;
      return true;
  }

  error = INVALID_FILE_TYPE;
  return false;
}

bool ImageSize::get_1_byte( unsigned int& byte )
{
  if( in.eof() )
    return false;

  char c;
  in.get( c );

  byte = static_cast<unsigned char>( c );

  return true;
}

bool ImageSize::get_le_2_bytes( unsigned int& bytes ) 
{
  unsigned int b1, b2;

  if( !get_1_byte( b1 ) || !get_1_byte( b2 ) )
    return false;

#ifndef WORDS_BIGENDIAN

  bytes = (b2 << 8 | b1);

#else

  // big endian
  bytes = (b1 << 8 | b2);

#endif

  return true;
}

bool ImageSize::get_be_2_bytes( unsigned int& bytes )
{
  unsigned int b1, b2;

  if( !get_1_byte( b1 ) || !get_1_byte( b2 ) )
    return false;


#ifndef WORDS_BIGENDIAN

  bytes = (b1 << 8 | b2);

#else

  // big endian
  bytes = (b2 << 8 | b1);

#endif

  return true;
}

bool ImageSize::get_le_4_bytes( unsigned int& bytes )
{
  unsigned int b1, b2;

  if( !get_le_2_bytes( b1 ) || !get_le_2_bytes( b2 ) )
    return false;

  //  cout << b1 << " " << b2 << endl;
  bytes = (b1 << 16 | b2 );

  return true;
}

bool ImageSize::get_be_4_bytes( unsigned int& bytes )
{
  unsigned int b1, b2;

  if( !get_be_2_bytes( b1 ) || !get_be_2_bytes( b2 ) )
    return false;

  bytes = (b1 << 16 | b2 );

  return true;
}

bool ImageSize::jump_abs( unsigned int pos )
{
  in.seekg( pos );
  if( in.eof() )
    return false;

  return true;
}

bool ImageSize::jump_rel( unsigned int n )
{
  in.seekg( n, ios_base::cur );
  if( in.eof() )
    return false;

  return true;
}

bool ImageSize::get_string( string& s, unsigned int n )
{
  s.resize( n );
  for( unsigned int i=0; i < n; ++i )
    {
      unsigned int b;
      if( !get_1_byte( b ) )
	return false;
      
      s[i] = static_cast<char>( b );
    }

  return true;
}

void ImageSize::get_gif_geometry()
{
  if( !check_gif_signature() )
    return;
  
  if( get_le_2_bytes( width )  && get_le_2_bytes( height ) )
    {
      valid = true;
      return;
    }

  error = BAD_IMAGE;
}

bool ImageSize::check_gif_signature()
{
  string sig;

  if( get_string( sig, 6 ) )
    {
      if( sig == "GIF87a" || sig == "GIF89a" )
	return true;
    }

  error = BAD_HEADER;
  return false;
}

void ImageSize::get_png_geometry()
{
  if( !check_png_signature() )
    return;

  if( jump_abs( 16 ) )
    if( get_be_4_bytes( width ) && get_be_4_bytes( height ) )
      {
	valid = true;  
	return;
      }

  error = BAD_IMAGE;
}

bool ImageSize::check_png_signature()
{
  string sig;

  if( get_string( sig, 8 ) )
    {
      if( sig == "\211PNG\r\n\032\n" )
	return true;
    }

  error = BAD_HEADER;
  return false;
}

void ImageSize::get_jpeg_geometry()
{
  if( !check_jpeg_signature() )
    return;

  if( jpeg_find_sof0_marker() )
    if( jump_rel( 3 ) )
      if( get_be_2_bytes( height ) && get_be_2_bytes( width ) )
	{
	  valid = true;
	  return;
	}
	
  error = BAD_IMAGE;
}

bool ImageSize::check_jpeg_signature()
{
#ifndef WORDS_BIGENDIAN
  const unsigned int M_SOI = 0xFF << 8 | 0xD8;
#else
  const unsigned int M_SOI = 0xD8 << 8 | 0xFF;
#endif

  unsigned int soi;

  if( get_be_2_bytes( soi ) )
    if( soi == M_SOI )
      return true;

  error = BAD_HEADER;
  return false;
}

#ifndef WORDS_BIGENDIAN
#  define MARKER( value ) \
     markers[i] = (0xFF << 8 | value); ++i
#else 
#  define MARKER( value ) \
     markers[i] = (value << 8 | 0xFF); ++i
#endif 

bool ImageSize::jpeg_find_sof0_marker()
{
  unsigned int markers[16];
  unsigned int i=0;

  MARKER( 0xC0 );
  MARKER( 0xC1 );
  MARKER( 0xC2 );
  MARKER( 0xC3 );
  MARKER( 0xC4 );
  MARKER( 0xC5 );
  MARKER( 0xC6 );
  MARKER( 0xC7 );
  MARKER( 0xC9 );
  MARKER( 0xCA );
  MARKER( 0xCB );
  MARKER( 0xCD );
  MARKER( 0xCE );
  MARKER( 0xCF );

  while( true )
    {
      unsigned int marker;

      if( !get_be_2_bytes( marker ) )
	return false;


      for( unsigned int i=0; i < 16; ++i )
	{
	  if( marker == markers[i] )
	    return true;
	}

      // not found jump over the data of the marker

      unsigned int len;
      
      if( !get_be_2_bytes( len ) )
	return false;

      len -= 2;

      if( !jump_rel( len ) )
	return false;
    }
}

#undef MARKER


void ImageSize::get_bmp_geometry()
{
  if( !check_bmp_signature() )
    return;

  // get subtype

  unsigned int subb;

  if( jump_abs( 14 ) )
    if( get_le_2_bytes( subb ) )
      {
	switch( subb )
	  {
	  case 12: // OS/2 1.x format
	  case 64: // OS/2 2.x format
	    if( get_le_2_bytes( width ) && get_le_2_bytes( height ) )
	      {
		valid = true;
		return;
	      }
	    break;
	  case 40: // Windows 3.x format
	    if( get_le_4_bytes( width ) && get_le_4_bytes( height ) )
	      {
		valid = true;
		return;
	      }
	    break;

	  default:
	    break;
	  }
      }

  error = BAD_IMAGE;
}

bool ImageSize::check_bmp_signature()
{
  string sig;

  if( get_string( sig, 2 ) )
    if( sig == "BM" )
      return true;

  error = BAD_HEADER;

  return false;
}
