/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date: 2014/Jan/07
 * @brief Image merging.
 */

#ifndef BIALIMAGEMERGE_C
#define BIALIMAGEMERGE_C

#include "Image.hpp"
#include "ImageMerge.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_ImageMerge )
#define BIAL_EXPLICIT_ImageMerge
#endif

#if defined ( BIAL_EXPLICIT_ImageMerge ) || ( BIAL_IMPLICIT_BIN )

namespace Bial {

  template< class D >
  Image< D > ImageOp::Merge( Vector< Image< D > > &img_vector, float new_pixel_size ) {
    try {
      COMMENT( "Checking if vector is not empty.", 2 );
      if( img_vector.size( ) == 0 ) {
        std::string msg( BIAL_ERROR( "img_vector is empty." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Checking if images have the same dimensions.", 2 );
      for( size_t i = 1; i < img_vector.size( ); ++i ) {
        if( img_vector[ 0 ].Dims( ) != img_vector[ i ].Dims( ) ) {
          std::string msg( BIAL_ERROR( "All images must have the same dimensions." ) );
          throw( std::logic_error( msg ) );
        }
      }
      COMMENT( "Checking if images are 2D.", 2 );
      if( img_vector[ 0 ].Dims( ) != 2 ) {
        std::string msg( BIAL_ERROR( "Images must be two dimensions." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Setting the parameters for the constructor.", 2 );
      Vector< size_t > spc_dims( img_vector[ 0 ].Dim( ) );
      spc_dims[ 2 ] = img_vector.size( );
      Vector< float > pixel_size( img_vector[ 0 ].PixelSize( ) );
      pixel_size[ 2 ] = new_pixel_size;
      Image< D > res( spc_dims, pixel_size );
      COMMENT( "Copying image contents.", 2 );
      for( size_t x = 0; x < spc_dims[ 0 ]; ++x ) {
        for( size_t y = 0; y < spc_dims[ 1 ]; ++y ) {
          for( size_t z = 0; z < spc_dims[ 2 ]; ++z ) {
            size_t src_coord = x + y * res.Displacement( 0 ) + z * res.Displacement( 1 );
            size_t tgt_coord = x + y * res.Displacement( 0 );
            res( src_coord ) = img_vector( z )( tgt_coord );
          }
        }
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

#ifdef BIAL_EXPLICIT_ImageMerge

  template Image< int > ImageOp::Merge( Vector< Image< int > > &img_vector, float new_pixel_size );
  template Image< llint > ImageOp::Merge( Vector< Image< llint > > &img_vector, float new_pixel_size );
  template Image< float > ImageOp::Merge( Vector< Image< float > > &img_vector, float new_pixel_size );
  template Image< double > ImageOp::Merge( Vector< Image< double > > &img_vector, float new_pixel_size );

#endif

}

#endif

#endif
