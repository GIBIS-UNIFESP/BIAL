/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date: 2014/Jan/07
 * @brief Image shear. Direct shear or corresponding affine transform.
 */

#ifndef BIALGEOMETRICSSHEAR_C
#define BIALGEOMETRICSSHEAR_C

#include "GeometricsShear.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_GeometricsShear )
#define BIAL_EXPLICIT_GeometricsShear
#endif

#if defined ( BIAL_EXPLICIT_GeometricsShear ) || ( BIAL_IMPLICIT_BIN )

#include "MatrixIdentity.hpp"

namespace Bial {

  template< class D >
  Image< D > Geometrics::Shear( const Image< D > &img, double s_xy, double s_yx, double s_xz, double s_yz, 
                                double s_zx, double s_zy ) {
    try {
      LinearInterpolation interpolation;
      return( Geometrics::Shear( img, interpolation, s_xy, s_yx, s_xz, s_yz, s_zx, s_zy ) );
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
  Image< D > Geometrics::Shear( const Image< D > &img, const PixelInterpolation &interpolation, double s_xy, 
                                double s_yx, double s_xz, double s_yz, double s_zx, double s_zy ) {
    try {
      size_t dims = img.Dims( );
      if( ( dims == 2 ) && ( ( s_xz != 0.0 ) || ( s_yz != 0.0 ) || ( s_zx != 0.0 ) || ( s_zy != 0.0 ) ) ) {
        std::string msg( BIAL_ERROR( "Trying to shaer 2D image with respect to z axis." ) );
        throw( std::logic_error( msg ) );
      }
      Image< D > res( img );
      if( dims == 2 ) {
        for( size_t v = 0; v < img.size( 1 ); v++ ) { /* scan values in y */
          for( size_t u = 0; u < img.size( 0 ); u++ ) { /* scan values in x */
            float x = static_cast< float >( u - ( v * s_xy ) );
            float y = static_cast< float >( -( u * s_yx ) + v );
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
              float x = static_cast< float >( u - ( v * s_xy ) - ( w * s_xz ) );
              float y = static_cast< float >( -( u * s_yx ) + v - ( w * s_yz ) );
              float z = static_cast< float >( -( u * s_zx ) - ( v * s_zy ) + w );
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

  Matrix< float > Geometrics::Shear( double s_xy, double s_yx, double s_xz, double s_yz, double s_zx, double s_zy ) {
    try {
      Matrix< float > mat;
      MatrixOp::Identity( mat );
      return( Geometrics::Shear( mat, s_xy, s_yx, s_xz, s_yz, s_zx, s_zy ) );
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

  Matrix< float > Geometrics::Shear( Matrix< float > &source, double s_xy, double s_yx, double s_xz, double s_yz, 
                                     double s_zx, double s_zy ) {
    try {
      Matrix< float > mat( 4, 4 );
      mat( 0, 0 ) = 1.0;
      mat( 1, 0 ) = -s_xy;
      mat( 2, 0 ) = -s_xz;
      mat( 3, 0 ) = 0.0;
      mat( 0, 1 ) = -s_yx;
      mat( 1, 1 ) = 1.0;
      mat( 2, 1 ) = -s_yz;
      mat( 3, 1 ) = 0.0;
      mat( 0, 2 ) = -s_zx;
      mat( 1, 2 ) = -s_zy;
      mat( 2, 2 ) = 1.0;
      mat( 3, 2 ) = 0.0;
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

#ifdef BIAL_EXPLICIT_GeometricsShear

  template Image< int > Geometrics::Shear( const Image< int > &img, double s_xy, double s_yx, double s_xz, 
                                                 double s_yz, double s_zx, double s_zy );
  template Image< llint > Geometrics::Shear( const Image< llint > &img, double s_xy, double s_yx, double s_xz,
                                             double s_yz, double s_zx, double s_zy );
  template Image< float > Geometrics::Shear( const Image< float > &img, double s_xy, double s_yx, double s_xz,
                                             double s_yz, double s_zx, double s_zy );
  template Image< double > Geometrics::Shear( const Image< double > &img, double s_xy, double s_yx, double s_xz,
                                              double s_yz, double s_zx, double s_zy );
  template Image< int > Geometrics::Shear( const Image< int > &img, const PixelInterpolation &interpolation, 
                                           double s_xy, double s_yx, double s_xz, double s_yz, double s_zx, 
                                           double s_zy );
  template Image< llint > Geometrics::Shear( const Image< llint > &img, const PixelInterpolation &interpolation, 
                                             double s_xy, double s_yx, double s_xz, double s_yz, double s_zx, 
                                             double s_zy );
  template Image< float > Geometrics::Shear( const Image< float > &img, const PixelInterpolation &interpolation,
                                             double s_xy, double s_yx, double s_xz, double s_yz, double s_zx, 
                                             double s_zy );
  template Image< double > Geometrics::Shear( const Image< double > &img, const PixelInterpolation &interpolation,
                                              double s_xy, double s_yx, double s_xz, double s_yz, double s_zx, 
                                              double s_zy );

#endif

}

#endif

#endif
