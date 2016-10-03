/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date: 2014/Jan/07
 * @brief Image shear. Direct shear or corresponding affine transform.
 */

#ifndef BIALGEOMETRICSSHEAR_H
#define BIALGEOMETRICSSHEAR_H

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
     * @param s_xy: Shearing in axis X with respect to axis Y.
     * @param s_yx: Shearing in axis Y with respect to axis X.
     * @param s_xz: Shearing in axis X with respect to axis Z.
     * @param s_yz: Shearing in axis Y with respect to axis Z.
     * @param s_zx: Shearing in axis Z with respect to axis X.
     * @param s_zy: Shearing in axis Z with respect to axis Y.
     * @return A Sheared image with linear interpolation.
     * @brief Shears an image.
     * @warning none.
     */
    template< class D >
    Image< D > Shear( const Image< D > &img, double s_xy, double s_yx, double s_xz = 0.0,
                      double s_yz = 0.0, double s_zx = 0.0, double s_zy = 0.0 );

    /**
     * @date 2014/Jun/08
     * @param img: Input image.
     * @param interpolation: Pixel interpolation to be used.
     * @param s_xy: Shearing in axis X with respect to axis Y.
     * @param s_yx: Shearing in axis Y with respect to axis X.
     * @param s_xz: Shearing in axis X with respect to axis Z.
     * @param s_yz: Shearing in axis Y with respect to axis Z.
     * @param s_zx: Shearing in axis Z with respect to axis X.
     * @param s_zy: Shearing in axis Z with respect to axis Y.
     * @return A Sheared image.
     * @brief Shears an image.
     * @warning none.
     */
    template< class D >
    Image< D > Shear( const Image< D > &img, const PixelInterpolation &interpolation, double s_xy, double s_yx,
                      double s_xz = 0.0, double s_yz = 0.0, double s_zx = 0.0, double s_zy = 0.0 );

    /**
     * @date 2014/Jun/08
     * @param s_xy: Shearing in axis X with respect to axis Y.
     * @param s_yx: Shearing in axis Y with respect to axis X.
     * @param s_xz: Shearing in axis X with respect to axis Z.
     * @param s_yz: Shearing in axis Y with respect to axis Z.
     * @param s_zx: Shearing in axis Z with respect to axis X.
     * @param s_zy: Shearing in axis Z with respect to axis Y.
     * @return The affine transform corresponding to a shear.
     * @brief Generates a transformation corresponding to a shear.
     * @warning none.
     */
    Matrix< float > Shear( double s_xy, double s_yx, double s_xz = 0.0, double s_yz = 0.0, double s_zx = 0.0,
                           double s_zy = 0.0 );

    /**
     * @date 2014/Jun/08
     * @param s_xy: Shearing in axis X with respect to axis Y.
     * @param s_yx: Shearing in axis Y with respect to axis X.
     * @param s_xz: Shearing in axis X with respect to axis Z.
     * @param s_yz: Shearing in axis Y with respect to axis Z.
     * @param s_zx: Shearing in axis Z with respect to axis X.
     * @param s_zy: Shearing in axis Z with respect to axis Y.
     * @return The affine transformation corresponding to a shear applyed over source matrix.
     * @brief Generates a composite transformation of the shear applyed over source matrix.
     * @warning none.
     */
    Matrix< float > Shear( Matrix< float > &source, double s_xy, double s_yx, double s_xz = 0.0, double s_yz = 0.0, 
                           double s_zx = 0.0, double s_zy = 0.0 );

  }

}

#include "GeometricsShear.cpp"

#endif
