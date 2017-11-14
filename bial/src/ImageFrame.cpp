/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date: 2014/Jan/07
 * @brief Image add/remove frame.
 */

#ifndef BIALIMAGEFRAME_C
#define BIALIMAGEFRAME_C

#include "ImageFrame.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_ImageFrame )
#define BIAL_EXPLICIT_ImageFrame
#endif

#if defined ( BIAL_EXPLICIT_ImageFrame ) || ( BIAL_IMPLICIT_BIN )

#include "Image.hpp"
#include "ImageResize.hpp"

namespace Bial {

  template< class D >
  Image< D > ImageOp::RemoveFrame( const Image< D > &img, const Vector< size_t > &low_coord,
                                   const Vector< size_t > &hgh_coord ) {
    try {
      return( ImageOp::Resize( img, low_coord, hgh_coord ) );
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
  Image< D > ImageOp::RemoveFrame( const Image< D > &img, size_t width ) {
    try {
      COMMENT( "Computing image location and dimensions.", 0 );
      size_t dims = img.Dims( );
      Vector< size_t > low_coord( dims, width );
      Vector< size_t > hgh_coord( img.Dim( ) );
      if( dims == 2 )
        hgh_coord.pop_back( );
      for( size_t dms = 0; dms < dims; ++dms )
        hgh_coord( dms ) -= width;
      return( ImageOp::Resize( img, low_coord, hgh_coord ) );
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
  Image< D > ImageOp::AddFrame( const Image< D > &img, const Vector< size_t > &base_coord,
                                const Vector< size_t > &full_size ) {
    try {
      COMMENT( "Verifying dimensions.", 1 );
      if( base_coord.size( ) != full_size.size( ) ) {
        std::string msg( BIAL_ERROR( "base_size and full_size number of dimensions do not match." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Converting image.", 0 );
      Image< D > res( full_size, img.PixelSize( ) );
      size_t dimensions = img.Dims( );
      Vector< size_t > pxl_coord( dimensions, 0 );
      Vector< size_t > max_dim( img.Dim( ) );
      for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {
        COMMENT( "Updating verification coordinates.", 3 );
        for( size_t dms = 0; dms < dimensions; ++dms ) {
          if( pxl_coord( dms ) < max_dim( dms ) - 1 ) {
            ++pxl_coord( dms );
            break;
          }
          else {
            pxl_coord( dms ) = 0;
          }
        }
        COMMENT( "Computing source coordinates.", 3 );
        Vector< size_t > tgt_coord( pxl_coord );
        for( size_t dms = 0; dms < dimensions; ++dms ) {
          tgt_coord( dms ) += base_coord( dms );
        }
        res( tgt_coord ) = img( pxl_coord );
      }
      return( res );
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
  Image< D > ImageOp::AddFrame( const Image< D > &img, size_t width ) {
    try {
      COMMENT( "Computing image location and dimensions.", 0 );
      size_t dims = img.Dims( );
      Vector< size_t > base_coord( dims, width );
      Vector< size_t > full_size( img.Dim( ) );
      if( dims == 2 )
        full_size.pop_back( );
      for( size_t dms = 0; dms < dims; ++dms )
        full_size( dms ) += 2 * width;
      return( ImageOp::AddFrame( img, base_coord, full_size ) );
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

#ifdef BIAL_EXPLICIT_ImageFrame

  template Image< int > ImageOp::RemoveFrame( const Image< int > &img, const Vector< size_t > &low_coord,
                                              const Vector< size_t > &hgh_coord );
  template Image< int > ImageOp::RemoveFrame( const Image< int > &img, size_t width );
  template Image< int > ImageOp::AddFrame( const Image< int > &img, const Vector< size_t > &base_coord,
                                           const Vector< size_t > &full_size );
  template Image< int > ImageOp::AddFrame( const Image< int > &img, size_t width );

  template Image< llint > ImageOp::RemoveFrame( const Image< llint > &img, const Vector< size_t > &low_coord,
                                                const Vector< size_t > &hgh_coord );
  template Image< llint > ImageOp::RemoveFrame( const Image< llint > &img, size_t width );
  template Image< llint > ImageOp::AddFrame( const Image< llint > &img, const Vector< size_t > &base_coord,
                                             const Vector< size_t > &full_size );
  template Image< llint > ImageOp::AddFrame( const Image< llint > &img, size_t width );

  template Image< float > ImageOp::RemoveFrame( const Image< float > &img, const Vector< size_t > &low_coord,
                                                const Vector< size_t > &hgh_coord );
  template Image< float > ImageOp::RemoveFrame( const Image< float > &img, size_t width );
  template Image< float > ImageOp::AddFrame( const Image< float > &img,
                                             const Vector< size_t > &base_coord,
                                             const Vector< size_t > &full_size );
  template Image< float > ImageOp::AddFrame( const Image< float > &img, size_t width );

  template Image< double > ImageOp::RemoveFrame( const Image< double > &img, const Vector< size_t > &low_coord,
                                                 const Vector< size_t > &hgh_coord );
  template Image< double > ImageOp::RemoveFrame( const Image< double > &img, size_t width );
  template Image< double > ImageOp::AddFrame( const Image< double > &img, const Vector< size_t > &base_coord,
                                              const Vector< size_t > &full_size );
  template Image< double > ImageOp::AddFrame( const Image< double > &img, size_t width );


#endif

}

#endif

#endif
