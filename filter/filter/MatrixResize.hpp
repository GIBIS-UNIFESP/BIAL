/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Jun/21
 * @brief Operations to get sub-matrix.
 */

#include "Common.hpp"

#ifndef BIALMATRIXRESIZE_H
#define BIALMATRIXRESIZE_H

#include "Matrix.hpp"

namespace Bial {

  namespace MatrixOp {

    /**
     * @date 2013/Oct/22
     * @param new_min: Vector containing the coord. of the begin of the sub_matrix dimensions
     * @param new_max: Vector containing the coord. of the end   of the sub_matrix dimensions
     * @return A submatrix with given coordinates.
     * @brief Returns a submatrix from this matrix.
     *              If this matrix has more dimensions than the input, than all elements of these
     *              Dimensions are included in the submatrix.
     * @warning The input vectors can not have more coordinates than this matrix.
     */
    template< class D >
    Matrix< D > Resize( const Matrix< D > &mat, const Vector< size_t > &new_min, const Vector< size_t > &new_max );

    /**
     * @brief
     * Returns a submatrix from this matrix. The coordinates string must be given in a similar way as used by MatLab, or in some other variations.
     * @code
     * Examples:
     * img( "1, 1:2 )" = Submatrix with column 1 and rows 1 and 2.
     * img( "1:, :2 )" = Submatrix with columns 1 to img.size( 0 ) and rows 0 to 2.
     * img( "1:, : )" = Submatrix with columns 1 to img.size( 0 ) and all rows.
     * @endcode
     * If this matrix has more dimensions than the input string, than all elements of these dimensions
     * are included in the submatrix.
     * @date 2013/Oct/03
     * @param coordinates: Coordinates in this matrix to be used to generate the submatrix.
     * @return A submatrix with given coordinates.
     * @warning The input string can not have more coordinates than this matrix.
     */
    template< class D >
    Matrix< D > Resize( const Matrix< D > &mat, const std::string &coordinates );

    /**
     * @date 2012/Oct/03
     * @param coordinates: Coordinates in this matrix to be used to generate the submatrix.
     * @return 2D Matrix with converted coordinates from the input string. First dimension referes to source
     * matrix coordinates and second dimension has lower and higher limits of each coordinate.
     * @brief Returns vectors with minimum and maximum coordinates from this matrix. The coordinates string must
     * be given in a similar fashion as used by MatLab, or in some other variations.
     * Examples: img( "1, 1:2 )" = coordinates with column 1 and rows 1 and 2.
     * img( "1:, :2 )" = coordinates with columns 1 to img.size( 0 ) and rows 0 to 2.
     * img( "1:, : )" = coordinates with columns 1 to img.size( 0 ) and all rows.
     * If this matrix has more dimensions than the input string, than all elements of these dimensions
     * are included in the resulting vector.
     * @warning The input string can not have more coordinates than this matrix.
     */
    template< class D >
    Matrix< size_t > Coordinates( const Matrix< D > &mat, const std::string &coordinates );
    
  }

}

#endif
