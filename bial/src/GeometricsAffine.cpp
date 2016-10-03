/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date: 2014/Jan/07
 * @brief Affine transformation.
 */

#ifndef BIALGEOMETRICSAFFINE_C
#define BIALGEOMETRICSAFFINE_C

#include "GeometricsAffine.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_GeometricsAffine )
#define BIAL_EXPLICIT_GeometricsAffine
#endif

#if defined ( BIAL_EXPLICIT_GeometricsAffine ) || ( BIAL_IMPLICIT_BIN )

namespace Bial {

  template< class D >
  Image< D > Geometrics::AffineTransform( const Image< D > &img, const Matrix< float > &transform ) {
    try {
      LinearInterpolation interpolation;
      return( Geometrics::AffineTransform( img, transform, interpolation ) );
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
  Image< D > Geometrics::AffineTransform( const Image< D > &img, const Matrix< float > &transform,
                                          const PixelInterpolation &interpolation ) {
    try {
      COMMENT( "BialAffine: " << std::endl << transform, 1 );
      if( img.Dims( ) == 2 ) {
        COMMENT( "2D image with 3x3 matrix.", 0 );
        Image< D > res( img );
        for( size_t v = 0; v < res.size( 1 ); ++v ) { /* scan values in y */
          for( size_t u = 0; u < res.size( 0 ); ++u ) { /* scan values in x */
            float x = u * transform( 0, 0 ) + v * transform( 1, 0 ) + transform( 3, 0 );
            float y = u * transform( 0, 1 ) + v * transform( 1, 1 ) + transform( 3, 1 );
            res( u, v ) = interpolation( img, x, y );
          }
        }
        return( res );
      }
      COMMENT( "3D image with 4x4 matrix.", 1 );
      Image< D > res( img );
      for( size_t w = 0; w < res.size( 2 ); ++w ) { /* scan values in z */
        for( size_t v = 0; v < res.size( 1 ); ++v ) { /* scan values in y */
          for( size_t u = 0; u < res.size( 0 ); ++u ) { /* scan values in x */
            float x = u * transform( 0, 0 ) + v * transform( 1, 0 ) + w * transform( 2, 0 ) + transform( 3, 0 );
            float y = u * transform( 0, 1 ) + v * transform( 1, 1 ) + w * transform( 2, 1 ) + transform( 3, 1 );
            float z = u * transform( 0, 2 ) + v * transform( 1, 2 ) + w * transform( 2, 2 ) + transform( 3, 2 );
            COMMENT( "Verifies if coordinates are in image domain.", 4 );
            if( ( x <= 0.0f ) || ( y <= 0.0f ) || ( z <= 0.0f ) || ( x > img.size( 0 ) ) || ( y > img.size( 1 ) ) || 
                ( z > img.size( 2 ) ) ) {
              res( u, v, w ) = 0.0;
            }
            else {
              res( u, v, w ) = interpolation( img, x, y, z );
            }
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

#ifdef BIAL_EXPLICIT_GeometricsAffine

  template Image< int > Geometrics::AffineTransform( const Image< int > &img, const Matrix< float > &transform );
  template Image< llint > Geometrics::AffineTransform( const Image< llint > &img, const Matrix< float > &transform );
  template Image< float > Geometrics::AffineTransform( const Image< float > &img, const Matrix< float > &transform );
  template Image< double > Geometrics::AffineTransform( const Image< double > &img, const Matrix< float > &transform );
  template Image< int > Geometrics::AffineTransform( const Image< int > &img, const Matrix< float > &transform,
                                                     const PixelInterpolation &interpolation );
  template Image< llint > Geometrics::AffineTransform( const Image< llint > &img, const Matrix< float > &transform,
                                                       const PixelInterpolation &interpolation );
  template Image< float > Geometrics::AffineTransform( const Image< float > &img, const Matrix< float > &transform,
                                                       const PixelInterpolation &interpolation );
  template Image< double > Geometrics::AffineTransform( const Image< double > &img, const Matrix< float > &transform,
                                                        const PixelInterpolation &interpolation );
  
#endif

}

#endif

#endif
