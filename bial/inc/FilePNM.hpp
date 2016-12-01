/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Aug/08 
 * @brief Reading and writing from PNM, PBM, PGM, and PNM files. 
 */

#include "Common.hpp"

#ifndef BIALFILEPNM_H
#define BIALFILEPNM_H

#include "PNMHeader.hpp"

namespace Bial {

  template< class D >
  class Image;

  /**
   * @date 2013/Jul/05
   * @param filename: Source filename to be readed.
   * @return A reference to the created image.
   * @brief Reads a PBM file and returns its contents.
   * @warning none.
   */
  template< class D >
  static Image< D > ReadPBM( const std::string &filename );

  /**
   * @date 2013/Jul/05
   * @param filename: Source filename to be readed.
   * @return A reference to the created image.
   * @brief Reads a PGM file and returns its contents.
   * @warning none.
   */
  template< class D >
  static Image< D > ReadPGM( const std::string &filename );


  /**
   * @date 2013/Jul/05
   * @param filename: Source filename to be readed.
   * @return A reference to the created image.
   * @brief Reads a PPM file and returns its contents.
   * @warning none.
   */
  template< class D >
  static Image< D > ReadPPM( const std::string &filename );

  /**
   * @date 2013/Jul/05
   * @param filename: Source filename to be readed.
   * @return A reference to the created image.
   * @brief Reads a PNM file and returns its contents.
   * @warning none.
   */
  template< class D >
  static Image< D > ReadPNM( const std::string &filename );

  /**
   * @date 2013/Jul/20
   * @param img: input image.
   * @param filename: Source file to be written.
   * @param hdr: PNM Header.
   * @param binary: true for binary and false for text file.
   * @return A reference to the input image.
   * @brief Open a PBM file to write 'this' content.
   * @warning none.
   */
  template< class D >
  static void WritePBM( const Image< D > &img, const std::string &filename, const PNMHeader &hdr = PNMHeader( ),
                        const bool binary = true );

  /**
   * @date 2013/Jul/20
   * @param img: input image.
   * @param filename: Source file to be written.
   * @param hdr: PNM Header.
   * @param binary: true for binary and false for text file.
   * @return A reference to the input image.
   * @brief Open a PGM file to write 'this' content.
   * @warning none.
   */
  template< class D >
  static void WritePGM( const Image< D > &img, const std::string &filename, const PNMHeader &hdr = PNMHeader( ),
                        const bool binary = false );

  /**
   * @date 2013/Jul/20
   * @param img: input image.
   * @param filename: Source file to be written.
   * @param hdr: PNM Header.
   * @param binary: true for binary and false for text file.
   * @return A reference to the input image.
   * @brief Open a PPM file to write 'this' content.
   * @warning none.
   */
  template< class D >
  static void WritePPM( const Image< D > &img, const std::string &filename, const PNMHeader &hdr = PNMHeader( ),
                        const bool binary = true );

  /**
   * @date 2013/Jul/20
   * @param img: input image.
   * @param filename: Source file to be written.
   * @param hdr: PNM Header.
   * @param binary: true for binary and false for text file.
   * @return none.
   * @brief Open a PNM file to write 'this' content.
   * @warning none.
   */
  template< class D >
  static void WritePNM( const Image< D > &img, const std::string &filename, const PNMHeader &hdr = PNMHeader( ),
                        const bool binary = true );

}

/* Implementation --------------------------------------------------------------------------------------------------- */

#include "Color.hpp"
#include "ColorRGB.hpp"
#include "File.hpp"

namespace Bial {

