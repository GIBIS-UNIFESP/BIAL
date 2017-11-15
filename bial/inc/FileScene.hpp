/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Aug/08 
 * @brief Reading and writing scene type images.
 */

#include "Common.hpp"

#ifndef BIALFILESCENE_H
#define BIALFILESCENE_H

namespace Bial {

  template< class D >
  class Image;

  /**
   * @date 2012/Jul/02
   * @param filename: Source filename to be readed.
   * @return A reference to the created scene.
   * @brief Open a Scene file for reading and returns it.
   * @warning none.
   */
  template< class D >
  static Image< D > ReadScene( const std::string &filename );

  /**
   * @date 2012/Jul/03
   * @param img: input image.
   * @param filename: Source file to be written.
   * @return A reference to the input image.
   * @brief Open a Scene file to write 'this' content.
   * @warning none.
   */
  template< class D >
  static void WriteScene( const Image< D > &img, const std::string &filename );
}

/* Implementation --------------------------------------------------------------------------------------------------- */

#include "Color.hpp"
#include "ColorRGB.hpp"
#include "File.hpp"
#include "Image.hpp"

namespace Bial {

  template< class D >
  Image< D > ReadScene( const std::string &filename ) {
    try {
      COMMENT( "Opening scene file.", 2 );
      IFile file;
      file.exceptions( std::fstream::failbit | std::fstream::badbit );
      file.open( filename );

      COMMENT( "Checking magic word.", 2 );
      std::string scn_type;
      getline( file, scn_type );
      if( scn_type.compare( "SCN" ) == 0 ) {
        COMMENT( "Reading dimension size.", 2 );
        Vector< size_t > size( 3 );
        file >> size[ 0 ] >> size[ 1 ] >> size[ 2 ];
        COMMENT( "Reading pixel dimension size.", 2 );
        Vector< float > pixdim( 3 );
        file >> pixdim[ 0 ] >> pixdim[ 1 ] >> pixdim[ 2 ];
        COMMENT( "Creating image.", 2 );
        Image< D > res( size, pixdim );
        COMMENT( "Getting data type.", 2 );
        unsigned int type;
        file >> type;
        file.ignore( 1 ); /* Ignores an enter. */

        COMMENT( "Getting number of pixels.", 2 );
        size_t img_size = res.size( );
        if( type == 8 ) {
          COMMENT( "Reading 8 bit integer data.", 2 );
          unsigned char *data8;
          data8 = new unsigned char[ img_size ];
          file.read( reinterpret_cast< char* >( data8 ), sizeof( unsigned char ) * img_size );
          for( size_t pxl = 0; pxl < img_size; ++pxl ) {
            res( pxl ) = static_cast< D >( data8[ pxl ] );
          }
          delete[] data8;
        }
        else if( type == 16 ) {
          COMMENT( "Reading 16 bit integer data.", 2 );
          unsigned short *data16;
          data16 = new unsigned short[ img_size ];
          file.read( reinterpret_cast< char* >( data16 ), sizeof( unsigned short ) * img_size );
          for( size_t pxl = 0; pxl < img_size; ++pxl ) {
            res( pxl ) = static_cast< D >( data16[ pxl ] );
          }
          delete[] data16;
        }
        else if( type == 32 ) {
          COMMENT( "Reading 32 bit integer data.", 2 );
          int *data32;
          data32 = new int[ img_size ];
          file.read( reinterpret_cast< char* >( data32 ), sizeof( int ) * img_size );
          for( size_t pxl = 0; pxl < img_size; ++pxl ) {
            res( pxl ) = static_cast< D >( data32[ pxl ] );
          }
          delete[] data32;
        }
        else { /* if( type == 64 ) */
          COMMENT( "Reading 32 bit float data.", 2 );
          float *fdata32;
          fdata32 = new float[ img_size ];
          file.read( reinterpret_cast< char* >( fdata32 ), sizeof( float ) * img_size );
          for( size_t pxl = 0; pxl < img_size; ++pxl ) {
            res( pxl ) = static_cast< D >( fdata32[ pxl ] );
          }
          delete[] fdata32;
        }
        file.close( );
        return( res );
      }
      COMMENT( "It is not a scene type image.", 2 );
      std::string msg( BIAL_ERROR( "Scene file not found, unsupported, or currupted. Expected extensions: " +
                                   ".scn or .scn.gz" ) );
      throw( std::logic_error( msg ) );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error opening/reading/closing Scene file." ) );
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
  Image< Color > ReadScene( const std::string &filename ) {
    try {
      COMMENT( "Reading to Image< int >.", 2 );
      Image< int > gray_img( ReadScene< int >( filename ) );
      COMMENT( "Converting to RGB.", 2 );
      return( ColorSpace::GraytoARGB( gray_img ) );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error opening/reading/closing Scene file." ) );
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

  template< class D >
  void WriteScene( const Image< D > &img, const std::string &filename ) {
    try {
      size_t img_dms = img.Dims( );
      COMMENT( "Insert here image type control.", 2 );
      if( img_dms != 3 ) {
        std::string msg( BIAL_ERROR( "Invalid number of dimensions. Expected 3." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Opening file.", 2 );
      OFile file;
      file.exceptions( std::fstream::failbit | std::fstream::badbit );
      file.open( filename );
      COMMENT( "Writing magic word and image dimensions.", 2 );
      file << "SCN" << std::endl;
      file << img.size( 0 ) << " " << img.size( 1 ) << " " << img.size( 2 ) << std::endl;
      file << img.PixelSize( 0 ) << " " << img.PixelSize( 1 ) << " " << img.PixelSize( 2 ) << std::endl;
      COMMENT( "Getting maximum intensity.", 2 );
      D Imax = img.Maximum( );

      COMMENT( "Writing data.", 2 );
      if( static_cast< D >( 1.1 ) != static_cast< D >( 1.0 ) ) {
        COMMENT( "32 bit float point image.", 2 );
        Vector< float > fdata32( img.size( ) );
        file << "64" << std::endl;
        for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {
          fdata32[ pxl ] = static_cast< float >( img( pxl ) );
        }
        file.write( reinterpret_cast< char* >( &fdata32[ 0 ] ), sizeof( float ) * img.size( ) );
      }
      else if( Imax < 256 ) {
        COMMENT( "8 bit integer point image.", 2 );
        Vector< unsigned char > data8( img.size( ) );
        file << "8" << std::endl;
        for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {
          data8[ pxl ] = static_cast< unsigned char >( img( pxl ) );
        }
        file.write( reinterpret_cast< char* >( &data8[ 0 ] ), sizeof( unsigned char ) * img.size( ) );
      }
      else if( Imax < 65536 ) {
        COMMENT( "16 bit integer point image.", 2 );
        Vector< unsigned short > data16( img.size( ) );
        file << "16" << std::endl;
        for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {
          data16[ pxl ] = static_cast< unsigned short >( img( pxl ) );
        }
        file.write( reinterpret_cast< char* >( &data16[ 0 ] ), sizeof( unsigned short ) * img.size( ) );
      }
      else {
        COMMENT( "32 bit integer point image.", 2 );
        Vector< int > data32( img.size( ) );
        file << "32" << std::endl;
        for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {
          data32[ pxl ] = static_cast< int >( img( pxl ) );
        }
        file.write( reinterpret_cast< char* >( &data32[ 0 ] ), sizeof( int ) * img.size( ) );
      }
      file.close( );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error opening/writing/closing Scene file." ) );
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
  void WriteScene( const Image< Color > &img, const std::string &filename ) {
    try {
      size_t img_dms = img.Dims( );
      COMMENT( "Insert here image type control.", 2 );
      if( img_dms != 3 ) {
        std::string msg( BIAL_ERROR( "Invalid number of dimensions. Expected 3." ) );
        throw( std::logic_error( msg ) );
      }
      Image< int > gray_img( ColorSpace::ARGBtoGraybyBrightness< int >( img ) );
      WriteScene( gray_img, filename );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error opening/writing/closing Scene file." ) );
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
