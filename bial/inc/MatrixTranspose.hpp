/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Jun/21
 * @brief Compute the matrix transpose.
 */

#include "Common.hpp"

#ifndef BIALMATRIXTRANSPOSE_H
#define BIALMATRIXTRANSPOSE_H

namespace Bial {

  template< class D >
  class Matrix;

  namespace MatrixOp {

    /**
     * @date 2012/Jun/21
     * @param none.
     * @return none.
     * @brief Compute the transposed of 'this' matrix, overwriting data Vector.
     * @warning 'this' matrix must be square and 2D.
     */
    template< class D >
    void Transpose( Matrix< D > &mat );

    /**
     * @date 2012/Jun/21
     * @param none.
     * @return The transposed of 'this' matrix.
     * @brief Compute and return the transposed of 'this' matrix.
     * @warning 'this' matrix must be square and 2D.
     */
    template< class D >
    Matrix< D > Transpose( const Matrix< D > &mat );

  }

}

#include "MatrixTranspose.cpp"

#endif
