/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date: 2014/Jan/07
 * @brief Image translation. Direct translation or corresponding affine transform.
 */

#ifndef BIALGEOMETRICSTRANSLATE_C
#define BIALGEOMETRICSTRANSLATE_C

#include "GeometricsTranslate.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_GeometricsTranslate )
#define BIAL_EXPLICIT_GeometricsTranslate
#endif

#if defined ( BIAL_EXPLICIT_GeometricsTranslate ) || ( BIAL_IMPLICIT_BIN )

#include "MatrixIdentity.hpp"

namespace Bial {

  template< class D >
  Image< D > Geometrics::Translate( const Image< D > &img, double dx, double dy, double dz ) {
    try {
      LinearInterpolation interpolation;
      return( Geometrics::Translate( img, interpolation, dx, dy, dz ) );
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
  Image< D > Geometrics::Translate( const Image< D > &img, const PixelInterpolation &interpolation, double dx, 
                                    double dy, double dz ) {
    try {
      size_t dims = img.Dims( );
      COMMENT( "Verifying if tried to translate 2D image in z axis.", 1 );
      if( ( dims == 2 ) && ( dz != 0.0 ) ) {
        std::string msg( BIAL_ERROR( "Trying to translate 2D image in z coordinate." ) );
        throw( std::logic_error( msg ) );
      }
      Image< D > res( img );
      if( dims == 2 ) {
        for( size_t v = 0; v < img.size( 1 ); v++ ) { /* scan values in y */
          for( size_t u = 0; u < img.size( 0 ); u++ ) { /* scan values in x */
            float x = static_cast< float >( u - dx );
            float y = static_cast< float >( v - dy );
            COMMENT( "Verify if pixel is inside image domain.", 3 );
            if( ( x <= 0.0f ) || ( y <= 0.0f ) || ( x > img.size( 0 ) ) || ( y > img.size( 1 ) ) ) {
                res( u, v ) = 0;
            }
            else {
              res( u, v ) = interpolation( img, x, y );
            }
          }
        }
      }
      else {
        for( size_t w = 0; w < img.size( 2 ); w++ ) { /* scan values in z */
          for( size_t v = 0; v < img.size( 1 ); v++ ) { /* scan values in y */
            for( size_t u = 0; u < img.size( 0 ); u++ ) { /* scan values in x */
              float x = static_cast< float >( u - dx );
              float y = static_cast< float >( v - dy );
              float z = static_cast< float >( w - dz );
              COMMENT( "Verify if pixel is inside image domain.", 3 );
              if( ( x <= 0.0f ) || ( y <= 0.0f ) || ( x > img.size( 0 ) ) || ( y > img.size( 1 ) ) || 
                  ( z <= 0.0f ) || ( z > img.size( 2 ) ) ) {
                res( u, v, w ) = 0;
              }
              else {
                res( u, v, w ) = interpolation( img, x, y, z );
              }
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

  Matrix< float > Geometrics::Translate( double dx, double dy, double dz ) {
    try {
      Matrix< float > mat( 4, 4 );
      MatrixOp::Identity( mat );
      return( Geometrics::Translate( mat, dx, dy, dz ) );
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

  Matrix< float > Geometrics::Translate( Matrix< float > &source, double dx, double dy, double dz ) {
    try {
      Matrix< float > mat( 4, 4 );
      mat( 0, 0 ) = 1.0;
      mat( 1, 0 ) = 0.0;
      mat( 2, 0 ) = 0.0;
      mat( 3, 0 ) = -dx;
      mat( 0, 1 ) = 0.0;
      mat( 1, 1 ) = 1.0;
      mat( 2, 1 ) = 0.0;
      mat( 3, 1 ) = -dy;
      mat( 0, 2 ) = 0.0;
      mat( 1, 2 ) = 0.0;
      mat( 2, 2 ) = 1.0;
      mat( 3, 2 ) = -dz;
      mat( 0, 3 ) = 0.0;
      mat( 1, 3 ) = 0.0;
      mat( 2, 3 ) = 0.0;
      mat( 3, 3 ) = 1.0;
      return( source *= mat );
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

#ifdef BIAL_EXPLICIT_GeometricsTranslate

  template Image< int > Geometrics::Translate( const Image< int > &img, double dx, double dy, double dz );
  template Image< llint > Geometrics::Translate( const Image< llint > &img, double dx, double dy, double dz );
  template Image< float > Geometrics::Translate( const Image< float > &img, double dx, double dy, double dz );
  template Image< double > Geometrics::Translate( const Image< double > &img, double dx, double dy, double dz );
  template Image< int > Geometrics::Translate( const Image< int > &img, const PixelInterpolation &interpolation, 
                                               double dx, double dy, double dz );
  template Image< llint > Geometrics::Translate( const Image< llint > &img, const PixelInterpolation &interpolation, 
                                                 double dx, double dy, double dz );
  template Image< float > Geometrics::Translate( const Image< float > &img, const PixelInterpolation &interpolation, 
                                                 double dx, double dy, double dz );
  template Image< double > Geometrics::Translate( const Image< double > &img, const PixelInterpolation &interpolation,
                                                  double dx, double dy, double dz );

#endif

}

#endif

#endif
