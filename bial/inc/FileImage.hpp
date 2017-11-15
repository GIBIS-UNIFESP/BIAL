/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2013/Aug/08
 * @brief Functions to read and write generic files and multiple files from directory.
 */

#include "Common.hpp"
#include "Vector.hpp"

#ifndef BIALFILEIMAGE_H
#define BIALFILEIMAGE_H

/* Macro _WRITE_ is translated to write images when in verbose mode. */
#ifdef BIAL_DEBUG
#define DEBUG_WRITE( obj, exp, num )                                    \
  if( Bial::verbose > num ) {                                           \
    Write( obj, "dat/" + std::string( exp ) +                           \
           ( ( obj ).Dims( ) == 3 ? ".nii.gz" : ( ( obj ).Dims( ) == 2 ? ".pnm" : ".bvet" ) ) ); }
#endif

namespace Bial {

  template< class D >
  class Image;
  template< class D >
  class Matrix;


  /**
   * @date 2014/Mai/26
   * @param filename: A filename.
   * @return True for supported images. Return false for folders.
   * @brief Returns true for supported images. Return false for folders.
   * @warning Must be optimized to change only letters.
   */
  static bool Supported( const std::string &filename );

  /**
   * @date 2012/Jul/02
   * @param filename: Source file to be readed.
   * @return Read image.
   * @brief Read an image from an input file and returns it.
   * @warning none.
   */
  template< class D >
  static Image< D > Read( const std::string &filename );

  /**
   * @date 2013/Oct/29
   * @param dirname: Source directory.
   * @return A vector with the images in the given directory.
   * @brief Reads all supported images contained in the
   * directory and return them in a vector of images.
   * @warning none.
   */
  template< class D >
  static Vector< Image< D > > ReadDir( const std::string &dirname );

  /**
   * @date 2012/Jul/03
   * @param img: input image.
   * @param filename: Source file to be written.
   * @return none.
   * @brief Write the input image to a file with given name.
   * @warning none.
   */
  template< class D >
  static void Write( const Image< D > &img, const std::string &filename );

  /**
   * @date 2013/Sep/25
   * @param img: input image.
   * @param filename: Name of the file to be written.
   * @param headername: Name of input file containg header to be cloned.
   * @return none.
   * @brief Write the input image to a file with given
   * name. Also, includes the header read from given
   * header name.
   * @warning Implemented only for nifti-1 files.
   */
  template< class D >
  static void Write( const Image< D > &img, const std::string &filename, const std::string &headername );

}

/* Implementation --------------------------------------------------------------------------------------------------- */

#include "Color.hpp"
#include "File.hpp"
#include "FileBMP.hpp"
#include "FileDicom.hpp"
#include "FileMatrix.hpp"
#include "FileNifti.hpp"
#include "FilePNM.hpp"
#include "FileScene.hpp"
#include "NiftiHeader.hpp"

namespace Bial {

