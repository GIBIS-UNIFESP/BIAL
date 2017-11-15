/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Sep/19
 * Content: Euclidean distance function.
 */

#ifndef BIALEUCLIDEANDISTANCEFUNCTION_C
#define BIALEUCLIDEANDISTANCEFUNCTION_C

#include "EuclideanDistanceFunction.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_EuclideanDistanceFunction )
#define BIAL_EXPLICIT_EuclideanDistanceFunction
#endif

#if defined ( BIAL_EXPLICIT_EuclideanDistanceFunction ) || ( BIAL_IMPLICIT_BIN )

#include "Image.hpp"

namespace Bial {

  double EuclideanDistanceFunction::Distance( const char *src, const size_t dms ) {
    return( EuclideanDistanceFunction::Distance< char >( src, dms ) );
  }
  double EuclideanDistanceFunction::Distance( const uchar *src, const size_t dms ) {
    return( EuclideanDistanceFunction::Distance< uchar >( src, dms ) );
  }
  double EuclideanDistanceFunction::Distance( const short *src, const size_t dms ) {
    return( EuclideanDistanceFunction::Distance< short >( src, dms ) );
  }
  /*
   * double EuclideanDistanceFunction::Distance( const unsigned short *src, const size_t dms ) {
   *   return( EuclideanDistanceFunction::Distance< unsigned short >( src, dms ) );
   * }
   */
  double EuclideanDistanceFunction::Distance( const int *src, const size_t dms ) {
    return( EuclideanDistanceFunction::Distance< int >( src, dms ) );
  }
  double EuclideanDistanceFunction::Distance( const unsigned int *src, const size_t dms ) {
    return( EuclideanDistanceFunction::Distance< unsigned int >( src, dms ) );
  }
  /*
   * double EuclideanDistanceFunction::Distance( const long *src, const size_t dms ) {
   *   return( EuclideanDistanceFunction::Distance< long >( src, dms ) );
   * }
   */
  double EuclideanDistanceFunction::Distance( const ulint *src, const size_t dms ) {
    return( EuclideanDistanceFunction::Distance< ulint >( src, dms ) );
  }
  double EuclideanDistanceFunction::Distance( const llint *src, const size_t dms ) {
    return( EuclideanDistanceFunction::Distance< llint >( src, dms ) );
  }
  double EuclideanDistanceFunction::Distance( const ullint *src, const size_t dms ) {
    return( EuclideanDistanceFunction::Distance< ullint >( src, dms ) );
  }
  double EuclideanDistanceFunction::Distance( const float *src, const size_t dms ) {
    return( EuclideanDistanceFunction::Distance< float >( src, dms ) );
  }
  double EuclideanDistanceFunction::Distance( const double *src, const size_t dms ) {
    return( EuclideanDistanceFunction::Distance< double >( src, dms ) );
  }

  double EuclideanDistanceFunction::Distance( const char *src, const char *tgt, const size_t dms ) {
    return( EuclideanDistanceFunction::Distance< char >( src, tgt, dms ) );
  }
  double EuclideanDistanceFunction::Distance( const uchar *src, const uchar *tgt, const size_t dms ) {
    return( EuclideanDistanceFunction::Distance< uchar >( src, tgt, dms ) );
  }
  double EuclideanDistanceFunction::Distance( const short *src, const short *tgt, const size_t dms ) {
    return( EuclideanDistanceFunction::Distance< short >( src, tgt, dms ) );
  }
  /*
   * double EuclideanDistanceFunction::Distance( const unsigned short *src, const unsigned short *tgt,
   *                                                    const size_t dms ) {
   *   return( EuclideanDistanceFunction::Distance< unsigned short >( src, tgt, dms ) );
   * }
   */
  double EuclideanDistanceFunction::Distance( const int *src, const int *tgt, const size_t dms ) {
    return( EuclideanDistanceFunction::Distance< int >( src, tgt, dms ) );
  }
  double EuclideanDistanceFunction::Distance( const unsigned int *src, const unsigned int *tgt,
                                                     const size_t dms ) {
    return( EuclideanDistanceFunction::Distance< unsigned int >( src, tgt, dms ) );
  }
  /*
   * double EuclideanDistanceFunction::Distance( const long *src, const long *tgt, const size_t dms ) {
   *   return( EuclideanDistanceFunction::Distance< long >( src, tgt, dms ) );
   * }
   */
  double EuclideanDistanceFunction::Distance( const ulint *src, const ulint *tgt, const size_t dms ) {
    return( EuclideanDistanceFunction::Distance< ulint >( src, tgt, dms ) );
  }
  double EuclideanDistanceFunction::Distance( const llint *src, const llint *tgt, const size_t dms ) {
    return( EuclideanDistanceFunction::Distance< llint >( src, tgt, dms ) );
  }
  double EuclideanDistanceFunction::Distance( const ullint *src, const ullint *tgt, const size_t dms ) {
    return( EuclideanDistanceFunction::Distance< ullint >( src, tgt, dms ) );
  }
  double EuclideanDistanceFunction::Distance( const float *src, const float *tgt, const size_t dms ) {
    return( EuclideanDistanceFunction::Distance< float >( src, tgt, dms ) );
  }
  double EuclideanDistanceFunction::Distance( const double *src, const double *tgt, const size_t dms ) {
    return( EuclideanDistanceFunction::Distance< double >( src, tgt, dms ) );
  }

  /* Explicit instantiation of template functions. ------------------------------------------------------------------ */

#ifdef BIAL_EXPLICIT_EuclideanDistanceFunction


#endif

}

#endif

#endif
