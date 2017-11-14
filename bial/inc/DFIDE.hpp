/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Sep/19
 * Content: Distance function interface for generic algorithm implementation.
 */

#ifndef BIALDFIDE_H
#define BIALDFIDE_H

#include "Common.hpp"
#include "Vector.hpp"

namespace Bial {

  /* Static DistanceFunction used by bial. */
  enum class DistanceFunctionType : char {
    Euclidean,
    PreEuclidean,
    SquareEuclidean,
    CityBlock,
    ChessBoard
    /* Others */
  };

  class DFIDE {

  public:

    static DistanceFunctionType distance_function_type;
    static double ( *CharDistancePtr )( const char *src, const size_t dms );
    static double ( *CharDistancePtrTgt )( const char *src, const char *tgt, const size_t dms );
    static double ( *UCharDistancePtr )( const uchar *src, const size_t dms );
    static double ( *UCharDistancePtrTgt )( const uchar *src, const uchar *tgt, const size_t dms );
    static double ( *ShortDistancePtr )( const short *src, const size_t dms );
    static double ( *ShortDistancePtrTgt )( const short *src, const short *tgt, const size_t dms );
    /*
     * static double ( *UShortDistancePtr )( const unsigned short *src, const size_t dms );
     * static double ( *UShortDistancePtrTgt )( const unsigned short *src, const unsigned short *tgt, const size_t dms
     * );
     */
    static double ( *IntDistancePtr )( const int *src, const size_t dms );
    static double ( *IntDistancePtrTgt )( const int *src, const int *tgt, const size_t dms );
    static double ( *UIntDistancePtr )( const uint *src, const size_t dms );
    static double ( *UIntDistancePtrTgt )( const uint *src, const uint *tgt, const size_t dms );
    /*
     * static double ( *LongDistancePtr )( const long *src, const size_t dms );
     * static double ( *LongDistancePtrTgt )( const long *src, const long *tgt, const size_t dms );
     */
    static double ( *ULongDistancePtr )( const ulint *src, const size_t dms );
    static double ( *ULongDistancePtrTgt )( const ulint *src, const ulint *tgt, const size_t dms );
    static double ( *LLongDistancePtr )( const llint *src, const size_t dms );
    static double ( *LLongDistancePtrTgt )( const llint *src, const llint *tgt, const size_t dms );
    static double ( *ULLongDistancePtr )( const ullint *src, const size_t dms );
    static double ( *ULLongDistancePtrTgt )( const ullint *src, const ullint *tgt, const size_t dms );
    static double ( *FloatDistancePtr )( const float *src, const size_t dms );
    static double ( *FloatDistancePtrTgt )( const float *src, const float *tgt, const size_t dms );
    static double ( *DoubleDistancePtr )( const double *src, const size_t dms );
    static double ( *DoubleDistancePtrTgt )( const double *src, const double *tgt, const size_t dms );

    /**
     * @date 2013/Oct/12
     * @param src: Source countainer.
     * @param orig: Initial element in src container.
     * @param dms: Number of dimensions
     * @return Distace from src to origing.
     * @brief Compute the distances. These are the used IDE that can run in verbose mode for code debuging.
     * @warning none.
     */
    template< template< class D > class C, class D >
    static double Distance( const C< D > &src, const size_t orig, const size_t dms );
    template< template< class D > class C >
    static double Distance( const C< char > &src, const size_t orig, const size_t dms );
    template< template< class D > class C >
    static double Distance( const C< uchar > &src, const size_t orig, const size_t dms );
    template< template< class D > class C >
    static double Distance( const C< short > &src, const size_t orig, const size_t dms );
    /*
     * template< template< class D > class C >
     * static double Distance( const C< unsigned short > &src, const size_t orig, const size_t dms );
     */
    template< template< class D > class C >
    static double Distance( const C< int > &src, const size_t orig, const size_t dms );
    template< template< class D > class C >
    static double Distance( const C< uint > &src, const size_t orig, const size_t dms );
    /*
     * template< template< class D > class C >
     * static double Distance( const C< long > &src, const size_t orig, const size_t dms );
     */
    template< template< class D > class C >
    static double Distance( const C< ulint > &src, const size_t orig, const size_t dms );
    template< template< class D > class C >
    static double Distance( const C< llint > &src, const size_t orig, const size_t dms );
    template< template< class D > class C >
    static double Distance( const C< ullint > &src, const size_t orig, const size_t dms );
    template< template< class D > class C >
    static double Distance( const C< float > &src, const size_t orig, const size_t dms );
    template< template< class D > class C >
    static double Distance( const C< double > &src, const size_t orig, const size_t dms );

