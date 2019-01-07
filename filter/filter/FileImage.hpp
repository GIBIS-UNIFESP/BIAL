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
    Write( obj, "dat/" + std::string( exp ) + ".nii" );		\
  }
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
#include "FileNifti.hpp"
#include "NiftiHeader.hpp"

namespace Bial {

  bool Supported( const std::string &filename ) {
    try {
      std::string extension = File::ToLowerExtension( filename );
      COMMENT( "Insert here new types.", 4 );
      if( ( extension.find( ".nii" ) != std::string::npos ) || ( extension.find( ".img" ) != std::string::npos ) || ( extension.find( ".hdr" ) != std::string::npos ) ) {
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
      std::string extension( File::ToLowerExtension( filename, static_cast< size_t >( std::max( 0, static_cast< int >( filename.size( ) ) - 8 ) ) ) );
      if( ( extension.rfind( ".hdr" ) != std::string::npos ) | ( extension.rfind( ".img" ) != std::string::npos ) |
          ( extension.rfind( ".nii" ) != std::string::npos ) ) {
        return( ReadNifti< D >( filename ) );
      }
      COMMENT( "Call here other image extensions.", 2 );
      std::string msg( BIAL_ERROR( "Unsupported extension for file " + filename + ". Currently supported: .img, .hdr, .nii" ) );
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
      COMMENT( "Call here other image extensions.", 2 );
      std::string msg( BIAL_ERROR( "Unsupported extension for file " + filename + ". Currently supported: .img, .hdr, .nii " ) );
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
      WriteNifti( img, filename, NiftiHeader( img ) );
      return;
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
      WriteNifti( img, filename, NiftiHeader( img ) );
      return;
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
      WriteNifti( img, filename, NiftiHeader( headername ) );
      return;
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
      WriteNifti( img, filename, NiftiHeader( headername ) );
      return;
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
