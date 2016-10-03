/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date: 2014/Jan/07
 * @brief Image translation. Direct translation or corresponding affine transform.
 */

#ifndef BIALGEOMETRICSTRANSLATE_H
#define BIALGEOMETRICSTRANSLATE_H

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
     * @param dx: Displacement in X
     * @param dy: Displacement in Y
     * @param dz: Displacement in Z
     * @return A translated image with linear interpolation.
     * @brief Translates a image.
     * @warning none.
     */
    template< class D >
    Image< D > Translate( const Image< D > &img, double dx, double dy, double dz = 0 );

    /**
     * @date 2014/Jun/08
     * @param img: Input image.
     * @param interpolation: Pixel interpolation to be used.
     * @param dx: Displacement in X
     * @param dy: Displacement in Y
     * @param dz: Displacement in Z
     * @return A translated image.
     * @brief Translates a image.
     * @warning none.
     */
    template< class D >
    Image< D > Translate( const Image< D > &img, const PixelInterpolation &interpolation, double dx, double dy,
                          double dz = 0 );

    /**
     * @date 2014/Jun/08
     * @param source: Initial affine transformation over which translation is applyed.
     * @param dx: Displacement in X
     * @param dy: Displacement in Y
     * @param dz: Displacement in Z
     * @return The affine transformation corresponding to a translation.
     * @brief Generates a transformation corresponding to a translation.
     * @warning none.
     */
    Matrix< float > Translate( double dx, double dy, double dz = 0 );

    /**
     * @date 2014/Jun/08
     * @param source: Initial affine transformation over which translation is applyed.
     * @param dx: Displacement in X
     * @param dy: Displacement in Y
     * @param dz: Displacement in Z
     * @return The affine transformation corresponding to a translation applyed over source matrix.
     * @brief Generates a composite transformation of the translation applyed over source matrix.
     * @warning none.
     */
    Matrix< float > Translate( Matrix< float > &source, double dx, double dy, double dz = 0 );

  }

}

#include "GeometricsTranslate.cpp"

#endif
