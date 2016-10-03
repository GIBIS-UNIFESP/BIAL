/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date: 2014/Jan/07
 * @brief Image rotation. Direct retation around coordinate system origin or corresponding affine transform.
 */

#ifndef BIALGEOMETRICSROTATE_C
#define BIALGEOMETRICSROTATE_C

#include "GeometricsRotate.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_GeometricsRotate )
#define BIAL_EXPLICIT_GeometricsRotate
#endif

#if defined ( BIAL_EXPLICIT_GeometricsRotate ) || ( BIAL_IMPLICIT_BIN )

#include "MatrixIdentity.hpp"

namespace Bial {
  
  template< class D >
  Image< D > Geometrics::Rotate( const Image< D > &img, float rad, size_t dms ) {
    try {
      LinearInterpolation interpolation;
      return( Geometrics::Rotate( img, interpolation, rad, dms ) );
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
  Image< D > Geometrics::Rotate( const Image< D > &img, const PixelInterpolation &interpolation, float rad, 
                                 size_t dms ) {
    try {
      COMMENT( "Verifying if 2D image rotation occurs around z axis.", 1 );
      if( ( img.Dims( ) == 2 ) && ( dms != 2 ) ) {
        std::string msg( BIAL_ERROR( "Trying to rotate 2D image around x or y axis." ) );
        throw( std::logic_error( msg ) );
      }
      Image< D > res( img );
      res.Set( 0 );
      if( img.Dims( ) == 2 ) {
        COMMENT( "2D image rotation.", 0 );
        for( size_t v = 0; v < img.size( 1 ); v++ ) { /* scan values in y */
          for( size_t u = 0; u < img.size( 0 ); u++ ) { /* scan values in x */
            float x = static_cast< float >( u * std::cos( rad ) - v * std::sin( rad ) );
            float y = static_cast< float >( u * std::sin( rad ) + v * std::cos( rad ) );
            if( ( x <= 0.0f ) || ( y <= 0.0f ) || ( x > img.size( 0 ) ) || ( y > img.size( 1 ) ) ) {
              res( u, v ) = 0;
            }
            else {
              COMMENT( "Interpolating.", 3 );
              res( u, v ) = interpolation( img, x, y );
            }
          }
        }
      }
      else {
        COMMENT( "3D image rotation.", 0 );
        if( dms == 0 ) {
          COMMENT( "Rotation around x axis.", 0 );
          for( size_t w = 0; w < img.size( 2 ); ++w ) { /* scan values in z */
            for( size_t v = 0; v < img.size( 1 ); ++v ) { /* scan values in y */
              float y = static_cast< float >( v * std::cos( rad ) - w * std::sin( rad ) );
              float z = static_cast< float >( v * std::sin( rad ) + w * std::cos( rad ) );
              COMMENT( "Verifies the position of the pixel if it is out of the source image range.", 3 );
              if( ( ( z >= 0 ) && ( z <= static_cast< float >( img.size( 2 ) ) - 1.0f ) ) &&
                  ( ( y >= 0 ) && ( y <= static_cast< float >( img.size( 1 ) ) - 1.0f ) ) ) {
                for( size_t u = 0; u < img.size( 0 ); ++u ) { /* scan values in x */
                  float x = static_cast< float >( u );
                  COMMENT( "Interpolating.", 3 );
                  res( u, v, w ) = interpolation( img, x, y, z );
                }
              }
            }
          }
        }
        else if( dms == 1 ) {
          COMMENT( "Rotation around y axis.", 0 );
          for( size_t w = 0; w < img.size( 2 ); ++w ) { /* scan values in z */
            for( size_t u = 0; u < img.size( 0 ); ++u ) { /* scan values in x */
              float x = static_cast< float >( u * std::cos( rad ) + w * std::sin( rad ) );
              float z = static_cast< float >( -u * std::sin( rad ) + w * std::cos( rad ) );
              COMMENT( "Verifies the position of the pixel if it is out of the source image range.", 3 );
              if( ( ( z >= 0 ) && ( z < static_cast< float >( img.size( 2 ) ) ) ) &&
                  ( ( x >= 0 ) && ( x < static_cast< float >( img.size( 0 ) ) ) ) ) {
                for( size_t v = 0; v < img.size( 1 ); ++v ) { /* scan values in y */
                  float y = static_cast< float >( v );
                  COMMENT( "Interpolating.", 3 );
                  res( u, v, w ) = interpolation( img, x, y, z );
                }
              }
            }
          }
        }
        else if( dms == 2 ) {
          COMMENT( "Rotation around z axis.", 0 );
          for( size_t v = 0; v < img.size( 1 ); v++ ) { /* scan values in y */
            for( size_t u = 0; u < img.size( 0 ); u++ ) { /* scan values in x */
              float x = static_cast< float >( u * std::cos( rad ) - v * std::sin( rad ) );
              float y = static_cast< float >( u * std::sin( rad ) + v * std::cos( rad ) );
              COMMENT( "Verifies the position of the pixel if it is out of the source image range.", 3 );
              if( ( ( x >= 0 ) && ( x < static_cast< float >( img.size( 0 ) ) ) ) &&
                  ( ( y >= 0 ) && ( y < static_cast< float >( img.size( 1 ) ) ) ) ) {
                for( size_t w = 0; w < img.size( 2 ); w++ ) { /* scan values in z */
                  float z = static_cast< float >( w );
                  COMMENT( "Interpolating.", 3 );
                  res( u, v, w ) = interpolation( img, x, y, z );
                }
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

  Matrix< float > Geometrics::Rotate( double rad, size_t dms ) {
    try {
      Matrix< float > mat( 4, 4 );
      MatrixOp::Identity( mat );
      return( Geometrics::Rotate( mat, rad, dms ) );
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

  Matrix< float > Geometrics::Rotate( Matrix< float > &source, double rad, size_t dms ) {
    try {
      Matrix< float > mat( 4, 4 );
      COMMENT( "Rotating around x axis.", 0 );
      if( dms == 0 ) {
        mat( 0, 0 ) = 1.0;
        mat( 1, 0 ) = 0.0;
        mat( 2, 0 ) = 0.0;
        mat( 3, 0 ) = 0.0;
        mat( 0, 1 ) = 0.0;
        mat( 1, 1 ) = std::cos( rad );
        mat( 2, 1 ) = -std::sin( rad );
        mat( 3, 1 ) = 0.0;
        mat( 0, 2 ) = 0.0;
        mat( 1, 2 ) = std::sin( rad );
        mat( 2, 2 ) = std::cos( rad );
        mat( 3, 2 ) = 0.0;
        mat( 0, 3 ) = 0.0;
        mat( 1, 3 ) = 0.0;
        mat( 2, 3 ) = 0.0;
        mat( 3, 3 ) = 1.0;
      }
      COMMENT( "Rotating around y axis.", 0 );
      if( dms == 1 ) {
        mat( 0, 0 ) = std::cos( rad );
        mat( 1, 0 ) = 0.0;
        mat( 2, 0 ) = std::sin( rad );
        mat( 3, 0 ) = 0.0;
        mat( 0, 1 ) = 0.0;
        mat( 1, 1 ) = 1.0;
        mat( 2, 1 ) = 0.0;
        mat( 3, 1 ) = 0.0;
        mat( 0, 2 ) = -std::sin( rad );
        mat( 1, 2 ) = 0.0;
        mat( 2, 2 ) = std::cos( rad );
        mat( 3, 2 ) = 0.0;
        mat( 0, 3 ) = 0.0;
        mat( 1, 3 ) = 0.0;
        mat( 2, 3 ) = 0.0;
        mat( 3, 3 ) = 1.0;
      }
      COMMENT( "Rotating around z axis.", 0 );
      if( dms == 2 ) {
        mat( 0, 0 ) = std::cos( rad );
        mat( 1, 0 ) = -std::sin( rad );
        mat( 2, 0 ) = 0.0;
        mat( 3, 0 ) = 0.0;
        mat( 0, 1 ) = std::sin( rad );
        mat( 1, 1 ) = std::cos( rad );
        mat( 2, 1 ) = 0.0;
        mat( 3, 1 ) = 0.0;
        mat( 0, 2 ) = 0.0;
        mat( 1, 2 ) = 0.0;
        mat( 2, 2 ) = 1.0;
        mat( 3, 2 ) = 0.0;
        mat( 0, 3 ) = 0.0;
        mat( 1, 3 ) = 0.0;
        mat( 2, 3 ) = 0.0;
        mat( 3, 3 ) = 1.0;
      }
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

#ifdef BIAL_EXPLICIT_GeometricsRotate

  template Image< int > Geometrics::Rotate( const Image< int > &img, float rad, size_t dms );
  template Image< llint > Geometrics::Rotate( const Image< llint > &img, float rad, size_t dms );
  template Image< float > Geometrics::Rotate( const Image< float > &img, float rad, size_t dms );
  template Image< double > Geometrics::Rotate( const Image< double > &img, float rad, size_t dms );
  template Image< int > Geometrics::Rotate( const Image< int > &img, const PixelInterpolation &interpolation, float rad,
                                            size_t dms );
  template Image< llint > Geometrics::Rotate( const Image< llint > &img, const PixelInterpolation &interpolation, 
                                              float rad, size_t dms );
  template Image< float > Geometrics::Rotate( const Image< float > &img, const PixelInterpolation &interpolation, 
                                              float rad, size_t dms );
  template Image< double > Geometrics::Rotate( const Image< double > &img, const PixelInterpolation &interpolation, 
                                               float rad, size_t dms );

#endif

}

#endif

#endif
