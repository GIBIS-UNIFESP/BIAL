/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Sep/19
 * Content: Euclidean distance function.
 */

#ifndef BIALSQUAREEUCLIDEANDISTANCEFUNCTION_C
#define BIALSQUAREEUCLIDEANDISTANCEFUNCTION_C

#include "SquareEuclideanDistanceFunction.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_SquareEuclideanDistanceFunction )
#define BIAL_EXPLICIT_SquareEuclideanDistanceFunction
#endif

#if defined ( BIAL_EXPLICIT_SquareEuclideanDistanceFunction ) || ( BIAL_IMPLICIT_BIN )

#include "Image.hpp"

namespace Bial {

  /* SquareEuclideanDistanceFunction **********************************************************************************
  **/

  double SquareEuclideanDistanceFunction::Distance( const char *src, const size_t dms ) {
    return( SquareEuclideanDistanceFunction::Distance< char >( src, dms ) );
  }
  double SquareEuclideanDistanceFunction::Distance( const uchar *src, const size_t dms ) {
    return( SquareEuclideanDistanceFunction::Distance< uchar >( src, dms ) );
  }
  double SquareEuclideanDistanceFunction::Distance( const short *src, const size_t dms ) {
    return( SquareEuclideanDistanceFunction::Distance< short >( src, dms ) );
  }
  /*
   * double SquareEuclideanDistanceFunction::Distance( const unsigned short *src, const size_t dms ) {
   *   return( SquareEuclideanDistanceFunction::Distance< unsigned short >( src, dms ) );
   * }
   */
  double SquareEuclideanDistanceFunction::Distance( const int *src, const size_t dms ) {
    return( SquareEuclideanDistanceFunction::Distance< int >( src, dms ) );
  }
  double SquareEuclideanDistanceFunction::Distance( const unsigned int *src, const size_t dms ) {
    return( SquareEuclideanDistanceFunction::Distance< unsigned int >( src, dms ) );
  }
  /*
   * double SquareEuclideanDistanceFunction::Distance( const long *src, const size_t dms ) {
   *   return( SquareEuclideanDistanceFunction::Distance< long >( src, dms ) );
   * }
   */
  double SquareEuclideanDistanceFunction::Distance( const ulint *src, const size_t dms ) {
    return( SquareEuclideanDistanceFunction::Distance< ulint >( src, dms ) );
  }
  double SquareEuclideanDistanceFunction::Distance( const llint *src, const size_t dms ) {
    return( SquareEuclideanDistanceFunction::Distance< llint >( src, dms ) );
  }
  double SquareEuclideanDistanceFunction::Distance( const ullint *src, const size_t dms ) {
    return( SquareEuclideanDistanceFunction::Distance< ullint >( src, dms ) );
  }
  double SquareEuclideanDistanceFunction::Distance( const float *src, const size_t dms ) {
    return( SquareEuclideanDistanceFunction::Distance< float >( src, dms ) );
  }
  double SquareEuclideanDistanceFunction::Distance( const double *src, const size_t dms ) {
    return( SquareEuclideanDistanceFunction::Distance< double >( src, dms ) );
  }

  double SquareEuclideanDistanceFunction::Distance( const char *src, const char *tgt, const size_t dms ) {
    return( SquareEuclideanDistanceFunction::Distance< char >( src, tgt, dms ) );
  }
  double SquareEuclideanDistanceFunction::Distance( const uchar *src, const uchar *tgt, const size_t dms ) {
    return( SquareEuclideanDistanceFunction::Distance< uchar >( src, tgt, dms ) );
  }
  double SquareEuclideanDistanceFunction::Distance( const short *src, const short *tgt, const size_t dms ) {
    return( SquareEuclideanDistanceFunction::Distance< short >( src, tgt, dms ) );
  }
  /*
   * double SquareEuclideanDistanceFunction::Distance( const unsigned short *src, const unsigned short *tgt,
   *                                                    const size_t dms ) {
   *   return( SquareEuclideanDistanceFunction::Distance< unsigned short >( src, tgt, dms ) );
   * }
   */
  double SquareEuclideanDistanceFunction::Distance( const int *src, const int *tgt, const size_t dms ) {
    return( SquareEuclideanDistanceFunction::Distance< int >( src, tgt, dms ) );
  }
  double SquareEuclideanDistanceFunction::Distance( const unsigned int *src,
                                                           const unsigned int *tgt,
                                                           const size_t dms ) {
    return( SquareEuclideanDistanceFunction::Distance< unsigned int >( src, tgt, dms ) );
  }
  /*
   * double SquareEuclideanDistanceFunction::Distance( const long *src, const long *tgt, const size_t dms ) {
   *   return( SquareEuclideanDistanceFunction::Distance< long >( src, tgt, dms ) );
   * }
   */
  double SquareEuclideanDistanceFunction::Distance( const ulint *src, const ulint *tgt, const size_t dms ) {
    return( SquareEuclideanDistanceFunction::Distance< ulint >( src, tgt, dms ) );
  }
  double SquareEuclideanDistanceFunction::Distance( const llint *src, const llint *tgt, const size_t dms ) {
    return( SquareEuclideanDistanceFunction::Distance< llint >( src, tgt, dms ) );
  }
  double SquareEuclideanDistanceFunction::Distance( const ullint *src, const ullint *tgt, const size_t dms ) {
    return( SquareEuclideanDistanceFunction::Distance< ullint >( src, tgt, dms ) );
  }
  double SquareEuclideanDistanceFunction::Distance( const float *src, const float *tgt, const size_t dms ) {
    return( SquareEuclideanDistanceFunction::Distance< float >( src, tgt, dms ) );
  }
  double SquareEuclideanDistanceFunction::Distance( const double *src, const double *tgt, const size_t dms ) {
    return( SquareEuclideanDistanceFunction::Distance< double >( src, tgt, dms ) );
  }

#ifdef BIAL_EXPLICIT_SquareEuclideanDistanceFunction

#endif

}

#endif

#endif
