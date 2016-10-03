/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Jun/21
 * @brief Set matrix to identity or computer distance from identity.
 */

#include "Common.hpp"

#ifndef BIALMATRIXIDENTITY_H
#define BIALMATRIXIDENTITY_H

#include "Vector.hpp"

namespace Bial {

  template< class D >
  class Matrix;

  namespace MatrixOp {

    /**
     * @date 2012/Jun/21
     * @return mat: Input matrix
     * @return Maximum difference from identity matrix.
     * @brief Computes the absolute maximum difference between the matrix and the identity.
     * @warning 'this' matrix must be square and 2D.
     */
    template< class D >
    D CompareToIdentity( const Matrix< D > &mat );

    /**
     * @date 2015/Jul/22
     * @param none
     * @return mat: Input matrix
     * @brief Resets to identity matrix.
     * @warning Matrix must be square.
     */
    template< class D >
    void Identity( Matrix< D > &mat );

  }

}

#include "MatrixIdentity.cpp"

#endif
