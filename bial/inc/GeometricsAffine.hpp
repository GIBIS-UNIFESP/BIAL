/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date: 2014/Jan/07
 * @brief Affine transformation.
 */

#ifndef BIALGEOMETRICSAFFINE_H
#define BIALGEOMETRICSAFFINE_H

#include "Common.hpp"
#include "PixelInterpolation.hpp"
#include "Matrix.hpp"
#include "Vector.hpp"

namespace Bial {

  template< class D > class Image;

  namespace Geometrics {

    /**
     * @date 2014/Jun/08
     * @param img: Input image.
     * @param transform: Affine transformation matrix.
     * @return A transformated image.
     * @brief Applies affine transformation to an image, using the matrix of transformation.
     * @warning none.
     */
    template< class D >
    Image< D > AffineTransform( const Image< D > &img, const Matrix< float > &transform );

    /**
     * @date 2014/Jun/08
     * @param img: Input image.
     * @param transform: Affine transformation matrix.
     * @param interpolation: Pixel interpolation to be used.
     * @return A transformated image.
     * @brief Applies affine transformation to an image, using the matrix of transformation.
     * @warning none.
     */
    template< class D >
    Image< D > AffineTransform( const Image< D > &img, const Matrix< float > &transform, 
                                const PixelInterpolation &interpolation );
    
  }

}

#include "GeometricsAffine.cpp"

#endif
