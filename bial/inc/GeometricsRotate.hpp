/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date: 2014/Jan/07
 * @brief Image rotation. Direct retation around coordinate system origin or corresponding affine transform.
 */

#ifndef BIALGEOMETRICSROTATE_H
#define BIALGEOMETRICSROTATE_H

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
     * @param rad: Rotation in radians.
     * @param dms: Rotation dimension. Default: 2 to rotate around z axis.
     * @return A rotated image in rad radians with linear interpolation.
     * @brief Rotates a image.
     * @warning none.
     */
    template< class D >
    Image< D > Rotate( const Image< D > &img, float rad, size_t dms = 2 );

    /**
     * @date 2014/Jun/08
     * @param img: Input image.
     * @param interpolation: Pixel interpolation to be used.
     * @param rad: Rotation in radians.
     * @param dms: Rotation dimension. Default: 2 to rotate around z axis.
     * @return A rotated image.
     * @brief Rotates a image.
     * @warning none.
     */
    template< class D >
    Image< D > Rotate( const Image< D > &img, const PixelInterpolation &interpolation, float rad, size_t dms = 2 );

    /**
     * @date 2014/May/27
     * @param rad: Rotation in radians
     * @param dms: Rotation dimension. Default: 2 to rotate around z axis.
     * @return The affine transformation corresponding to a rotation.
     * @brief Generates a transformation corresponding to a rotation.
     * @warning none.
     */
    Matrix< float > Rotate( double rad, size_t dms = 2 );

    /**
     * @date 2014/May/27
     * @param source: Initial affine transformation over which rotation is applyed.
     * @param rad: Rotation in radians
     * @param dms: Rotation dimension. Default: 2 to rotate around z axis.
     * @return The affine transformation corresponding to a rotation applyed over source matrix.
     * @brief Generates a composite transformation of the rotation applyed over source matrix.
     * @warning none.
     */
    Matrix< float > Rotate( Matrix< float > &source, double rad, size_t dms = 2 );

  }

}

#include "GeometricsRotate.cpp"

#endif