  template< class D >
  Image< D > ReadPBM( const std::string &filename ) {
    try {
      COMMENT( "Opening file.", 2 );
      IFile file;
      file.exceptions( std::fstream::failbit | std::fstream::badbit );
      file.open( filename );
      COMMENT( "Ignoring comments.", 2 );
      std::string ignore;
      while( ( file.peek( ) == '#' ) || ( file.peek( ) == '\n' ) ) {
        getline( file, ignore );
      }
      COMMENT( "Getting image type.", 2 );
      std::string pbm_type;
      getline( file, pbm_type );
      COMMENT( "Ignoring more comments.", 2 );
      while( ( file.peek( ) == '#' ) || ( file.peek( ) == '\n' ) ) {
        getline( file, ignore );
      }
      COMMENT( "Reading image size.", 2 );
      size_t xsize, ysize;
      file >> xsize >> ysize;
      COMMENT( "Creating image.", 2 );
      Image< D > res( xsize, ysize );

      COMMENT( "Comparing magic word to PBM.", 2 );
      if( pbm_type.find( "P1" ) != std::string::npos ) {
        COMMENT( "Reading text file data.", 2 );
        size_t pxl = 0;
        while( ( pxl < res.size( ) ) && ( !file.eof( ) ) ) {
          char byte = file.get( );
          COMMENT( "Converting char to number.", 4 );
          if( ( byte != ' ' ) && ( byte != '\n' ) && ( byte != '\r' ) ) {
            if( ( byte == '0' ) || ( byte == '1' ) ) {
              res( pxl ) = static_cast< D >( !( byte - 48 ) );
              ++pxl;
            }
            else {
              std::string msg( BIAL_ERROR( "Corrupted pbm file." ) );
              throw( std::logic_error( msg ) );
            }
          }
        }
        file.close( );
        return( res );
      }
      else if( pbm_type.find( "P4" ) != std::string::npos ) {
        COMMENT( "Ignoring byte.", 2 );
        getline( file, ignore );
        COMMENT( "Reading binary file data.", 2 );
        unsigned char ubyte = 0;
        for( size_t y = 0; y < ysize; ++y ) {
          for( size_t x = 0; x < xsize; ++x ) {
            COMMENT( "reading 8 bits.", 4 );
            if( x % 8 == 0 ) {
              char byte;
              file.read( &byte, 1 );
              ubyte = static_cast< unsigned char >( byte );
            }
            COMMENT( "Converting bits to numbers.", 4 );
            unsigned int val = 7 - ( x % 8 );
            if( ( ( ubyte >> val ) % 2 == 0 ) || ( ( ubyte >> val ) % 2 == 1 ) ) {
              res( x, y ) = !static_cast< D >( ( ubyte >> val ) % 2 );
            }
            else {
              std::string msg( BIAL_ERROR( "Corrupted pbm file." ) );
              throw( std::logic_error( msg ) );
            }
          }
        }
        file.close( );
        return( res );
      }
      COMMENT( "Not P1 nor P4 -> Invalid type.", 2 );
      std::string msg( BIAL_ERROR( "Corrupted pbm file." ) );
      throw( std::logic_error( msg ) );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error opening/reading/closing PBM file." ) );
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
  Image< Color > ReadPBM( const std::string &filename ) {
    try {
      COMMENT( "Reading to Image< int >.", 2 );
      Image< int > gray_img( ReadPBM< int >( filename ) );

      COMMENT( "Converting to RGB.", 2 );
      return( ColorSpace::GraytoARGB( gray_img ) );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error opening/reading/closing PBM file." ) );
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
  Image< D > ReadPGM( const std::string &filename ) {
    try {
      COMMENT( "Opening file.", 2 );
      IFile file;
      file.exceptions( std::fstream::failbit | std::fstream::badbit );
      file.open( filename );
      COMMENT( "Ignoring comments.", 2 );
      std::string ignore;
      while( ( file.peek( ) == '#' ) || ( file.peek( ) == '\n' ) ) {
        getline( file, ignore );
      }
      COMMENT( "Getting image type.", 2 );
      std::string pgm_type;
      getline( file, pgm_type );
      COMMENT( "Image of type: " << pgm_type, 2 );
      COMMENT( "Ignoring comments.", 2 );
      while( ( file.peek( ) == '#' ) || ( file.peek( ) == '\n' ) ) {
        getline( file, ignore );
      }
      COMMENT( "Reading image dimensions.", 2 );
      size_t xsize, ysize;
      file >> xsize >> ysize;
      COMMENT( "Image dimensions: " << xsize << "," << ysize, 2 );
      COMMENT( "Ignoring more comments.", 2 );
      while( ( file.peek( ) == '#' ) || ( file.peek( ) == '\n' ) ) {
        getline( file, ignore );
      }
      COMMENT( "Reading maximum intensity.", 2 );
      int maxval;
      file >> maxval;
      COMMENT( "Maximum intensity: " << maxval, 2 );
      COMMENT( "Creating resultant image.", 2 );
      Image< D > res( xsize, ysize );
      COMMENT( "Computing image size.", 2 );
      size_t img_size = res.size( );
      COMMENT( "Image size: " << img_size, 2 );
      COMMENT( "Comparing the type of  the .pgm file.", 2 );

      if( pgm_type.find( "P2" ) != std::string::npos ) {
        COMMENT( "reading text file data.", 2 );
        for( size_t pxl = 0; ( pxl < img_size ) && ( !file.eof( ) ); ++pxl ) {
          int val;
          file >> val;
          if( val <= maxval ) {
            res( pxl ) = static_cast< D >( val );
          }
          else {
            std::string msg( BIAL_ERROR( " Corrupted pgm file." ) );
            throw( std::logic_error( msg ) );
          }
        }
        file.close( );
        DEBUG_WRITE( res, "res", 4 );
        return( res );
      }
      else if( pgm_type.find( "P5" ) != std::string::npos ) {
        COMMENT( "Ignoring byte.", 2 );
        getline( file, ignore );
        COMMENT( "Reading 8 bit binary file data.", 2 );
        if( maxval < 256 ) {
          COMMENT( "Read 8 bit integer contents of binary file.", 2 );
          Vector< char > data8( img_size );
          file.read( reinterpret_cast< char* >( &( data8[ 0 ] ) ), img_size );
          COMMENT( "Copying to image.", 2 );
          for( size_t pxl = 0; pxl < img_size; ++pxl ) {
            if( data8[ pxl ] <= maxval ) {
              res( pxl ) = static_cast< D >( data8[ pxl ] );
            }
            else {
              std::string msg( BIAL_ERROR( "Corrupted pgm file." ) );
              throw( std::logic_error( msg ) );
            }
          }
        }
        else if( maxval < 65536 ) {
          COMMENT( "Read 16 bit integer contents of binary file.", 2 );
          Vector< short > data16( img_size );
          file.read( reinterpret_cast< char* >( &( data16[ 0 ] ) ), img_size * 2 );
          COMMENT( "Copying to image.", 2 );
          for( size_t pxl = 0; pxl < img_size; ++pxl ) {
            if( data16[ pxl ] <= maxval ) {
              res( pxl ) = static_cast< D >( data16[ pxl ] );
            }
            else {
              std::string msg( BIAL_ERROR( "Corrupted pgm file." ) );
              throw( std::logic_error( msg ) );
            }
          }
        }
        else { /* if( maxval < 4294967296 ) { */
          COMMENT( "Read 32 bit integer contents of binary file.", 2 );
          Vector< int > data32( img_size );
          file.read( reinterpret_cast< char* >( &( data32[ 0 ] ) ), img_size * 4 );
          COMMENT( "Copying to image.", 2 );
          for( size_t pxl = 0; pxl < img_size; ++pxl ) {
            if( data32[ pxl ] <= maxval ) {
              res( pxl ) = static_cast< D >( data32[ pxl ] );
            }
            else {
              std::string msg( BIAL_ERROR( "Corrupted pgm file." ) );
              throw( std::logic_error( msg ) );
            }
          }
        }
        file.close( );
        return( res );
      }
      COMMENT( "Not P2 nor P5 -> Invalid type.", 2 );
      std::string msg( BIAL_ERROR( "Corrupted pgm file." ) );
      throw( std::logic_error( msg ) );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error opening/reading/closing PGM file." ) );
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
  Image< Color > ReadPGM( const std::string &filename ) {
    try {
      COMMENT( "Reading to Image< int >.", 2 );
      Image< int > gray_img( ReadPGM< int >( filename ) );

      COMMENT( "Converting to RGB.", 2 );
      return( ColorSpace::GraytoARGB( gray_img ) );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error opening/reading/closing PGM file." ) );
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
  Image< D > ReadPPM( const std::string &filename ) {
    try {
      COMMENT( "Reading to Image< Color >.", 2 );
      Image< Color > color_img( ReadPPM< Color >( filename ) );

      COMMENT( "Converting to RGB.", 2 );
      return( ColorSpace::ARGBtoGraybyBrightness< D >( color_img ) );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error opening/reading/closing PPM file." ) );
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
  Image< Color > ReadPPM( const std::string &filename ) {
    try {
      COMMENT( "Opening file.", 2 );
      IFile file;
      file.exceptions( std::fstream::failbit | std::fstream::badbit );
      file.open( filename );
      COMMENT( "Ignoring comments.", 2 );
      std::string ignore;
      while( ( file.peek( ) == '#' ) || ( file.peek( ) == '\n' ) ) {
        getline( file, ignore );
      }
      COMMENT( "Getting image type.", 2 );
      std::string ppm_type;
      getline( file, ppm_type );
      COMMENT( "Ignoring comments.", 2 );
      while( ( file.peek( ) == '#' ) || ( file.peek( ) == '\n' ) ) {
        getline( file, ignore );
      }
      COMMENT( "Reading image dimensions.", 2 );
      size_t xsize, ysize;
      file >> xsize >> ysize;
      COMMENT( "Dimensions: " << xsize << ", " << ysize << ". Ignoring more comments.", 2 );
      while( ( file.peek( ) == '#' ) || ( file.peek( ) == '\n' ) ) {
        getline( file, ignore );
      }
      COMMENT( "Getting image maximum intensity.", 2 );
      unsigned int maxval;
      file >> maxval;
      COMMENT( "Creating resultant image.", 2 );
      Image< Color > res( xsize, ysize );
      COMMENT( "Getting image size.", 2 );
      size_t img_size = xsize * ysize;
      COMMENT( "Image size " << img_size << ".", 2 );

      COMMENT( "Comparing the type of the .ppm file.", 2 );
      if( ppm_type.find( "P3" ) != std::string::npos ) {
        COMMENT( "Reading text file data.", 2 );
        for( size_t pxl = 0; pxl < img_size; ++pxl ) {
          for( size_t chl = 1; chl < 4; ++chl ) {
            uint val;
            file >> val;
            if( val <= maxval ) {
              res( pxl )( chl ) = static_cast< uchar >( val );
            }
            else {
              std::string msg( BIAL_ERROR( "Corrupted ppm file." ) );
              throw( std::logic_error( msg ) );
            }
          }
        }
        file.close( );
        return( res );
      }
      else if( ppm_type.find( "P6" ) != std::string::npos ) {
        COMMENT( "Ignoring byte.", 2 );
        getline( file, ignore );
        if( maxval < 256 ) {
          COMMENT( "Reading 8 bit binary file data.", 2 );
          Vector< uchar > data8( img_size * 3 );
          file.read( reinterpret_cast< char* >( &( data8[ 0 ] ) ), img_size * 3 );
          size_t idx = 0;
          for( size_t pxl = 0; pxl < img_size; ++pxl ) {
            for( size_t chl = 1; chl < 4; ++chl ) {
              if( data8[ idx ] <= maxval ) {
                res( pxl )( chl ) = data8[ idx ];
              }
              else {
                std::string msg( BIAL_ERROR( "Corrupted ppm file. Some pixel values are greater than the given"
                                             + "maximum." ) );
                throw( std::logic_error( msg ) );
              }
              ++idx;
            }
          }
        }
        else if( maxval < 65536 ) {
          COMMENT( "Reading 16 bit data.", 2 );
          Vector< unsigned short > data16( img_size * 3 );
          file.read( reinterpret_cast< char* >( &( data16[ 0 ] ) ), img_size * 6 );
          size_t idx = 0;
          for( size_t pxl = 0; pxl < img_size; ++pxl ) {
            for( size_t chl = 1; chl < 4; ++chl ) {
              if( data16[ idx ] <= maxval ) {
                res( pxl )( chl ) = static_cast< uchar >( data16[ idx ] );
              }
              else {
                std::string msg( BIAL_ERROR( "Corrupted ppm file." ) );
                throw( std::logic_error( msg ) );
              }
              ++idx;
            }
          }
        }
        else { /* if( maxval <= 4294967295 ) { */
          COMMENT( "Reading 32 bit data.", 2 );
          Vector< unsigned int > data32( img_size * 3 );
          file.read( reinterpret_cast< char* >( &( data32[ 0 ] ) ), img_size * 12 );
          size_t idx = 0;
          for( size_t pxl = 0; pxl < img_size; ++pxl ) {
            for( size_t chl = 1; chl < 4; ++chl ) {
              if( data32[ idx ] <= maxval ) {
                res( pxl )( chl ) = static_cast< uchar >( data32[ idx ] );
              }
              else {
                std::string msg( BIAL_ERROR( "Corrupted ppm file." ) );
                throw( std::logic_error( msg ) );
              }
              ++idx;
            }
          }
        }
        file.close( );
        return( res );
      }
      COMMENT( "Not P3 nor P6 -> Invalid type.", 2 );
      std::string msg( BIAL_ERROR( "Corrupted ppm file." ) );
      throw( std::logic_error( msg ) );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error opening/reading/closing PPM file." ) );
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
  Image< D > ReadPNM( const std::string &filename ) {
    try {
      COMMENT( "Opening file.", 2 );
      IFile file;
      file.exceptions( std::fstream::failbit | std::fstream::badbit );
      file.open( filename );
      COMMENT( "Reading comments in header.", 2 );
      std::string ignore;
      while( ( file.peek( ) == '#' ) || ( file.peek( ) == '\n' ) ) {
        getline( file, ignore );
      }
      COMMENT( "Getting image type among PBM, PGM, and PBM.", 2 );
      std::string pnm_type;
      getline( file, pnm_type );
      file.close( );
      COMMENT( "Comparing the first line that represents the type of the .pnm file.", 2 );
      if( ( pnm_type.find( "P1" ) != std::string::npos ) || ( pnm_type.find( "P4" ) != std::string::npos ) ) {
        return( ReadPBM< D >( filename ) );
      }
      if( ( pnm_type.find( "P2" ) != std::string::npos ) || ( pnm_type.find( "P5" ) != std::string::npos ) ) {
        return( ReadPGM< D >( filename ) );
      }
      if( ( pnm_type.find( "P3" ) != std::string::npos ) || ( pnm_type.find( "P6" ) != std::string::npos ) ) {
        return( ReadPPM< D >( filename ) );
      }
      COMMENT( "If tipe is invalid, call exception.", 2 );
      std::string msg( BIAL_ERROR( "Unsupported or currupted pnm file." ) );
      throw( std::logic_error( msg ) );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error opening/reading/closing PNM file." ) );
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
  Image< Color > ReadPNM( const std::string &filename ) {
    try {
      COMMENT( "Opening file.", 2 );
      IFile file;
      file.exceptions( std::fstream::failbit | std::fstream::badbit );
      file.open( filename );
      COMMENT( "Reading comments in header.", 2 );
      std::string ignore;
      while( ( file.peek( ) == '#' ) || ( file.peek( ) == '\n' ) ) {
        getline( file, ignore );
      }
      COMMENT( "Getting image type among PBM, PGM, and PBM.", 2 );
      std::string pnm_type;
      getline( file, pnm_type );
      file.close( );
      COMMENT( "Comparing the first line that represents the type of the .pnm file.", 2 );
      if( ( pnm_type.find( "P1" ) != std::string::npos ) || ( pnm_type.find( "P4" ) != std::string::npos ) ) {
        return( ReadPBM< Color >( filename ) );
      }
      if( ( pnm_type.find( "P2" ) != std::string::npos ) || ( pnm_type.find( "P5" ) != std::string::npos ) ) {
        return( ReadPGM< Color >( filename ) );
      }
      if( ( pnm_type.find( "P3" ) != std::string::npos ) || ( pnm_type.find( "P6" ) != std::string::npos ) ) {
        return( ReadPPM< Color >( filename ) );
      }
      COMMENT( "If tipe is invalid, call exception.", 2 );
      std::string msg( BIAL_ERROR( "Unsupported or currupted pnm file." ) );
      throw( std::logic_error( msg ) );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error opening/reading/closing PNM file." ) );
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
  void WritePBM( const Image< D > &img, const std::string &filename, const PNMHeader &, const bool binary ) {
    try {
      COMMENT( "Insert here image type control.", 2 );
      if( img.Dims( ) != 2 ) {
        std::string msg( BIAL_ERROR( "Invalid number of spatial dimensions. Expected 2. Given: " +
                                     std::to_string( img.Dims( ) ) ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Check if image needs to be converted to PBM format.", 2 );
      D max = img.Maximum( );
      Image< D > bw_img( img );
      if( max > 1 ) {
        COMMENT( "Computing black/white threshold.", 2 );
        D threshold = max / 2;
        for( size_t pxl = 0; pxl < bw_img.size( ); ++pxl ) {
          if( bw_img( pxl ) > threshold )
            bw_img( pxl ) = 1;
          else
            bw_img( pxl ) = 0;
        }
      }
      COMMENT( "Opening file.", 2 );
      OFile file;
      file.exceptions( std::fstream::failbit | std::fstream::badbit );
      file.open( filename );
      COMMENT( "Getting dimensions.", 2 );
      size_t xsize = bw_img.size( 0 );
      size_t ysize = bw_img.size( 1 );
      COMMENT( "Writing data.", 2 );
      if( binary ) {
        COMMENT( "Writning header of binary file.", 2 );
        file << "P4" << std::endl << xsize << ' ' << ysize << std::endl;
        int val;
        unsigned char byte = 0;
        COMMENT( "Writing data.", 2 );
        for( size_t y = 0; y < ysize; ++y ) {
          for( size_t x = 0; x < xsize; ++x ) {
            val = 7 - x % 8;
            int bit = !static_cast< int >( bw_img( x, y ) );
            byte = byte | ( bit << val );
            if( ( ( x + 1 ) % 8 == 0 ) || ( x == xsize - 1 ) ) {
              file << byte;
              byte = 0;
            }
          }
        }
      }
      else {
        COMMENT( "Writing header of text file.", 2 );
        file << "P1" << std::endl << xsize << ' ' << ysize << std::endl;
        COMMENT( "Writing data.", 2 );
        for( size_t pxl = 0; pxl < bw_img.size( ); ++pxl ) {
          if( pxl % bw_img.size( 0 ) == 0 ) {
            file << std::endl;
          }
          file << !bw_img( pxl ) << ' ';
        }
      }
      file.close( );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error opening/writing/closing PBM file." ) );
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
  void WritePBM( const Image< Color > &img, const std::string &filename, const PNMHeader &hdr, const bool binary ) {
    try {
      COMMENT( "Insert here image type control.", 2 );
      if( img.Dims( ) != 2 ) {
        std::string msg( BIAL_ERROR( "Invalid number of dimensions. Given: " + std::to_string( img.Dims( ) ) +
                                     ", expected 2." ) );
        throw( std::logic_error( msg ) );
      }

      Image< int > gray_img( ColorSpace::ARGBtoGraybyBrightness< int >( img ) );
      WritePBM( gray_img, filename, hdr, binary );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error opening/writing/closing PGM file." ) );
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
  void WritePGM( const Image< D > &img, const std::string &filename, const PNMHeader &, const bool binary ) {
    try {
      COMMENT( "Insert here image type control.", 2 );
      if( img.Dims( ) != 2 ) {
        std::string msg( BIAL_ERROR( "Invalid number of dimensions. Given: " + std::to_string( img.Dims( ) ) +
                                     ", expected 2." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Opening file.", 2 );
      OFile file;
      file.exceptions( std::fstream::failbit | std::fstream::badbit );
      file.open( filename );
      COMMENT( "Getting dimensions, image size, and maximum intensity.", 2 );
      size_t img_size = img.size( );
      D maxval = img.Maximum( );
      size_t xsize = img.size( 0 );
      size_t ysize = img.size( 1 );

      COMMENT( "Writing data.", 2 );
      if( binary ) {
        COMMENT( "Writing header of binary file.", 2 );
        file << "P5" << std::endl << xsize << ' ' << ysize << std::endl << static_cast< int >( maxval ) << std::endl;
        COMMENT( "Writing data.", 2 );
        if( maxval < 256 ) {
          Vector< unsigned char > data8( img_size );
          for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {
            data8[ pxl ] = static_cast< unsigned char >( img( pxl ) );
          }
          file.write( reinterpret_cast< char* >( &data8[ 0 ] ), img_size );
        }
        else if( maxval < 65536 ) {
          Vector< unsigned short > data16( img_size );
          for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {
            data16[ pxl ] = static_cast< unsigned short >( img( pxl ) );
          }
          file.write( reinterpret_cast< char* >( &data16[ 0 ] ), 2 * img_size );
        }
        else { /* if( maxval < 4294967296 ) { */
          Vector< unsigned int > data32( img_size );
          for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {
            data32[ pxl ] = static_cast< unsigned int >( img( pxl ) );
          }
          file.write( reinterpret_cast< char* >( &data32[ 0 ] ), 4 * img_size );
        }
      }
      else {
        COMMENT( "Writing header of text file.", 2 );
        file << "P2" << std::endl;
        file << xsize << ' ' << ysize << std::endl << static_cast< int >( maxval );
        COMMENT( "Writing data.", 2 );
        for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {
          if( pxl % img.size( 0 ) == 0 ) {
            file << std::endl;
          }
          file << static_cast< int >( img( pxl ) ) << ' ';
        }
      }
      file.close( );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error opening/writing/closing PGM file." ) );
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
  void WritePGM( const Image< Color > &img, const std::string &filename, const PNMHeader &hdr, const bool binary ) {
    try {
      COMMENT( "Insert here image type control.", 2 );
      if( img.Dims( ) != 2 ) {
        std::string msg( BIAL_ERROR( "Invalid number of dimensions. Given: " + std::to_string( img.Dims( ) ) +
                                     ", expected 2." ) );
        throw( std::logic_error( msg ) );
      }
      Image< int > gray_img( ColorSpace::ARGBtoGraybyBrightness< int >( img ) );
      WritePGM( gray_img, filename, hdr, binary );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error opening/writing/closing PGM file." ) );
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
  void WritePPM( const Image< D > &img, const std::string &filename, const PNMHeader &hdr, const bool binary ) {
    try {
      COMMENT( "Insert here image type control.", 2 );
      if( img.Dims( ) != 2 ) {
        std::string msg( BIAL_ERROR( "Invalid number of dimensions. Given: " + std::to_string( img.Dims( ) ) +
                                     ", expected 2." ) );
        throw( std::logic_error( msg ) );
      }

      Image< Color > RGB_img( ColorSpace::GraytoARGB( img ) );
      WritePPM( RGB_img, filename, hdr, binary );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error opening/writing/closing PPM file." ) );
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
  void WritePPM( const Image< Color > &img, const std::string &filename, const PNMHeader &, const bool binary ) {
    try {
      COMMENT( "Insert here image type control.", 2 );
      if( img.Dims( ) != 2 ) {
        std::string msg( BIAL_ERROR( "Invalid number of dimensions. Given: " + std::to_string( img.Dims( ) ) +
                                     ", expected 2." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Opening file.", 2 );
      OFile file;
      file.exceptions( std::fstream::failbit | std::fstream::badbit );
      file.open( filename );
      COMMENT( "Getting maximum intensity and image dimensions.", 2 );
      uchar maxval = img( 0 )( 0 );
      for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {
        for( size_t chl = 1; chl < 4; ++chl ) {
          if( maxval < img( pxl )( chl ) )
            maxval = img( pxl )( chl );
        }
      }
      Image< Color > wrt_img( img );
      size_t xsize = wrt_img.size( 0 );
      size_t ysize = wrt_img.size( 1 );

      COMMENT( "Writing data.", 2 );
      if( binary ) {
        COMMENT( "Writing header of binary file.", 2 );
        file << "P6" << std::endl << xsize << ' ' << ysize << std::endl << static_cast< int >( maxval ) << std::endl;
        COMMENT( "Converting to channel in the first dimension.", 2 );
        size_t img_size = wrt_img.size( );
        Vector< unsigned char > data8( img_size * 3 );
        for( size_t pxl = 0; pxl < img_size; ++pxl ) {
          for( size_t chl = 0; chl < 3; ++chl ) {
            data8[ 3 * pxl + chl ] = static_cast< unsigned char >( wrt_img( pxl )( chl + 1 ) );
          }
        }
        COMMENT( "Writing data.", 2 );
        file.write( reinterpret_cast< char* >( &data8[ 0 ] ), img_size * 3 );
      }
      else {
        COMMENT( "Writing header of text file.", 2 );
        file << "P3" << std::endl << xsize << ' ' << ysize << std::endl << static_cast< int >( maxval );
        COMMENT( "Writing data.", 2 );
        for( size_t pxl = 0; pxl < wrt_img.size( ); ++pxl ) {
          for( size_t chl = 1; chl < 4; ++chl ) {
            if( pxl % wrt_img.size( 0 ) == 0 ) {
              file << std::endl;
            }
            file << static_cast< int >( wrt_img( pxl )( chl ) ) << ' ';
          }
        }
      }
      file.close( );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error opening/writing/closing PGM file." ) );
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
  void WritePNM( const Image< D > &img, const std::string &filename, const PNMHeader &hdr, const bool binary ) {
    try {
      COMMENT( "Finding the correct image format (P1, P2, P3, P4, P5 or P6).", 2 );
      if( img.Maximum( ) <= 1 ) {
        WritePBM( img, filename, hdr, binary );
        return;
      }
      WritePGM( img, filename, hdr, binary );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "I/O error while writing to PNM file." ) );
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
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

  template< >
  void WritePNM( const Image< Color > &img, const std::string &filename, const PNMHeader &hdr, 
                 const bool binary ) {
    try {
      WritePPM( img, filename, hdr, binary );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "I/O error while writing to PNM file." ) );
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
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

}

#endif