  bool Supported( const std::string &filename ) {
    try {
      std::string extension = File::ToLowerExtension( filename );
      COMMENT( "Insert here new types.", 4 );
      if( ( extension.find( ".pbm" ) != std::string::npos ) || ( extension.find( ".pgm" ) != std::string::npos ) ||
          ( extension.find( ".ppm" ) != std::string::npos ) || ( extension.find( ".pnm" ) != std::string::npos ) ||
          ( extension.find( ".scn" ) != std::string::npos ) || ( extension.find( ".nii" ) != std::string::npos ) ||
          ( extension.find( ".img" ) != std::string::npos ) || ( extension.find( ".hdr" ) != std::string::npos ) ||
          ( extension.find( ".dcm" ) != std::string::npos ) || ( extension.find( ".bmat" ) != std::string::npos ) ) {
        return( true );
      }
      return( false );
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

  template< class D >
  Vector< Image< D > > ReadDir( const std::string &dirname ) {
    try {
      COMMENT( "Getting files in folder.", 2 );
      Vector< std::string > file = Directory::List( dirname, false );

      COMMENT( "Reading imagens from files.", 2 );
      Vector< Image< D > > result;
      for( size_t idx = 0; idx < file.size( ); ++idx ) {
        result.push_back( Read< D >( dirname + DIR_SEPARATOR + file[ idx ] ) );
      }
      return( result );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "I/O error while reading directory or file." ) );
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

  template< class D >
  Image< D > Read( const std::string &filename ) {
    try {
      COMMENT( "Checking file type.", 2 );
      std::string extension( File::ToLowerExtension( filename, static_cast< size_t >( std::max( 0,
                                                                                                static_cast< int >( filename.size( ) ) -
                                                                                                8 ) ) ) );
      if( ( extension.rfind( ".hdr" ) != std::string::npos ) | ( extension.rfind( ".img" ) != std::string::npos ) |
          ( extension.rfind( ".nii" ) != std::string::npos ) ) {
        return( ReadNifti< D >( filename ) );
      }
      if( extension.rfind( ".scn" ) != std::string::npos ) {
        return( ReadScene< D >( filename ) );
      }
      if( extension.rfind( ".pnm" ) != std::string::npos ) {
        return( ReadPNM< D >( filename ) );
      }
      if( extension.rfind( ".pbm" ) != std::string::npos ) {
        return( ReadPBM< D >( filename ) );
      }
      if( extension.rfind( ".pgm" ) != std::string::npos ) {
        return( ReadPGM< D >( filename ) );
      }
      if( extension.rfind( ".ppm" ) != std::string::npos ) {
        return( ReadPPM< D >( filename ) );
      }
      if( extension.rfind( ".dcm" ) != std::string::npos ) {
        return( ReadDicom< D >( filename ) );
      }
      if( extension.rfind( ".bmat" ) != std::string::npos ) {
        return( ReadMatrixImage< D >( filename ) );
      }
      COMMENT( "Call here other image extensions.", 2 );
      std::string msg( BIAL_ERROR(
                         "Unsupported extension for file " + filename + ". Currently supported: .scn(.gz), " +
                         ".img(.gz), .hdr(.gz), .nii(.gz), .pnm(.gz), .ppm(.gz), .pgm(.gz), .pbm(.gz), " +
                         ".dcm(.gz), .bmat(.gz)" ) );
      throw( std::invalid_argument( msg ) );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "I/O error while reading file." ) );
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
  Image< Color > Read( const std::string &filename ) {
    try {
      COMMENT( "Checking file type.", 2 );
      std::string extension
        ( File::ToLowerExtension
          ( filename, static_cast< size_t >( std::max( 0, static_cast< int >( filename.size( ) ) - 8 ) ) ) );
      if( ( extension.rfind( ".hdr" ) != std::string::npos ) | ( extension.rfind( ".img" ) != std::string::npos ) |
          ( extension.rfind( ".nii" ) != std::string::npos ) ) {
        return( ReadNifti< Color >( filename ) );
      }
      if( extension.rfind( ".scn" ) != std::string::npos ) {
        return( ReadScene< Color >( filename ) );
      }
      if( extension.rfind( ".pnm" ) != std::string::npos ) {
        return( ReadPNM< Color >( filename ) );
      }
      if( extension.rfind( ".pbm" ) != std::string::npos ) {
        return( ReadPBM< Color >( filename ) );
      }
      if( extension.rfind( ".pgm" ) != std::string::npos ) {
        return( ReadPGM< Color >( filename ) );
      }
      if( extension.rfind( ".ppm" ) != std::string::npos ) {
        return( ReadPPM< Color >( filename ) );
      }
      if( extension.rfind( ".dcm" ) != std::string::npos ) {
        return( ReadDicom< Color >( filename ) );
      }
      if( extension.rfind( ".bmat" ) != std::string::npos ) {
        return( ReadMatrixImage< Color >( filename ) );
      }
      COMMENT( "Call here other image extensions.", 2 );
      std::string msg( BIAL_ERROR(
                         "Unsupported extension for file " + filename + ". Currently supported: .scn(.gz), " +
                         ".img(.gz), .hdr(.gz), .nii(.gz), .pnm(.gz), .ppm(.gz), .pgm(.gz), .pbm(.gz), " +
                         ".dcm(.gz), .bmat(.gz)" ) );
      throw( std::invalid_argument( msg ) );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "I/O error while reading file." ) );
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


  template< class D >
  void Write( const Image< D > &img, const std::string &filename ) {
    try {
      COMMENT( "Checking file type.", 2 );
      std::string
        extension( File::ToLowerExtension( filename,
                                           static_cast< size_t >( std::max( 0, static_cast< int >( filename.size( ) )
                                                                            - 8 ) ) ) );
      if( ( extension.rfind( ".hdr" ) != std::string::npos ) || ( extension.rfind( ".img" ) != std::string::npos ) ||
          ( extension.rfind( ".nii" ) != std::string::npos ) ) {
        WriteNifti( img, filename, NiftiHeader( img ) );
        return;
      }
      if( extension.rfind( ".scn" ) != std::string::npos ) {
        WriteScene( img, filename );
        return;
      }
      if( extension.rfind( ".pnm" ) != std::string::npos ) {
        WritePNM( img, filename, PNMHeader( ) );
        return;
      }
      if( extension.rfind( ".pbm" ) != std::string::npos ) {
        WritePBM( img, filename, PNMHeader( ) );
        return;
      }
      if( extension.rfind( ".pgm" ) != std::string::npos ) {
        WritePGM( img, filename, PNMHeader( ) );
        return;
      }
      if( extension.rfind( ".ppm" ) != std::string::npos ) {
        WritePPM( img, filename, PNMHeader( ) );
        return;
      }
      if( extension.rfind( ".bmp" ) != std::string::npos ) {
        WriteBMP( img, filename );
        return;
      }
      if( extension.rfind( ".bmat" ) != std::string::npos ) {
        WriteMatrixImage( img, filename );
        return;
      }
      COMMENT( "Call here other image extensions.", 2 );
      std::string msg( BIAL_ERROR(
                         "Unsupported extension. Currently supported: .scn(.gz), .bmp(.gz) .img(.gz), .hdr(.gz), "
                         + ".nii(.gz), .pnm(.gz), .ppm(.gz), .pgm(.gz), .pbm(.gz), .bmat(.gz)." ) );
      throw( std::invalid_argument( msg ) );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "I/O error while writing to file." ) );
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
  void Write( const Image< Color > &img, const std::string &filename ) {
    try {
      COMMENT( "Checking file type.", 2 );
      std::string
        extension( File::ToLowerExtension( filename,
                                           static_cast< size_t >( std::max( 0, static_cast< int >( filename.size( ) )
                                                                            - 8 ) ) ) );
      if( ( extension.rfind( ".hdr" ) != std::string::npos ) || ( extension.rfind( ".img" ) != std::string::npos ) ||
          ( extension.rfind( ".nii" ) != std::string::npos ) ) {
        WriteNifti( img, filename, NiftiHeader( img ) );
        return;
      }
      if( extension.rfind( ".scn" ) != std::string::npos ) {
        WriteScene( img, filename );
        return;
      }
      if( extension.rfind( ".pnm" ) != std::string::npos ) {
        WritePNM( img, filename, PNMHeader( ) );
        return;
      }
      if( extension.rfind( ".pbm" ) != std::string::npos ) {
        WritePBM( img, filename, PNMHeader( ) );
        return;
      }
      if( extension.rfind( ".pgm" ) != std::string::npos ) {
        WritePGM( img, filename, PNMHeader( ) );
        return;
      }
      if( extension.rfind( ".ppm" ) != std::string::npos ) {
        WritePPM( img, filename, PNMHeader( ) );
        return;
      }
      if( extension.rfind( ".bmp" ) != std::string::npos ) {
        WriteBMP( img, filename );
        return;
      }
      if( extension.rfind( ".bmat" ) != std::string::npos ) {
        WriteMatrixImage( img, filename );
        return;
      }
      COMMENT( "Call here other image extensions.", 2 );
      std::string msg( BIAL_ERROR(
                         "Unsupported extension. Currently supported: .scn(.gz), .bmp(.gz) .img(.gz), .hdr(.gz), "
                         + ".nii(.gz), .pnm(.gz), .ppm(.gz), .pgm(.gz), .pbm(.gz), .bmat(.gz)." ) );
      throw( std::invalid_argument( msg ) );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "I/O error while writing to file." ) );
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

  template< class D >
  void Write( const Image< D > &img, const std::string &filename, const std::string &headername ) {
    try {
      std::string file_extension
        ( File::ToLowerExtension( filename, static_cast< size_t >
                                  ( std::max( 0, static_cast< int >( filename.size( ) ) - 8 ) ) ) );
      std::string header_extension
        ( File::ToLowerExtension( headername, static_cast< size_t >
                                  ( std::max( 0, static_cast< int >( headername.size( ) ) - 8 ) ) ) );
      if( ( file_extension.rfind( ".hdr" ) != std::string::npos ) ||
          ( file_extension.rfind( ".img" ) != std::string::npos ) ||
          ( file_extension.rfind( ".nii" ) != std::string::npos ) ) {
        if( ( header_extension.rfind( ".hdr" ) != std::string::npos ) ||
            ( header_extension.rfind( ".img" ) != std::string::npos ) ||
            ( header_extension.rfind( ".nii" ) != std::string::npos ) ) {
          WriteNifti( img, filename, NiftiHeader( headername ) );
          return;
        }
        else {
          WriteNifti( img, filename, NiftiHeader( img ) );
          return;
        }
      }
      else {
        COMMENT( "Implement here for other headers.", 2 );
      }
      Write( img, filename );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "I/O error while writing to file." ) );
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
  void Write( const Image< Color > &img, const std::string &filename, const std::string &headername ) {
    try {
      std::string file_extension( File::ToLowerExtension( filename, static_cast< size_t >(
                                                            std::max( 0,
                                                                      static_cast< int >( filename.size( ) ) -
                                                                      8 ) ) ) );
      std::string header_extension( File::ToLowerExtension( headername, static_cast< size_t >(
                                                              std::max( 0,
                                                                        static_cast< int >( headername.size( ) ) -
                                                                        8 ) ) ) );
      if( ( file_extension.rfind( ".hdr" ) != std::string::npos ) ||
          ( file_extension.rfind( ".img" ) != std::string::npos ) ||
          ( file_extension.rfind( ".nii" ) != std::string::npos ) ) {
        if( ( header_extension.rfind( ".hdr" ) != std::string::npos ) ||
            ( header_extension.rfind( ".img" ) != std::string::npos ) ||
            ( header_extension.rfind( ".nii" ) != std::string::npos ) ) {
          WriteNifti( img, filename, NiftiHeader( headername ) );
          return;
        }
        else {
          WriteNifti( img, filename, NiftiHeader( img ) );
          return;
        }
      }
      else {
        COMMENT( "Implement here for other headers.", 2 );
      }
      Write( img, filename );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "I/O error while writing to file." ) );
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
