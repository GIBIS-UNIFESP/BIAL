/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Jul/05
 * @brief Kernel relation with square format.
 * <br> Future add-on's: none.
 */

#include "Kernel.hpp"
#include "Common.hpp"
#include "Vector.hpp"

#ifndef BIALKERNELSQUARE_H
#define BIALKERNELSQUARE_H

/* Declarations -------------------------------------------------------------------------------------------------------- */

namespace Bial {

  template< class D >
  class Image;
  template< class D >
  class Matrix;
  union Color;

  namespace KernelType {
    
    /**
     * @date 2013/Nov/19
     * @param mat: A matrix.
     * @return none.
     * @brief Basic Constructor. Build an adjacency based on a matrix. Non-zero elements are set as adjacents a
     * coefficients are matrix values. Kernel centralized in dim / 2.
     * @warning Uninitialized kernel is created.
     */
    template< class D > 
    Kernel Square( const Matrix< D > &mat );

    /**
     * @date 2013/Nov/19
     * @param img: An image.
     * @return none.
     * @brief Basic Constructor. Build an adjacency based on an image. Non-zero elements are set as adjacents a
     * coefficients are image values. Kernel centralized in dim / 2.
     * @warning Uninitialized kernel is created.
     */
    template< class D > 
    Kernel Square( const Image< D > &img );
    Kernel Square( const Image< Color > &img );

  }

}

#include "KernelSquare.cpp"

#endif
