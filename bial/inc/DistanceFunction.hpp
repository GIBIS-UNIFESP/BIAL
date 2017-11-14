/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Sep/19
 * Content: Distance function abstract class to compute the distance between array elements.
 */

#ifndef BIALDISTANCEFUNCTION_H
#define BIALDISTANCEFUNCTION_H

#include "Common.hpp"
#include "Vector.hpp"

namespace Bial {

  template< class DERIVED >
  class DistanceFunction {

  private:

    /**
     * @date 2015/Mar/10
     * @param none.
     * @return none.
     * @brief Foo function just to set this class as pure virtual.
     * @warning none.
     */
    virtual void Foo( ) const = 0;

  public:

    /**
     * @date 2013/Jul/07
     * @param none.
     * @return none.
     * @brief Virtual destructor required for virtual classes.
     * @warning none.
     */
    virtual ~DistanceFunction( ) {
    }

    /**
     * @date 2013/Jul/07
     * @param src: Source array.
     * @param dms: Number of dimensions
     * @return Distace from origing to src.
     * @brief Returns the distance from origing to src.
     * @warning none.
     */
    template< class D >
    static double Distance( const D *src, const size_t dms ) {
      return( DERIVED::template Distance< D >( src, dms ) );
    }
    static double Distance( const char *src, const size_t dms ) {
      return( DERIVED::Distance( src, dms ) );
    }
    static double Distance( const uchar *src, const size_t dms ) {
      return( DERIVED::Distance( src, dms ) );
    }
    static double Distance( const short *src, const size_t dms ) {
      return( DERIVED::Distance( src, dms ) );
    }
    /*
     * static double Distance( const unsigned short *src, const size_t dms ) {
     *   return( DERIVED::Distance( src, dms ) );
     * }
     */
    static double Distance( const int *src, const size_t dms ) {
      return( DERIVED::Distance( src, dms ) );
    }
    static double Distance( const uint *src, const size_t dms ) {
      return( DERIVED::Distance( src, dms ) );
    }
    /*
     * static double Distance( const long *src, const size_t dms ) {
     *   return( DERIVED::Distance( src, dms ) );
     * }
     */
    static double Distance( const ulint *src, const size_t dms ) {
      return( DERIVED::Distance( src, dms ) );
    }
    static double Distance( const llint *src, const size_t dms ) {
      return( DERIVED::Distance( src, dms ) );
    }
    static double Distance( const ullint *src, const size_t dms ) {
      return( DERIVED::Distance( src, dms ) );
    }
    static double Distance( const float *src, const size_t dms ) {
      return( DERIVED::Distance( src, dms ) );
    }
    static double Distance( const double *src, const size_t dms ) {
      return( DERIVED::Distance( src, dms ) );
    }

    /**
     * @date 2013/Jul/07
     * @param src: Source array.
     * @param tgt: Target array.
     * @param dms: Number of dimensions
     * @return Distace from src to tgt.
     * @brief Returns the distance from src to tgt.
     * @warning src and tgt must have the same size. Not checked!
     */
    template< class D >
    static double Distance( const D *src, const D *tgt, const size_t dms ) {
      return( DERIVED::template Distance< D >( src, tgt, dms ) );
    }
    static double Distance( const char *src, const char *tgt, const size_t dms ) {
      return( DERIVED::Distance( src, tgt, dms ) );
    }
    static double Distance( const uchar *src, const uchar *tgt, const size_t dms ) {
      return( DERIVED::Distance( src, tgt, dms ) );
    }
    static double Distance( const short *src, const short *tgt, const size_t dms ) {
      return( DERIVED::Distance( src, tgt, dms ) );
    }
    /*
     * static double Distance( const unsigned short *src, const unsigned short *tgt, const size_t dms ) {
     *   return( DERIVED::Distance( src, tgt, dms ) );
     * }
     */
    static double Distance( const int *src, const int *tgt, const size_t dms ) {
      return( DERIVED::Distance( src, tgt, dms ) );
    }
    static double Distance( const uint *src, const uint *tgt, const size_t dms ) {
      return( DERIVED::Distance( src, tgt, dms ) );
    }
    /*
     * static double Distance( const long *src, const long *tgt, const size_t dms ) {
     *   return( DERIVED::Distance( src, tgt, dms ) );
     * }
     */
    static double Distance( const ulint *src, const ulint *tgt, const size_t dms ) {
      return( DERIVED::Distance( src, tgt, dms ) );
    }
    static double Distance( const llint *src, const llint *tgt, const size_t dms ) {
      return( DERIVED::Distance( src, tgt, dms ) );
    }
    static double Distance( const ullint *src, const ullint *tgt, const size_t dms ) {
      return( DERIVED::Distance( src, tgt, dms ) );
    }
    static double Distance( const float *src, const float *tgt, const size_t dms ) {
      return( DERIVED::Distance( src, tgt, dms ) );
    }
    static double Distance( const double *src, const double *tgt, const size_t dms ) {
      return( DERIVED::Distance( src, tgt, dms ) );
    }
  };

}

#endif
