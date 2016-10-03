/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Sep/19
 * Content: Euclidean distance function with square distances pre computed.
 */

#ifndef BIALPREEUCLIDEANDISTANCEFUNCTION_C
#define BIALPREEUCLIDEANDISTANCEFUNCTION_C

#include "PreEuclideanDistanceFunction.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_PreEuclideanDistanceFunction )
#define BIAL_EXPLICIT_PreEuclideanDistanceFunction
#endif

#if defined ( BIAL_EXPLICIT_PreEuclideanDistanceFunction ) || ( BIAL_IMPLICIT_BIN )

#include "Image.hpp"

namespace Bial {

  void PreEuclideanDistanceFunction::SetPreEuclideanDistanceFunction( size_t maximum_amplitude ) {
    square = Vector< ullint >( maximum_amplitude );
    for( size_t idx = 1; idx < square.size( ); ++idx ) {
      square( idx ) = idx * idx;
    }
  }

  double PreEuclideanDistanceFunction::Distance( const char *src, const size_t dms ) {
    return( PreEuclideanDistanceFunction::Distance< char >( src, dms ) );
  }
  double PreEuclideanDistanceFunction::Distance( const uchar *src, const size_t dms ) {
    return( PreEuclideanDistanceFunction::Distance< uchar >( src, dms ) );
  }
  double PreEuclideanDistanceFunction::Distance( const short *src, const size_t dms ) {
    return( PreEuclideanDistanceFunction::Distance< short >( src, dms ) );
  }
  /*
   * double PreEuclideanDistanceFunction::Distance( const unsigned short *src, const size_t dms ) {
   *   return( PreEuclideanDistanceFunction::Distance< unsigned short >( src, dms ) );
   * }
   */
  double PreEuclideanDistanceFunction::Distance( const int *src, const size_t dms ) {
    return( PreEuclideanDistanceFunction::Distance< int >( src, dms ) );
  }
  double PreEuclideanDistanceFunction::Distance( const unsigned int *src, const size_t dms ) {
    return( PreEuclideanDistanceFunction::Distance< unsigned int >( src, dms ) );
  }
  /*
   * double PreEuclideanDistanceFunction::Distance( const long *src, const size_t dms ) {
   *   return( PreEuclideanDistanceFunction::Distance< long >( src, dms ) );
   * }
   */
  double PreEuclideanDistanceFunction::Distance( const ulint *src, const size_t dms ) {
    return( PreEuclideanDistanceFunction::Distance< ulint >( src, dms ) );
  }
  double PreEuclideanDistanceFunction::Distance( const llint *src, const size_t dms ) {
    return( PreEuclideanDistanceFunction::Distance< llint >( src, dms ) );
  }
  double PreEuclideanDistanceFunction::Distance( const ullint *src, const size_t dms ) {
    return( PreEuclideanDistanceFunction::Distance< ullint >( src, dms ) );
  }
  double PreEuclideanDistanceFunction::Distance( const float *src, const size_t dms ) {
    return( PreEuclideanDistanceFunction::Distance< float >( src, dms ) );
  }
  double PreEuclideanDistanceFunction::Distance( const double *src, const size_t dms ) {
    return( PreEuclideanDistanceFunction::Distance< double >( src, dms ) );
  }

  double PreEuclideanDistanceFunction::Distance( const char *src, const char *tgt, const size_t dms ) {
    return( PreEuclideanDistanceFunction::Distance< char >( src, tgt, dms ) );
  }
  double PreEuclideanDistanceFunction::Distance( const uchar *src, const uchar *tgt, const size_t dms ) {
    return( PreEuclideanDistanceFunction::Distance< uchar >( src, tgt, dms ) );
  }
  double PreEuclideanDistanceFunction::Distance( const short *src, const short *tgt, const size_t dms ) {
    return( PreEuclideanDistanceFunction::Distance< short >( src, tgt, dms ) );
  }
  /*
   * double PreEuclideanDistanceFunction::Distance( const unsigned short *src, const unsigned short *tgt,
   *                                                    const size_t dms ) {
   *   return( PreEuclideanDistanceFunction::Distance< unsigned short >( src, tgt, dms ) );
   * }
   */
  double PreEuclideanDistanceFunction::Distance( const int *src, const int *tgt, const size_t dms ) {
    return( PreEuclideanDistanceFunction::Distance< int >( src, tgt, dms ) );
  }
  double PreEuclideanDistanceFunction::Distance( const unsigned int *src,
                                                        const unsigned int *tgt,
                                                        const size_t dms ) {
    return( PreEuclideanDistanceFunction::Distance< unsigned int >( src, tgt, dms ) );
  }
  /*
   * double PreEuclideanDistanceFunction::Distance( const long *src, const long *tgt, const size_t dms ) {
   *   return( PreEuclideanDistanceFunction::Distance< long >( src, tgt, dms ) );
   * }
   */
  double PreEuclideanDistanceFunction::Distance( const ulint *src, const ulint *tgt, const size_t dms ) {
    return( PreEuclideanDistanceFunction::Distance< ulint >( src, tgt, dms ) );
  }
  double PreEuclideanDistanceFunction::Distance( const llint *src, const llint *tgt, const size_t dms ) {
    return( PreEuclideanDistanceFunction::Distance< llint >( src, tgt, dms ) );
  }
  double PreEuclideanDistanceFunction::Distance( const ullint *src, const ullint *tgt, const size_t dms ) {
    return( PreEuclideanDistanceFunction::Distance< ullint >( src, tgt, dms ) );
  }
  double PreEuclideanDistanceFunction::Distance( const float *src, const float *tgt, const size_t dms ) {
    return( PreEuclideanDistanceFunction::Distance< float >( src, tgt, dms ) );
  }
  double PreEuclideanDistanceFunction::Distance( const double *src, const double *tgt, const size_t dms ) {
    return( PreEuclideanDistanceFunction::Distance< double >( src, tgt, dms ) );
  }

  /* Implementing PreEuclideanDistance square variable. */
  Vector< ullint > PreEuclideanDistanceFunction::square;

  /* Explicit instantiation of template functions. ------------------------------------------------------------------ */

#ifdef BIAL_EXPLICIT_PreEuclideanDistanceFunction

#endif

}

#endif

#endif
