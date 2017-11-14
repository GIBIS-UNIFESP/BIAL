/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Jul/05
 * @brief Adjacency relation with square format.
 * <br> Future add-on's: none.
 */

#include "Adjacency.hpp"
#include "Common.hpp"
#include "Vector.hpp"

#ifndef BIALADJACENCYSQUARE_H
#define BIALADJACENCYSQUARE_H

/* Declarations -------------------------------------------------------------------------------------------------------- */

namespace Bial {

  template< class D >
  class Image;
  template< class D >
  class Matrix;
  union Color;

  namespace AdjacencyType {

    /**
     * @date 2014/Jan/19
     * @param src_displacement: source displacement of the adjacency.
     * @param tgt_displacement: target displacement of the adjacency.
     * @return non.
     * @brief Basic Constructor. Build a n-dimensional box from src_displacement to tgt_displacement. 
     * @warning none.
     */
    Adjacency Square( const Vector< size_t > &src_displacement, const Vector< size_t > &tgt_displacement );
    
    /**
     * @date 2013/Nov/19
     * @param mat: A matrix.
     * @return none.
     * @brief Basic Constructor. Build an adjacency based on a matrix. Non-zero elements are set as adjacents a
     * coefficients are matrix values. Adjacency centralized in dim / 2.
     * @warning Uninitialized kernel is created.
     */
    template< class D > 
    Adjacency Square( const Matrix< D > &mat );

    /**
     * @date 2013/Nov/19
     * @param img: An image.
     * @return none.
     * @brief Basic Constructor. Build an adjacency based on an image. Non-zero elements are set as adjacents a
     * coefficients are image values. Adjacency centralized in dim / 2.
     * @warning Uninitialized kernel is created.
     */
    template< class D > 
    Adjacency Square( const Image< D > &img );
    Adjacency Square( const Image< Color > &img );

  }

}

#include "AdjacencySquare.cpp"

#endif
