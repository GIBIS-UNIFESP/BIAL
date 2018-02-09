/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Aug/08 
 * @brief Writing to BMP gray images. 
 * @warning Very basic support. Only writing to gray images yet.
 */

#include "Common.hpp"

#ifndef BIALFILEBMP_H
#define BIALFILEBMP_H

namespace Bial {

  template< class D >
  class Image;

  /**
   * @date 2013/Dec/20
   * @param img: input image.
   * @param filename: Source file to be written.
   * @return A reference to the input image.
   * @brief Open a BMP file to write 'this' content.
   * @warning none.
   */
  template< class D >
  static void WriteBMP( const Image< D > &img, const std::string &filename );

}

/* Implementation --------------------------------------------------------------------------------------------------- */

#include "Color.hpp"
#include "ColorRGB.hpp"
#include "File.hpp"
#include "Image.hpp"

namespace Bial {

  template< class D >
  void WriteBMP( const Image< D > &img, const std::string &filename ) {
    COMMENT( "Check if image is 2D.", 2 );
    if( img.Dims( ) != 2 ) {
      std::string msg( BIAL_ERROR( " Only 2D BMP files supported." ) );
      throw( std::logic_error( msg ) );
    }
    try {
      COMMENT( "Opening file.", 2 );
      OFile file;
      file.exceptions( std::fstream::failbit | std::fstream::badbit );
      file.open( filename );
      COMMENT( "Writing magic word.", 2 );
      std::string hdr_char( "BM" );
      file.write( reinterpret_cast< char* >( &hdr_char[ 0 ] ), 2 );
      COMMENT( "Writing file size.", 2 );
      size_t dw_var = 18 + 40 + 1024 + img.size( ); /* filesize in bytes */
      file.write( reinterpret_cast< char* >( &dw_var ), 4 );
      COMMENT( "Writing zeros.", 2 );
      size_t zero_padding = 0;
      file.write( reinterpret_cast< char* >( &zero_padding ), 4 );
      COMMENT( "Writing byte offset to the bitmapdata.", 2 );
      dw_var = 1077; /* byte offset to the bitmapdata */
      file.write( reinterpret_cast< char* >( &dw_var ), 4 );
      COMMENT( "Info header.", 2 );
      dw_var = 40;
      file.write( reinterpret_cast< char* >( &dw_var ), 4 );
      COMMENT( "Writing dimensions.", 2 );
      long dimension = img.size( 0 );
      file.write( reinterpret_cast< char* >( &dimension ), 4 );
      dimension = img.size( 1 );
      file.write( reinterpret_cast< char* >( &dimension ), 4 );
      COMMENT( "Writing data length and other data.", 2 );
      unsigned short word_var = 1;
      file.write( reinterpret_cast< char* >( &word_var ), 2 );
      word_var = 8;
      file.write( reinterpret_cast< char* >( &word_var ), 2 );
      dw_var = 0;
      file.write( reinterpret_cast< char* >( &dw_var ), 4 );
      dw_var = img.size( 0 ) * img.size( 1 );
      file.write( reinterpret_cast< char* >( &dw_var ), 4 );
      file.write( reinterpret_cast< char* >( &zero_padding ), 4 );
      file.write( reinterpret_cast< char* >( &zero_padding ), 4 );
      file.write( reinterpret_cast< char* >( &zero_padding ), 4 );
      file.write( reinterpret_cast< char* >( &zero_padding ), 4 );
      COMMENT( "colortable (1024 bytes = 4 x 256 x char).", 2 );
      for( size_t chl = 0; chl < 256; ++chl ) {
        char color = chl;
        file.write( &color, 1 );
        file.write( &color, 1 );
        file.write( &color, 1 );
        file.write( &color, 1 );
      }
      COMMENT( "Writing data.", 2 );
      if( static_cast< llint >( img.Maximum( ) ) > 255 ) {
        Image< D > copy_img( img );
        copy_img.SetRange( static_cast< D >( 0 ), static_cast< D >( 255 ) );
        Image< int > range_img( copy_img );
        for( int row = img.size( 1 ) - 1; row >= 0; --row ) {
          for( size_t col = 0; col < img.size( 0 ); ++col ) {
            file.write( reinterpret_cast< char* >( &range_img( col, row ) ), 1 );
          }
        }
      }
      else {
        for( int row = img.size( 1 ) - 1; row >= 0; --row ) {
          for( size_t col = 0; col < img.size( 0 ); ++col ) {
            file.write( reinterpret_cast< const char* >( &img( col, row ) ), 1 );
          }
        }
      }
      file.close( );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error opening/writing/closing BMP file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

  template< >
  void WriteBMP( const Image< Color > &img, const std::string &filename ) {
    COMMENT( "Check if image is 2D.", 2 );
    if( img.Dims( ) != 2 ) {
      std::string msg( BIAL_ERROR( " Only 2D BMP files supported." ) );
      throw( std::logic_error( msg ) );
    }
    try {
      BIAL_WARNING( "BMP only supported for grayscale images. Converting color to gray." );
      Image< int > gray_img( ColorSpace::ARGBtoGraybyBrightness< int >( img ) );
      WriteBMP( gray_img, filename );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error opening/writing/closing BMP file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

}

#endif
