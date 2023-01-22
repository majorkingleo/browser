#ifndef image_size_h
#define image_size_h

#include <string>
#include <fstream>

// class for getting the geometry of an image
class ImageSize
{
public:
  
  enum Type
    {
      UNKNOWN  = -1,
      GIF      =  1,
      JPEG     =  2,
      PNG      =  3,
      BMP      =  4
    };

  /// if the image is not valid these errors are saying why
  enum Error
    {
      SUCCESS           =0, ///< no Error
      INVALID_FILE      =1, ///< couldn't open the file
      INVALID_FILE_NAME =2, ///< eg.: file name without an extension
      INVALID_FILE_TYPE =3, ///< unsuported file type
      BAD_HEADER        =4, ///< probably damaged image header
      BAD_IMAGE         =5  ///< probably damaged image
    };

private:
  bool valid;

  unsigned int width;
  unsigned int height;

  std::ifstream in;

  Type type;

  std::string file_name;

  Error error;

public:  
  ImageSize( std::string file_name );

  bool operator!() const { return !valid; }
  bool is_valid() const { return valid; }

  unsigned int get_width() const { return width; }
  unsigned int get_height() const { return height; }

  /// returns the type of the image
  Type get_type() const { return type; } 

  /// more detailed description why an error occured
  Error get_error() const { return error; }

private:
  ImageSize(){}

  /// detects the image type out of the file_name extension
  bool get_image_type();

  /// uncasesesitiv string compare
  /** string a will be converted to upper case, string b won't be touched */
  bool compare( std::string a, std::string b );

  bool get_1_byte( unsigned int& byte );

  bool get_le_2_bytes( unsigned int& bytes ); ///< 2 bytes, littel endian 
  bool get_be_2_bytes( unsigned int& bytes ); ///< 2 bytes, big endian 
  
  bool get_le_4_bytes( unsigned int& bytes ); ///< 4 bytes, littel endian 
  bool get_be_4_bytes( unsigned int& bytes ); ///< 4 bytes, big endian 

  bool jump_abs( unsigned int pos); ///< absolut seeking in file
  bool jump_rel( unsigned int n );  ///< relative seeking in file

  /// reads n bytes from the file and stores them as a string in s
  bool get_string( std::string& s, unsigned int n );

  // gif support
  void get_gif_geometry();
  bool check_gif_signature();

  // jpeg support
  void get_jpeg_geometry();
  bool check_jpeg_signature();
  bool jpeg_find_sof0_marker();

  // png support
  void get_png_geometry();
  bool check_png_signature();

  // bmp support
  void get_bmp_geometry();
  bool check_bmp_signature();

};

#endif
