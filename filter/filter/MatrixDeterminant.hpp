/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Jun/21
 * @brief Compute matrix determinant.
 */

#include "Common.hpp"

#ifndef BIALMATRIXDETERMINANT_H
#define BIALMATRIXDETERMINANT_H

#include "Vector.hpp"

namespace Bial {

  namespace MatrixOp {

    /**
     * @date 2012/Oct/23
     * @param mat: Input matrix.
     * @return Determinant of this matrix.
     * @brief Compute and return the determinant of this matrix.
     * @warning 'this' matrix must be square and 2D.
     */
    template< class D >
    double Determinant( const Matrix< D > &mat );

  }

}

#endif
