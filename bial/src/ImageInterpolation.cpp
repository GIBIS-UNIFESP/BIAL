/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2016/Sep/15 
 * @brief Image interpolation using interpolation functions.
 */

#ifndef BIALIMAGEINTERPOLATION_C
#define BIALIMAGEINTERPOLATION_C

#include "ImageInterpolation.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_ImageInterpolation )
#define BIAL_EXPLICIT_ImageInterpolation
#endif

#if defined ( BIAL_EXPLICIT_ImageInterpolation ) || ( BIAL_IMPLICIT_BIN )

#include "Image.hpp"
#include "PixelInterpolation.hpp"

namespace Bial {

  template< class D >
  Image< D > ImageInterpolation::Interpolation( const Image< D > &img, float dx, float dy, 
                                                  const PixelInterpolation &interpolation_type ) {
    if( img.Dims( ) != 2 ) {
      std::string msg( BIAL_ERROR( "Must be a 2D input image. " ) );
      throw( std::logic_error( msg ) );
    }
    if( ( dx < 0.01 ) || ( dy < 0.01 ) ) {
      std::string msg( BIAL_ERROR( "Pixel dimensions must be greater than 1. Given: dx: " + std::to_string( dx ) +
                                   ", dy: " + std::to_string( dy ) + "." ) );
      throw( std::logic_error( msg ) );
    }
    if( ( img.PixelSize( 0 ) < 0.01 ) || ( img.PixelSize( 1 ) < 0.01 ) ) {
      std::string msg( BIAL_ERROR( "Invalid input image pixel size. Given: dx: " + std::to_string( img.PixelSize( 0 ) )
                                   + ", dy: " + std::to_string( img.PixelSize( 1 ) ) + "." ) );
      throw( std::logic_error( msg ) );
    }
    try {
      COMMENT( "Return the input image if the same pixel dimensions are given.", 0 );
      if( ( img.PixelSize( 0 ) >= dx - 0.001 ) && ( img.PixelSize( 0 ) <= dx + 0.001 ) &&
          ( img.PixelSize( 1 ) >= dy - 0.001 ) && ( img.PixelSize( 1 ) <= dy + 0.001 ) )
        return( img );
      COMMENT( "Computing output image pixel dimensions.", 0 );
      Vector< float > pxl_size( 2 );
      pxl_size[ 0 ] = dx;
      pxl_size[ 1 ] = dy;
      COMMENT( "Computing step used to run over in input image.", 0 );
      float delta_x = pxl_size[ 0 ] / img.PixelSize( 0 );
      float delta_y = pxl_size[ 1 ] / img.PixelSize( 1 );
      COMMENT( "Computing multiplication factor to compute output image size.", 0 );
      float factor_x = 1.0f / delta_x;
      float factor_y = 1.0f / delta_y;
      COMMENT( "Computing output image dimensions and creating output image.", 0 );
      Vector< float > img_size( 2 );
      img_size[ 0 ] = std::round( img.size( 0 ) * factor_x );
      img_size[ 1 ] = std::round( img.size( 1 ) * factor_y );
      Image< D > res( img_size, pxl_size );
      COMMENT( "Running interpolation for all new pixels.", 0 );
      float src_y = 0;
      for( size_t tgt_y = 0; tgt_y < img_size[ 1 ]; ++tgt_y, src_y += delta_y ) {
        float src_x = 0;
        for( size_t tgt_x = 0; tgt_x < img_size[ 0 ]; ++tgt_x, src_x += delta_x ) {
          res( tgt_x, tgt_y ) = interpolation_type( img, src_x, src_y );
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
      std::string msg( e.what( ) + std::string( "\n" ) + 
                       BIAL_ERROR( "Image, window end, and/or window size dimensions do not match." ) );
      throw( std::logic_error( msg ) );
    }
  }

  template< class D >
  Image< D > ImageInterpolation::Interpolation( const Image< D > &img, float dx, float dy, float dz, 
                                                const PixelInterpolation &interpolation_type ) {
    if( img.Dims( ) != 3 ) {
      std::string msg( BIAL_ERROR( "Must be a 3D input image. " ) );
      throw( std::logic_error( msg ) );
    }
    if( ( dx < 0.01 ) || ( dy < 0.01 ) || ( dz < 0.01 ) ) {
      std::string msg( BIAL_ERROR( "Pixel dimensions must be greater than 1. Given: dx: " + std::to_string( dx ) +
                                   ", dy: " + std::to_string( dy ) + ", dz: " + std::to_string( dz ) + "." ) );
      throw( std::logic_error( msg ) );
    }
    if( ( img.PixelSize( 0 ) < 0.01 ) || ( img.PixelSize( 1 ) < 0.01 ) || ( img.PixelSize( 2 ) < 0.01 ) ) {
      std::string msg( BIAL_ERROR( "Invalid input image pixel size. Given: dx: " + std::to_string( img.PixelSize( 0 ) )
                                   + ", dy: " + std::to_string( img.PixelSize( 1 ) ) + ", dz: " + 
                                   std::to_string( img.PixelSize( 2 ) ) + "." ) );
      throw( std::logic_error( msg ) );
    }
    try {
      COMMENT( "Return the input image if the same pixel dimensions are given.", 0 );
      if( ( img.PixelSize( 0 ) >= dx - 0.001 ) && ( img.PixelSize( 0 ) <= dx + 0.001 ) &&
          ( img.PixelSize( 1 ) >= dy - 0.001 ) && ( img.PixelSize( 1 ) <= dy + 0.001 ) &&
          ( img.PixelSize( 2 ) >= dz - 0.001 ) && ( img.PixelSize( 2 ) <= dz + 0.001 ) )
        return( img );
      COMMENT( "Input image size: " << img.Dim( ), 0 );
      COMMENT( "Input image pixel size: " << img.PixelSize( ), 0 );
      COMMENT( "Computing output image pixel dimensions.", 0 );
      Vector< float > pxl_size( 3 );
      pxl_size[ 0 ] = dx;
      pxl_size[ 1 ] = dy;
      pxl_size[ 2 ] = dz;
      COMMENT( "pixel size: " << pxl_size, 0 );
      COMMENT( "Computing step used to run over in input image.", 0 );
      float delta_x = pxl_size[ 0 ] / img.PixelSize( 0 );
      float delta_y = pxl_size[ 1 ] / img.PixelSize( 1 );
      float delta_z = pxl_size[ 2 ] / img.PixelSize( 2 );
      COMMENT( "deltas: " << delta_x << ", " << delta_y << ", " << delta_z, 0 );
      COMMENT( "Computing multiplication factor to compute output image size.", 0 );
      float factor_x = 1.0f / delta_x;
      float factor_y = 1.0f / delta_y;
      float factor_z = 1.0f / delta_z;
      COMMENT( "factors: " << factor_x << ", " << factor_y << ", " << factor_z, 0 );
      COMMENT( "Computing output image dimensions and creating output image.", 0 );
      Vector< float > img_size( 3 );
      img_size[ 0 ] = std::round( img.size( 0 ) * factor_x );
      img_size[ 1 ] = std::round( img.size( 1 ) * factor_y );
      img_size[ 2 ] = std::round( img.size( 2 ) * factor_z );
      COMMENT( "image size: " << img_size, 0 );
      Image< D > res( img_size, pxl_size );
      COMMENT( "Running interpolation for all new pixels.", 0 );
      float src_z = 0;
      for( size_t tgt_z = 0; tgt_z < img_size[ 2 ]; ++tgt_z, src_z += delta_z ) {
        float src_y = 0;
        for( size_t tgt_y = 0; tgt_y < img_size[ 1 ]; ++tgt_y, src_y += delta_y ) {
          float src_x = 0;
          for( size_t tgt_x = 0; tgt_x < img_size[ 0 ]; ++tgt_x, src_x += delta_x ) {
            res( tgt_x, tgt_y, tgt_z ) = interpolation_type( img, src_x, src_y, src_z );
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
      std::string msg( e.what( ) + std::string( "\n" ) + 
                       BIAL_ERROR( "Image, window end, and/or window size dimensions do not match." ) );
      throw( std::logic_error( msg ) );
    }
  }

#ifdef BIAL_EXPLICIT_ImageInterpolation

  template Image< int > ImageInterpolation::Interpolation( const Image< int > &img, float dx, float dy,
                                                           const PixelInterpolation &interpolation_type );
  template Image< int > ImageInterpolation::Interpolation( const Image< int > &img, float dx, float dy, float dz,
                                                           const PixelInterpolation &interpolation_type );
  template Image< llint > ImageInterpolation::Interpolation( const Image< llint > &img, float dx, float dy,
                                                             const PixelInterpolation &interpolation_type );
  template Image< llint > ImageInterpolation::Interpolation( const Image< llint > &img, float dx, float dy, float dz,
                                                             const PixelInterpolation &interpolation_type );
  template Image< float > ImageInterpolation::Interpolation( const Image< float > &img, float dx, float dy,
                                                             const PixelInterpolation &interpolation_type );
  template Image< float > ImageInterpolation::Interpolation( const Image< float > &img, float dx, float dy, float dz,
                                                             const PixelInterpolation &interpolation_type );
  template Image< double > ImageInterpolation::Interpolation( const Image< double > &img, float dx, float dy,
                                                              const PixelInterpolation &interpolation_type );
  template Image< double > ImageInterpolation::Interpolation( const Image< double > &img, float dx, float dy, float dz,
                                                              const PixelInterpolation &interpolation_type );

  
#endif

}


#endif

#endif
