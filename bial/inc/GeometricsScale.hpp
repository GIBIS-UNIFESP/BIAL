/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date: 2014/Jan/07
 * @brief Image scaling. Direct scaling or corresponding affine transform.
 */

#ifndef BIALGEOMETRICSSCALE_H
#define BIALGEOMETRICSSCALE_H

#include "Common.hpp"
#include "PixelInterpolation.hpp"
#include "Matrix.hpp"
#include "Vector.hpp"

namespace Bial {

  template< class D > class Image;

  namespace Geometrics {
    
    /**
     * @date 2014/Jan/07
     * @param img: Input image.
     * @param factor: Scaling factor. If greater than 1.0, image is dilated. If lower than 1.0, image is
     *             compressed.
     * @param resize: resize the resultant image.
     * @return A scaled image with linear interpolation.
     * @brief Scales a image.
     * @warning none.
     */
    template< class D >
    Image< D > Scale( const Image< D > &img, double factor, bool resize = false );

    /**
     * @date 2014/Jan/07
     * @param img: Input image.
     * @param interpolation: Pixel interpolation to be used.
     * @param factor: Scaling factor. If greater than 1.0, image is dilated. If lower than 1.0, image is
     *             compressed.
     * @param resize: resize the resultant image.
     * @return A scaled image.
     * @brief Scales a image.
     * @warning none.
     */
    template< class D >
    Image< D > Scale( const Image< D > &img, const PixelInterpolation &interpolation, double factor, 
                      bool resize = false );

    /**
     * @date 2014/Aug/16
     * @param img: Input image.
     * @param fx: Scaling factor in X. If greater than 1.0, image is dilated. If lower than 1.0, image is
     *             compressed.
     * @param fy: Scaling factor in Y. If greater than 1.0, image is dilated. If lower than 1.0, image is
     *             compressed.
     * @param fz: Scaling factor in Z. If greater than 1.0, image is dilated. If lower than 1.0, image is
     *             compressed.
     * @param resize: resize the resultant image.
     * @return A scaled image with linear interpolation.
     * @brief Scales a image.
     * @warning none.
     */
    template< class D >
    Image< D > Scale( const Image< D > &img, double fx, double fy, double fz = 1.0, bool resize = false );

    /**
     * @date 2014/Aug/16
     * @param img: Input image.
     * @param interpolation: Pixel interpolation to be used.
     * @param fx: Scaling factor in X. If greater than 1.0, image is dilated. If lower than 1.0, image is
     *             compressed.
     * @param fy: Scaling factor in Y. If greater than 1.0, image is dilated. If lower than 1.0, image is
     *             compressed.
     * @param fz: Scaling factor in Z. If greater than 1.0, image is dilated. If lower than 1.0, image is
     *             compressed.
     * @param resize: resize the resultant image.
     * @return A scaled image.
     * @brief Scales a image.
     * @warning none.
     */
    template< class D >
    Image< D > Scale( const Image< D > &img, const PixelInterpolation &interpolation, double fx, double fy, 
                      double fz = 1.0, bool resize = false );

    /**
     * @date 2014/Jan/07
     * @param img: Input image.
     * @param dims: Resulting image dimensions.
     * @return A scaled image with linear interpolation.
     * @brief Scales a image.
     * @warning none.
     */
    template< class D >
    Image< D > Scale( const Image< D > &img, const Vector< size_t > &dims );

    /**
     * @date 2014/Jan/07
     * @param img: Input image.
     * @param interpolation: Pixel interpolation to be used.
     * @param dims: Resulting image dimensions.
     * @return A scaled image.
     * @brief Scales a image.
     * @warning none.
     */
    template< class D >
    Image< D > Scale( const Image< D > &img, const PixelInterpolation &interpolation, const Vector< size_t > &dims );

    /**
     * @date 2014/Jun/26
     * @param img: Input image.
     * @param fx: Scaling factor in X. If greater than 1.0, image is dilated. If lower than 1.0, image is
     *             compressed.
     * @param fy: Scaling factor in Y. If greater than 1.0, image is dilated. If lower than 1.0, image is
     *             compressed.
     * @param fz: Scaling factor in Z. If greater than 1.0, image is dilated. If lower than 1.0, image is
     *             compressed.
     * @return The affine transformation corresponding to a scaling.
     * @brief Generates a transformation corresponding to a scaling.
     * @warning none.
     */
    Matrix< float > Scale( double fx, double fy, double fz = 1.0 );

    /**
     * @date 2014/Jun/26
     * @param img: Input image.
     * @param source: Initial affine transformation over which scaling is applyed.
     * @param fx: Scaling factor in X. If greater than 1.0, image is dilated. If lower than 1.0, image is
     *             compressed.
     * @param fy: Scaling factor in Y. If greater than 1.0, image is dilated. If lower than 1.0, image is
     *             compressed.
     * @param fz: Scaling factor in Z. If greater than 1.0, image is dilated. If lower than 1.0, image is
     *             compressed.
     * @return The affine transformation corresponding to a scaling applyed over source matrix.
     * @brief Generates a composite transformation of the scaling applyed over source matrix.
     * @warning none.
     */
    Matrix< float > Scale( Matrix< float > &source, double fx, double fy, double fz = 1.0 );

  }

}

#include "GeometricsScale.cpp"

#endif
