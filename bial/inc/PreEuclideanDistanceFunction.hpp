/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Sep/19
 * Content: Euclidean distance function with square distances pre computed.
 */

#ifndef BIALPREEUCLIDEANDISTANCEFUNCTION_H
#define BIALPREEUCLIDEANDISTANCEFUNCTION_H

#include "Common.hpp"
#include "DistanceFunction.hpp"
#include "Vector.hpp"

namespace Bial {

  class PreEuclideanDistanceFunction : public DistanceFunction< PreEuclideanDistanceFunction > {

  private:

    static Vector< ullint > square;

  public:

    /**
     * @date 2013/Jun/21
     * @param maximum_distance: Maximum distance between elements
     * of a feature vector. i.e. max - min.
     * @return none.
     * @brief Basic Constructor.
     * @warning This class should be used only with integer types. Behavior for floating point types is unknown.
     */
    static void SetPreEuclideanDistanceFunction( size_t maximum_distance );

    /**
     * @date 2015/Mar/10
     * @param none.
     * @return none.
     * @brief Foo function just to implement pure virtual function from parent class.
     * @warning none.
     */
    void Foo( ) const {
    }

    /**
     * @date 2013/Jun/21
     * @param src: Source array.
     * @param dms: Number of dimensions
     * @return Distace from origing to src.
     * @brief Returns the distance from origing to src.
     * @warning none.
     */
    template< class D >
    static double Distance( const D *src, const size_t dms );
    static double Distance( const char *src, const size_t dms );
    static double Distance( const uchar *src, const size_t dms );
    static double Distance( const short *src, const size_t dms );
    /* static double Distance( const unsigned short *src, const size_t dms ); */
    static double Distance( const int *src, const size_t dms );
    static double Distance( const uint *src, const size_t dms );
    /* static double Distance( const long *src, const size_t dms ); */
    static double Distance( const ulint *src, const size_t dms );
    static double Distance( const llint *src, const size_t dms );
    static double Distance( const ullint *src, const size_t dms );
    static double Distance( const float *src, const size_t dms );
    static double Distance( const double *src, const size_t dms );

    /**
     * @date 2013/Jun/21
     * @param src: Source array.
     * @param tgt: Target array.
     * @param dms: Number of dimensions
     * @return Distace from src to tgt.
     * @brief Returns the distance from src to tgt.
     * @warning src and tgt must have the same size. Not checked!
     */
    template< class D >
    static double Distance( const D *src, const D *tgt, const size_t dms );
    static double Distance( const char *src, const char *tgt, const size_t dms );
    static double Distance( const uchar *src, const uchar *tgt, const size_t dms );
    static double Distance( const short *src, const short *tgt, const size_t dms );
    /* static double Distance( const unsigned short *src, const unsigned short *tgt, const size_t dms ); */
    static double Distance( const int *src, const int *tgt, const size_t dms );
    static double Distance( const uint *src, const uint *tgt, const size_t dms );
    /* static double Distance( const long *src, const long *tgt, const size_t dms ); */
    static double Distance( const ulint *src, const ulint *tgt, const size_t dms );
    static double Distance( const llint *src, const llint *tgt, const size_t dms );
    static double Distance( const ullint *src, const ullint *tgt, const size_t dms );
    static double Distance( const float *src, const float *tgt, const size_t dms );
    static double Distance( const double *src, const double *tgt, const size_t dms );
  };

}

/* Implementation of template functions that are not explicitly instantiated ---------------------------------------- */

namespace Bial {


  template< class D >
  double PreEuclideanDistanceFunction::Distance( const D *src, const size_t dms ) {
    double dist = 0.0;
    for( size_t ftr = 0; ftr < dms; ++ftr ) {
      size_t src_ftr = ( src[ ftr ] > 0 ) ? src[ ftr ] : -src[ ftr ];
      dist += square( src_ftr );
    }
    return( std::sqrt( dist ) );
  }

  template< class D >
  double PreEuclideanDistanceFunction::Distance( const D *src, const D *tgt, const size_t dms ) {
    double dist = 0.0;
    for( size_t ftr = 0; ftr < dms; ++ftr ) {
      size_t diff_ftr = ( src[ ftr ] > tgt[ ftr ] ) ? src[ ftr ] - tgt[ ftr ] : tgt[ ftr ] - src[ ftr ];
      dist += square( diff_ftr );
    }
    return( std::sqrt( dist ) );
  }

}

#include "PreEuclideanDistanceFunction.cpp"

#endif
