/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Sep/19
 * Content: Distance function abstract class to compute the distance between array elements.
 */

#ifndef BIALCITYBLOCKDISTANCEFUNCTION_C
#define BIALCITYBLOCKDISTANCEFUNCTION_C

#include "CityBlockDistanceFunction.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_CityBlockDistanceFunction )
#define BIAL_EXPLICIT_CityBlockDistanceFunction
#endif

#if defined ( BIAL_EXPLICIT_CityBlockDistanceFunction ) || ( BIAL_IMPLICIT_BIN )

#include "Image.hpp"

namespace Bial {

  double CityBlockDistanceFunction::Distance( const char *src, const size_t dms ) {
    return( CityBlockDistanceFunction::Distance< char >( src, dms ) );
  }
  double CityBlockDistanceFunction::Distance( const uchar *src, const size_t dms ) {
    return( CityBlockDistanceFunction::Distance< uchar >( src, dms ) );
  }
  double CityBlockDistanceFunction::Distance( const short *src, const size_t dms ) {
    return( CityBlockDistanceFunction::Distance< short >( src, dms ) );
  }
  /*
   * double CityBlockDistanceFunction::Distance( const unsigned short *src, const size_t dms ) {
   *   return( CityBlockDistanceFunction::Distance< unsigned short >( src, dms ) );
   * }
   */
  double CityBlockDistanceFunction::Distance( const int *src, const size_t dms ) {
    return( CityBlockDistanceFunction::Distance< int >( src, dms ) );
  }
  double CityBlockDistanceFunction::Distance( const unsigned int *src, const size_t dms ) {
    return( CityBlockDistanceFunction::Distance< unsigned int >( src, dms ) );
  }
  /*
   * double CityBlockDistanceFunction::Distance( const long *src, const size_t dms ) {
   *   return( CityBlockDistanceFunction::Distance< long >( src, dms ) );
   * }
   */
  double CityBlockDistanceFunction::Distance( const ulint *src, const size_t dms ) {
    return( CityBlockDistanceFunction::Distance< ulint >( src, dms ) );
  }
  double CityBlockDistanceFunction::Distance( const llint *src, const size_t dms ) {
    return( CityBlockDistanceFunction::Distance< llint >( src, dms ) );
  }
  double CityBlockDistanceFunction::Distance( const ullint *src, const size_t dms ) {
    return( CityBlockDistanceFunction::Distance< ullint >( src, dms ) );
  }
  double CityBlockDistanceFunction::Distance( const float *src, const size_t dms ) {
    return( CityBlockDistanceFunction::Distance< float >( src, dms ) );
  }
  double CityBlockDistanceFunction::Distance( const double *src, const size_t dms ) {
    return( CityBlockDistanceFunction::Distance< double >( src, dms ) );
  }

  double CityBlockDistanceFunction::Distance( const char *src, const char *tgt, const size_t dms ) {
    return( CityBlockDistanceFunction::Distance< char >( src, tgt, dms ) );
  }
  double CityBlockDistanceFunction::Distance( const uchar *src, const uchar *tgt, const size_t dms ) {
    return( CityBlockDistanceFunction::Distance< uchar >( src, tgt, dms ) );
  }
  double CityBlockDistanceFunction::Distance( const short *src, const short *tgt, const size_t dms ) {
    return( CityBlockDistanceFunction::Distance< short >( src, tgt, dms ) );
  }
  /*
   * double CityBlockDistanceFunction::Distance( const unsigned short *src, const unsigned short *tgt,
   *                                                    const size_t dms ) {
   *   return( CityBlockDistanceFunction::Distance< unsigned short >( src, tgt, dms ) );
   * }
   */
  double CityBlockDistanceFunction::Distance( const int *src, const int *tgt, const size_t dms ) {
    return( CityBlockDistanceFunction::Distance< int >( src, tgt, dms ) );
  }
  double CityBlockDistanceFunction::Distance( const unsigned int *src, const unsigned int *tgt,
                                                     const size_t dms ) {
    return( CityBlockDistanceFunction::Distance< unsigned int >( src, tgt, dms ) );
  }
  /*
   * double CityBlockDistanceFunction::Distance( const long *src, const long *tgt, const size_t dms ) {
   *   return( CityBlockDistanceFunction::Distance< long >( src, tgt, dms ) );
   * }
   */
  double CityBlockDistanceFunction::Distance( const ulint *src, const ulint *tgt, const size_t dms ) {
    return( CityBlockDistanceFunction::Distance< ulint >( src, tgt, dms ) );
  }
  double CityBlockDistanceFunction::Distance( const llint *src, const llint *tgt, const size_t dms ) {
    return( CityBlockDistanceFunction::Distance< llint >( src, tgt, dms ) );
  }
  double CityBlockDistanceFunction::Distance( const ullint *src, const ullint *tgt, const size_t dms ) {
    return( CityBlockDistanceFunction::Distance< ullint >( src, tgt, dms ) );
  }
  double CityBlockDistanceFunction::Distance( const float *src, const float *tgt, const size_t dms ) {
    return( CityBlockDistanceFunction::Distance< float >( src, tgt, dms ) );
  }
  double CityBlockDistanceFunction::Distance( const double *src, const double *tgt, const size_t dms ) {
    return( CityBlockDistanceFunction::Distance< double >( src, tgt, dms ) );
  }

  /* Explicit instantiation of template functions. ------------------------------------------------------------------ */

#ifdef BIAL_EXPLICIT_CityBlockDistanceFunction

#endif

}

#endif

#endif
