/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Jun/21
 * @brief Functions to compute polar matrix.
 */

#include "Common.hpp"

#ifndef BIALMATRIXPOLAR_H
#define BIALMATRIXPOLAR_H

#include "Vector.hpp"

namespace Bial {

  namespace MatrixOp {

    /**
     * @date 2012/Oct/23
     * @param none.
     * @return Minimum norm among all rows.
     * @brief Computes and returns the minimum norm (sum or elements) among all matrix rows.
     * @warning 'this' matrix must be 2D.
     */
    template< class D >
    double MinimumRowNorm( const Matrix< D > &mat );

    /**
     * @date 2012/Oct/23
     * @param none.
     * @return Minimum norm among all columns.
     * @brief Computes and returns the minimum norm (sum or elements) among all matrix colunms.
     * @warning 'this' matrix must be 2D.
     */
    template< class D >
    double MinimumColNorm( const Matrix< D > &mat );

    /**
     * @date 2012/Oct/23
     * @param none.
     * @return none or Polar matrix.
     * @brief Computes and returns polar matrix.
     * @warning 'this' matrix must be 3x3 2D.
     */
    template< class D >
    void Polar( Matrix< D > &mat );
    template< class D >
    Matrix< D > Polar( const Matrix< D > &mat );

  }

}

#endif
