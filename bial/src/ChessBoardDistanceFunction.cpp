/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Sep/19
 * Content: ChessBoard distance function.
 */

#ifndef BIALCHESSBOARDDISTANCEFUNCTION_C
#define BIALCHESSBOARDDISTANCEFUNCTION_C

#include "ChessBoardDistanceFunction.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_ChessBoardDistanceFunction )
#define BIAL_EXPLICIT_ChessBoardDistanceFunction
#endif

#if defined ( BIAL_EXPLICIT_ChessBoardDistanceFunction ) || ( BIAL_IMPLICIT_BIN )

#include "Feature.hpp"
#include "Image.hpp"

namespace Bial {

  double ChessBoardDistanceFunction::Distance( const char *src, const size_t dms ) {
    return( ChessBoardDistanceFunction::Distance< char >( src, dms ) );
  }
  double ChessBoardDistanceFunction::Distance( const uchar *src, const size_t dms ) {
    return( ChessBoardDistanceFunction::Distance< uchar >( src, dms ) );
  }
  double ChessBoardDistanceFunction::Distance( const short *src, const size_t dms ) {
    return( ChessBoardDistanceFunction::Distance< short >( src, dms ) );
  }
  /*
   * double ChessBoardDistanceFunction::Distance( const unsigned short *src, const size_t dms ) {
   *   return( ChessBoardDistanceFunction::Distance< unsigned short >( src, dms ) );
   * }
   */
  double ChessBoardDistanceFunction::Distance( const int *src, const size_t dms ) {
    return( ChessBoardDistanceFunction::Distance< int >( src, dms ) );
  }
  double ChessBoardDistanceFunction::Distance( const unsigned int *src, const size_t dms ) {
    return( ChessBoardDistanceFunction::Distance< unsigned int >( src, dms ) );
  }
  /*
   * double ChessBoardDistanceFunction::Distance( const long *src, const size_t dms ) {
   *   return( ChessBoardDistanceFunction::Distance< long >( src, dms ) );
   * }
   */
  double ChessBoardDistanceFunction::Distance( const ulint *src, const size_t dms ) {
    return( ChessBoardDistanceFunction::Distance< ulint >( src, dms ) );
  }
  double ChessBoardDistanceFunction::Distance( const llint *src, const size_t dms ) {
    return( ChessBoardDistanceFunction::Distance< llint >( src, dms ) );
  }
  double ChessBoardDistanceFunction::Distance( const ullint *src, const size_t dms ) {
    return( ChessBoardDistanceFunction::Distance< ullint >( src, dms ) );
  }
  double ChessBoardDistanceFunction::Distance( const float *src, const size_t dms ) {
    return( ChessBoardDistanceFunction::Distance< float >( src, dms ) );
  }
  double ChessBoardDistanceFunction::Distance( const double *src, const size_t dms ) {
    return( ChessBoardDistanceFunction::Distance< double >( src, dms ) );
  }

  double ChessBoardDistanceFunction::Distance( const char *src, const char *tgt, const size_t dms ) {
    return( ChessBoardDistanceFunction::Distance< char >( src, tgt, dms ) );
  }
  double ChessBoardDistanceFunction::Distance( const uchar *src, const uchar *tgt, const size_t dms ) {
    return( ChessBoardDistanceFunction::Distance< uchar >( src, tgt, dms ) );
  }
  double ChessBoardDistanceFunction::Distance( const short *src, const short *tgt, const size_t dms ) {
    return( ChessBoardDistanceFunction::Distance< short >( src, tgt, dms ) );
  }
  /*
   * double ChessBoardDistanceFunction::Distance( const unsigned short *src, const unsigned short *tgt,
   *                                                    const size_t dms ) {
   *   return( ChessBoardDistanceFunction::Distance< unsigned short >( src, tgt, dms ) );
   * }
   */
  double ChessBoardDistanceFunction::Distance( const int *src, const int *tgt, const size_t dms ) {
    return( ChessBoardDistanceFunction::Distance< int >( src, tgt, dms ) );
  }
  double ChessBoardDistanceFunction::Distance( const unsigned int *src,
                                                      const unsigned int *tgt,
                                                      const size_t dms ) {
    return( ChessBoardDistanceFunction::Distance< unsigned int >( src, tgt, dms ) );
  }
  /*
   * double ChessBoardDistanceFunction::Distance( const long *src, const long *tgt, const size_t dms ) {
   *   return( ChessBoardDistanceFunction::Distance< long >( src, tgt, dms ) );
   * }
   */
  double ChessBoardDistanceFunction::Distance( const ulint *src, const ulint *tgt, const size_t dms ) {
    return( ChessBoardDistanceFunction::Distance< ulint >( src, tgt, dms ) );
  }
  double ChessBoardDistanceFunction::Distance( const llint *src, const llint *tgt, const size_t dms ) {
    return( ChessBoardDistanceFunction::Distance< llint >( src, tgt, dms ) );
  }
  double ChessBoardDistanceFunction::Distance( const ullint *src, const ullint *tgt, const size_t dms ) {
    return( ChessBoardDistanceFunction::Distance< ullint >( src, tgt, dms ) );
  }
  double ChessBoardDistanceFunction::Distance( const float *src, const float *tgt, const size_t dms ) {
    return( ChessBoardDistanceFunction::Distance< float >( src, tgt, dms ) );
  }
  double ChessBoardDistanceFunction::Distance( const double *src, const double *tgt, const size_t dms ) {
    return( ChessBoardDistanceFunction::Distance< double >( src, tgt, dms ) );
  }

  /* Explicit instantiation of template functions. ------------------------------------------------------------------ */

#ifdef BIAL_EXPLICIT_ChessBoardDistanceFunction

#endif

}

#endif

#endif
