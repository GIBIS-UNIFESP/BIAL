/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date: 2014/Jan/07
 * @brief Image scaling. Direct scaling or corresponding affine transform.
 */

#ifndef BIALGEOMETRICSSCALE_C
#define BIALGEOMETRICSSCALE_C

#include "GeometricsScale.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_GeometricsScale )
#define BIAL_EXPLICIT_GeometricsScale
#endif

#if defined ( BIAL_EXPLICIT_GeometricsScale ) || ( BIAL_IMPLICIT_BIN )

#include "MatrixIdentity.hpp"

namespace Bial {

  template< class D >
  Image< D > Geometrics::Scale( const Image< D > &img, double factor, bool resize ) {
    try {
      LinearInterpolation interpolation;
      return( Geometrics::Scale( img, interpolation, factor, resize ) );
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
  Image< D > Geometrics::Scale( const Image< D > &img, const PixelInterpolation &interpolation, double factor,
                                bool resize ) {
    try {
      if( factor <= 0.0001 ) {
        std::string msg( BIAL_ERROR( "Zero pixel dimension in output image." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Returning input image for trivial transformation.", 0 );
      if( factor == 1.0 ) {
        return( Image< D >( img ) );
      }
      if( img.Dims( ) == 2 ) {
        return( Geometrics::Scale( img, interpolation, factor, factor, 1.0, resize ) );
      }
      return( Geometrics::Scale( img, interpolation, factor, factor, factor, resize ) );
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
  Image< D > Geometrics::Scale( const Image< D > &img, double fx, double fy, double fz, bool resize ) {
    try {
      LinearInterpolation interpolation;
      return( Geometrics::Scale( img, interpolation, fx, fy, fz, resize ) );
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
  Image< D > Geometrics::Scale( const Image< D > &img, const PixelInterpolation &interpolation, double fx, double fy,
                                double fz, bool resize ) {
    try {
      size_t dims = img.Dims( );
      COMMENT( "Checking if asked to change z scale for 2D image.", 1 );
      if( ( dims == 2 ) && ( fz != 1.0 ) ) {
        std::string msg( BIAL_ERROR( "Changing scale in z axis for 2D image." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Cheking if factors are positive.", 1 );
      if( ( fx <= 0.0001 ) || ( fy <= 0.0001 ) || ( fz <= 0.0001 ) ) {
        std::string msg( BIAL_ERROR( "Zero pixel dimension in output image." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Returning input image for trivial transformation.", 0 );
      if( ( fx == 1.0 ) && ( fy == 1.0 ) && ( fz == 1.0 ) ) {
        return( Image< D >( img ) );
      }
      COMMENT( "Creating resultant image.", 0 );
      Vector< size_t > spc_dim( img.Dim( ) );
      Vector< float > pixel_size = img.PixelSize( );
      pixel_size( 0 ) /= fx;
      pixel_size( 1 ) /= fy;
      if( dims == 3 ) {
        pixel_size( 2 ) /= fz;
      }
      COMMENT( "Checking if resizing is required.", 0 );
      if( resize ) {
        spc_dim( 0 ) = std::ceil( spc_dim( 0 ) * fx );
        spc_dim( 1 ) = std::ceil( spc_dim( 1 ) * fy );
        spc_dim( 2 ) = std::ceil( spc_dim( 2 ) * fz );
      }
      COMMENT( "spc_dim: " << spc_dim << ", pixel_size: " << pixel_size, 2 );
      Image< D > res( spc_dim, pixel_size );
      COMMENT( "Setting zsize in order to make algorithm generic for 2D and 3D images with no substantital delay.", 0 );
      if( dims == 2 ) {
        for( size_t v = 0; v < res.size( 1 ); v++ ) { /* scan values in y */
          for( size_t u = 0; u < res.size( 0 ); u++ ) { /* scan values in x */
            float x = u / fx;
            float y = v / fy;
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
        for( size_t w = 0; w < res.size( 2 ); w++ ) { /* scan values in z */
          for( size_t v = 0; v < res.size( 1 ); v++ ) { /* scan values in y */
            for( size_t u = 0; u < res.size( 0 ); u++ ) { /* scan values in x */
              float x = u / fx;
              float y = v / fy;
              float z = w / fz;
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

  template< class D >
  Image< D > Geometrics::Scale( const Image< D > &img, const Vector< size_t > &dims ) {
    try {
      LinearInterpolation interpolation;
      return( Geometrics::Scale( img, interpolation, dims ) );
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
  Image< D > Geometrics::Scale( const Image< D > &img, const PixelInterpolation &interpolation,
                                const Vector< size_t > &dims ) {
    try {
      COMMENT( "Checking image and dimensions.", 1 );
      if( ( img.Dims( ) == 3 ) && ( 3 != dims.size( ) ) ) {
        std::string msg( BIAL_ERROR( "Input and output number of dimensions do not match. Given: Image dimensions: " +
                                     std::to_string( img.Dims( ) ) + ", output dimensions: " +
                                     std::to_string( dims.size( ) ) ) );
        throw( std::logic_error( msg ) );
      }
      if( dims( 0 ) == 0 ) {
        std::string msg( BIAL_ERROR( "Zero pixel dimension in output image." ) );
        throw( std::logic_error( msg ) );
      }
      float factor = dims( 0 ) / img.size( 0 );
      COMMENT( "resizing factor: " << factor, 1 );
      COMMENT( "Computing new image dimensions.", 0 );
      size_t dimensions = img.Dims( );
      Vector< float > pxl_size( img.PixelSize( ) );
      for( size_t dms = 0; dms < dimensions; ++dms )
        pxl_size( dms ) /= factor;
      Image< D > res( dims, pxl_size );
      COMMENT( "Converting image.", 0 );
      Vector< size_t > max_dim( res.Dim( ) );
      for( size_t w = 0; w < res.size( 2 ); w++ ) { /* scan values in z */
        for( size_t v = 0; v < res.size( 1 ); v++ ) { /* scan values in y */
          for( size_t u = 0; u < res.size( 0 ); u++ ) { /* scan values in x */
            COMMENT( "Computing source and target coordinates.", 3 );
            float x = std::min( u / factor, img.size( 0 ) - 1.0f );
            float y = std::min( v / factor, img.size( 1 ) - 1.0f );
            float z = std::min( w / factor, img.size( 2 ) - 1.0f );
            COMMENT( "Interpolating.", 3 );
            res( u, v, w ) = interpolation( img, x, y, z );
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

  Matrix< float > Geometrics::Scale( double fx, double fy, double fz ) {
    try {
      Matrix< float > mat( 4, 4 );
      MatrixOp::Identity( mat );
      return( Geometrics::Scale( mat, fx, fy, fz ) );
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

  Matrix< float > Geometrics::Scale( Matrix< float > &source, double fx, double fy, double fz ) {
    try {
      Matrix< float > mat( 4, 4 );
      mat( 0, 0 ) = fx;
      mat( 1, 0 ) = 0.0;
      mat( 2, 0 ) = 0.0;
      mat( 3, 0 ) = 0.0;
      mat( 0, 1 ) = 0.0;
      mat( 1, 1 ) = fy;
      mat( 2, 1 ) = 0.0;
      mat( 3, 1 ) = 0.0;
      mat( 0, 2 ) = 0.0;
      mat( 1, 2 ) = 0.0;
      mat( 2, 2 ) = fz;
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

#ifdef BIAL_EXPLICIT_GeometricsScale

  template Image< int > Geometrics::Scale( const Image< int > &img, double factor, bool resize );
  template Image< int > Geometrics::Scale( const Image< int > &img, double fx, double fy, double fz, 
                                                 bool resize );
  template Image< int > Geometrics::Scale( const Image< int > &img, const Vector< size_t > &dims );
  template Image< llint > Geometrics::Scale( const Image< llint > &img, double factor, bool resize );
  template Image< llint > Geometrics::Scale( const Image< llint > &img, double fx, double fy, double fz, 
                                                   bool resize );
  template Image< llint > Geometrics::Scale( const Image< llint > &img, const Vector< size_t > &dims );
  template Image< float > Geometrics::Scale( const Image< float > &img, double factor, bool resize );
  template Image< float > Geometrics::Scale( const Image< float > &img, double fx, double fy, double fz, 
                                                   bool resize );
  template Image< float > Geometrics::Scale( const Image< float > &img, const Vector< size_t > &dims );
  template Image< double > Geometrics::Scale( const Image< double > &img, double factor, bool resize );
  template Image< double > Geometrics::Scale( const Image< double > &img, double fx, double fy, double fz,
                                              bool resize );
  template Image< double > Geometrics::Scale( const Image< double > &img, const Vector< size_t > &dims );
  template Image< int > Geometrics::Scale( const Image< int > &img, const PixelInterpolation &interpolation, 
                                           double factor, bool resize );
  template Image< int > Geometrics::Scale( const Image< int > &img, const PixelInterpolation &interpolation, 
                                           double fx, double fy, double fz, bool resize );
  template Image< int > Geometrics::Scale( const Image< int > &img, const PixelInterpolation &interpolation, 
                                           const Vector< size_t > &dims );
  template Image< llint > Geometrics::Scale( const Image< llint > &img, const PixelInterpolation &interpolation, 
                                             double factor, bool resize );
  template Image< llint > Geometrics::Scale( const Image< llint > &img, const PixelInterpolation &interpolation, 
                                             double fx, double fy, double fz, bool resize );
  template Image< llint > Geometrics::Scale( const Image< llint > &img, const PixelInterpolation &interpolation, 
                                             const Vector< size_t > &dims );
  template Image< float > Geometrics::Scale( const Image< float > &img, const PixelInterpolation &interpolation, 
                                             double factor, bool resize );
  template Image< float > Geometrics::Scale( const Image< float > &img, const PixelInterpolation &interpolation, 
                                             double fx, double fy, double fz, bool resize );
  template Image< float > Geometrics::Scale( const Image< float > &img, const PixelInterpolation &interpolation, 
                                             const Vector< size_t > &dims );
  template Image< double > Geometrics::Scale( const Image< double > &img, const PixelInterpolation &interpolation, 
                                              double factor, bool resize );
  template Image< double > Geometrics::Scale( const Image< double > &img, const PixelInterpolation &interpolation, 
                                              double fx, double fy, double fz, bool resize );
  template Image< double > Geometrics::Scale( const Image< double > &img, const PixelInterpolation &interpolation,
                                              const Vector< size_t > &dims );

#endif

}

#endif

#endif