    /**
     * @date 2013/Oct/12
     * @param src: Source countainer.
     * @param src_orig: Initial element in src container.
     * @param tgt_orig: Initial element in tgt container.
     * @param dms: Number of dimensions
     * @return Distace from src to tgt.
     * @brief Compute the distances. These are the used IDE that can run in verbose mode for code debuging.
     * @warning none.
     */
    template< template< class D > class C, class D >
    static double Distance( const C< D > &src, const size_t src_orig, const size_t tgt_orig, const size_t dms );
    template< template< class D > class C >
    static double Distance( const C< char > &src, const size_t src_orig, const size_t tgt_orig, const size_t dms );
    template< template< class D > class C >
    static double Distance( const C< uchar > &src, const size_t src_orig, const size_t tgt_orig, const size_t dms );
    template< template< class D > class C >
    static double Distance( const C< short > &src, const size_t src_orig, const size_t tgt_orig, const size_t dms );
    /*
     * template< template< class D > class C >
     * static double Distance( const C< unsigned short > &src, const size_t src_orig, const size_t tgt_orig,
     *                         const size_t dms );
     */
    template< template< class D > class C >
    static double Distance( const C< int > &src, const size_t src_orig, const size_t tgt_orig, const size_t dms );
    template< template< class D > class C >
    static double Distance( const C< uint > &src, const size_t src_orig, const size_t tgt_orig, const size_t dms );
    /*
     * template< template< class D > class C >
     * static double Distance( const C< long > &src, const size_t src_orig, const size_t tgt_orig, const size_t dms );
     */
    template< template< class D > class C >
    static double Distance( const C< ulint > &src, const size_t src_orig, const size_t tgt_orig, const size_t dms );
    template< template< class D > class C >
    static double Distance( const C< llint > &src, const size_t src_orig, const size_t tgt_orig, const size_t dms );
    template< template< class D > class C >
    static double Distance( const C< ullint > &src, const size_t src_orig, const size_t tgt_orig, const size_t dms );
    template< template< class D > class C >
    static double Distance( const C< float > &src, const size_t src_orig, const size_t tgt_orig, const size_t dms );
    template< template< class D > class C >
    static double Distance( const C< double > &src, const size_t src_orig, const size_t tgt_orig, const size_t dms );

    /**
     * @date 2013/Oct/12
     * @param src: Source countainer.
     * @param tgt: Target countainer.
     * @param src_orig: Initial element in src container.
     * @param tgt_orig: Initial element in tgt container.
     * @param dms: Number of dimensions
     * @return Distace from src to tgt.
     * @brief Compute the distances. These are the used IDE that can run in verbose mode for code debuging.
     * @warning none.
     */
    template< template< class D > class C, class D >
    static double Distance( const C< D > &src, const C< D > &tgt, const size_t src_orig, const size_t tgt_orig,
                            const size_t dms );
    template< template< class D > class C >
    static double Distance( const C< char > &src, const C< char > &tgt, const size_t src_orig, const size_t tgt_orig,
                            const size_t dms );
    template< template< class D > class C >
    static double Distance( const C< uchar > &src, const C< uchar > &tgt, const size_t src_orig, const size_t tgt_orig,
                            const size_t dms );
    template< template< class D > class C >
    static double Distance( const C< short > &src, const C< short > &tgt, const size_t src_orig, const size_t tgt_orig,
                            const size_t dms );
    /*
     * template< template< class D > class C >
     * static double Distance( const C< unsigned short > &src, const C< unsigned short > &tgt, const size_t src_orig,
     *                         const size_t tgt_orig, const size_t dms );
     */
    template< template< class D > class C >
    static double Distance( const C< int > &src, const C< int > &tgt, const size_t src_orig, const size_t tgt_orig,
                            const size_t dms );
    template< template< class D > class C >
    static double Distance( const C< uint > &src, const C< uint > &tgt, const size_t src_orig, const size_t tgt_orig,
                            const size_t dms );
    /*
     * template< template< class D > class C >
     * static double Distance( const C< long > &src, const C< long > &tgt, const size_t src_orig,
     *                         const size_t tgt_orig, const size_t dms );
     */
    template< template< class D > class C >
    static double Distance( const C< ulint > &src, const C< ulint > &tgt, const size_t src_orig, const size_t tgt_orig,
                            const size_t dms );
    template< template< class D > class C >
    static double Distance( const C< llint > &src, const C< llint > &tgt, const size_t src_orig, const size_t tgt_orig,
                            const size_t dms );
    template< template< class D > class C >
    static double Distance( const C< ullint > &src, const C< ullint > &tgt, const size_t src_orig,
                            const size_t tgt_orig, const size_t dms );
    template< template< class D > class C >
    static double Distance( const C< float > &src, const C< float > &tgt, const size_t src_orig, const size_t tgt_orig,
                            const size_t dms );
    template< template< class D > class C >
    static double Distance( const C< double > &src, const C< double > &tgt, const size_t src_orig,
                            const size_t tgt_orig, const size_t dms );

    /**
     * @date 2015/Mar/10
     * @param maximum_distance: Maximum accessed distance for pre computed euclidean distance.
     * @return none.
     * @brief Functions to change current distance function.
     * @warning src and tgt must have the same size. Not checked!
     */
    static void SetEuclideanDistanceFunction( );
    static void SetPreEuclideanDistanceFunction( size_t maximum_distance );
    static void SetSquareEuclideanDistanceFunction( );
    static void SetCityBlockDistanceFunction( );
    static void SetChessBoardDistanceFunction( );
  };

}

#include "DFIDE.cpp"

#endif
