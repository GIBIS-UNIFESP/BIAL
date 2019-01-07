/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Jun/21
 * @brief Functions to compute matrix inverse.
 */

#include "Common.hpp"

#ifndef BIALMATRIXINVERSE_H
#define BIALMATRIXINVERSE_H

#include "Vector.hpp"

namespace Bial {

  namespace MatrixOp {

    /**
     * @date 2012/Oct/23
     * @param mat: input matrix.
     * @return Cofactors matrix.
     * @brief Compute and return cofactors matrix.
     * @warning 'this' matrix must be square and 2D.
     */
    template< class D >
    Matrix< D > Cofactor( const Matrix< D > &mat );

    /**
     * @date 2012/Jun/21
     * @param mat: input matrix.
     * @return none.
     * @brief Computes LU decomposition based inverse of 'this' matrix, overwriting data Vector.
     * @warning 'this' matrix must be square and 2D.
     */
    template< class D >
    void Inverse( Matrix< D > &mat );

    /**
     * @date 2012/Jun/21
     * @param mat: input matrix.
     * @return The inverse of this matrix.
     * @brief Compute and return LU decomposition based inverse of 'this' matrix.
     * @warning 'this' matrix must be square and 2D.
     */
    template< class D >
    Matrix< D > Inverse( const Matrix< D > &mat );

    /**
     * @date 2012/Jun/21
     * @param mat: input matrix.
     * @return none.
     * @brief Computes eigen Vector based inverse of 'this' matrix, overwriting data Vector.
     * @warning 'this' matrix must be square and 2D.
     */
    template< class D >
    void Inverse2( Matrix< D > &mat );

    /**
     * @date 2012/Jun/21
     * @param mat: input matrix.
     * @return The inverse of this matrix.
     * @brief Compute and return eigen Vector based inverse of 'this' matrix.
     * @warning 'this' matrix must be square and 2D.
     */
    template< class D >
    Matrix< D > Inverse2( const Matrix< D > &mat );

    /**
     * @date 2012/Oct/23
     * @param mat: input matrix.
     * @return none.
     * @brief Computes the inverse based on the cofactor matrix of 'this' matrix.
     * @warning 'this' matrix must be square and 2D.
     */
    template< class D >
    void Inverse3( Matrix< D > &mat );

    /**
     * @date 2012/Oct/23
     * @param mat: input matrix.
     * @return The inverse of this matrix.
     * @brief Computes and returns the inverse based on the cofactor matrix of 'this' matrix.
     * @warning 'this' matrix must be square and 2D.
     */
    template< class D >
    Matrix< D > Inverse3( const Matrix< D > &mat );

  }
}

#endif
