/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date: 2014/Jan/07
 * @brief Operations to get sub-matrix.
 */

#ifndef BIALIMAGERESIZE_C
#define BIALIMAGERESIZE_C

#include "ImageResize.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_ImageResize )
#define BIAL_EXPLICIT_ImageResize
#endif

#if defined ( BIAL_EXPLICIT_ImageResize ) || ( BIAL_IMPLICIT_BIN )

#include "Image.hpp"
#include "MatrixResize.hpp"

namespace Bial {
  
  template< class D >
  Image< D > ImageOp::Resize( const Image< D > &img, const Vector< size_t > &min, const Vector< size_t > &max ) {
    try {
      COMMENT( "Checking if min and max have the same amount of dimensions of image.", 2 );
      size_t dims = img.Dims( );
      if( max.size( ) != min.size( ) ) {
        std::string msg( BIAL_ERROR( "Number of dimensions of the image does not match input vectors. min: " +
                                     std::to_string( min.size( ) ) + ", max: " + std::to_string( max.size( ) ) +
                                     ", image spatial dims: " + std::to_string( dims ) + "." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "New pixel size: ", 2 );
      Vector< float > new_pixel_size( img.PixelSize( ) );
      COMMENT( "Creating sub image.", 2 );
      COMMENT( "Pixel size: " << new_pixel_size, 1 );
      Matrix< D > mat( MatrixOp::Resize( img.DataMatrix( ), min, max ) );
      COMMENT( "img dims: " << mat.Dim( ), 1 );
      Image< D > res( mat, new_pixel_size );
      return( res );
    }
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
    }
  }

  template< class D >
  Image< D > ImageOp::Resize( const Image< D > &img, const std::string &coordinates ) {
    try {
      COMMENT( "Computes coodinates from string.", 2 );
      Matrix< size_t > coords( MatrixOp::Coordinates( img.DataMatrix( ), coordinates ) );
      COMMENT( "Coordinates: " << coords << ". Assigning coordinates. to vector.", 2 );
      Vector< size_t > new_min( coords.size( 0 ) );
      Vector< size_t > new_max( coords.size( 0 ) );
      for( size_t dms = 0; dms < coords.size( 0 ); ++dms ) {
        new_min( dms ) = coords( dms, 0 );
        new_max( dms ) = coords( dms, 1 );
      }
      COMMENT( "Computing sub image.", 2 );
      Image< D > res( ImageOp::Resize( img, new_min, new_max ) );
      COMMENT( "returning result with dims: " << res.Dims( ), 2 );
      return( res );
    }
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
    }
  }

#ifdef BIAL_EXPLICIT_ImageResize

  template Image< int > ImageOp::Resize( const Image< int > &img, const Vector< size_t > &new_min,
                                         const Vector< size_t > &new_max );
  template Image< int > ImageOp::Resize( const Image< int > &img, const std::string &coordinates );

  template Image< llint > ImageOp::Resize( const Image< llint > &img, const Vector< size_t > &new_min, 
                                           const Vector< size_t > &new_max );
  template Image< llint > ImageOp::Resize( const Image< llint > &img, const std::string &coordinates );

  template Image< float > ImageOp::Resize( const Image< float > &img, const Vector< size_t > &new_min, 
                                           const Vector< size_t > &new_max );
  template Image< float > ImageOp::Resize( const Image< float > &img, const std::string &coordinates );

  template Image< double > ImageOp::Resize( const Image< double > &img, const Vector< size_t > &new_min, 
                                            const Vector< size_t > &new_max );
  template Image< double > ImageOp::Resize( const Image< double > &img, const std::string &coordinates );

#endif

}

#endif

#endif
