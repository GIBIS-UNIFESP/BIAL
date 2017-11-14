/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date: 2014/Jan/07
 * @brief Class for interpolation of pixels.
 */

#ifndef BIALPIXELINTERPOLATION_C
#define BIALPIXELINTERPOLATION_C

#include "PixelInterpolation.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_PixelInterpolation )
#define BIAL_EXPLICIT_PixelInterpolation
#endif

#if defined ( BIAL_EXPLICIT_PixelInterpolation ) || ( BIAL_IMPLICIT_BIN )

namespace Bial {

  /* NearestInterpolation ------------------------------------------------------------------------------------------ **/

  int NearestInterpolation::operator()( const Image< int > &img, float x, float y ) const {
    return( NearestInterpolation::operator()< int >( img, x, y ) );
  }

  llint NearestInterpolation::operator()( const Image< llint > &img, float x, float y ) const {
    return( NearestInterpolation::operator()< llint >( img, x, y ) );
  }

  float NearestInterpolation::operator()( const Image< float > &img, float x, float y ) const {
    return( NearestInterpolation::operator()< float >( img, x, y ) );
  }

  double NearestInterpolation::operator()( const Image< double > &img, float x, float y ) const {
    return( NearestInterpolation::operator()< double >( img, x, y ) );
  }

  int NearestInterpolation::operator()( const Image< int > &img, float x, float y, float z ) const {
    return( NearestInterpolation::operator()< int >( img, x, y, z ) );
  }

  llint NearestInterpolation::operator()( const Image< llint > &img, float x, float y, float z ) const {
    return( NearestInterpolation::operator()< llint >( img, x, y, z ) );
  }

  float NearestInterpolation::operator()( const Image< float > &img, float x, float y, float z ) const {
    return( NearestInterpolation::operator()< float >( img, x, y, z ) );
  }

  double NearestInterpolation::operator()( const Image< double > &img, float x, float y, float z ) const {
    return( NearestInterpolation::operator()< double >( img, x, y, z ) );
  }

  /* LinearInterpolation ------------------------------------------------------------------------------------------- **/

  int LinearInterpolation::operator()( const Image< int > &img, float x, float y ) const {
    return( LinearInterpolation::operator()< int >( img, x, y ) );
  }

  llint LinearInterpolation::operator()( const Image< llint > &img, float x, float y ) const {
    return( LinearInterpolation::operator()< llint >( img, x, y ) );
  }

  float LinearInterpolation::operator()( const Image< float > &img, float x, float y ) const {
    return( LinearInterpolation::operator()< float >( img, x, y ) );
  }

  double LinearInterpolation::operator()( const Image< double > &img, float x, float y ) const {
    return( LinearInterpolation::operator()< double >( img, x, y ) );
  }

  int LinearInterpolation::operator()( const Image< int > &img, float x, float y, float z ) const {
    return( LinearInterpolation::operator()< int >( img, x, y, z ) );
  }

  llint LinearInterpolation::operator()( const Image< llint > &img, float x, float y, float z ) const {
    return( LinearInterpolation::operator()< llint >( img, x, y, z ) );
  }

  float LinearInterpolation::operator()( const Image< float > &img, float x, float y, float z ) const {
    return( LinearInterpolation::operator()< float >( img, x, y, z ) );
  }

  double LinearInterpolation::operator()( const Image< double > &img, float x, float y, float z ) const {
    return( LinearInterpolation::operator()< double >( img, x, y, z ) );
  }

}

#endif

#endif
