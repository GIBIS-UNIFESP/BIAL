/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2016/Sep/15 
 * @brief Image interpolation using interpolation functions.
 */

#include "Common.hpp"

#ifndef BIALIMAGEINTERPOLATION_H
#define BIALIMAGEINTERPOLATION_H

namespace Bial {

  template< class D >
  class Image;

  class PixelInterpolation;

  namespace ImageInterpolation {

    /**
     * @date 2016/Set/15
     * @param img: 2D input image. 
     * @param interpolation_type: type of interpolation.
     * @return Returns an interpolation of the 2D input image. 
     * @brief Computes the interpolation of the 2D input image with given dimensions and intepolation type.
     * @warning none. 
     */
    template< class D >
    Image< D > Interpolation( const Image< D > &img, float dx, float dy, const PixelInterpolation &interpolation_type );

    /**
     * @date 2016/Set/15
     * @param img: 3D input image. 
     * @param interpolation_type: type of interpolation.
     * @return Returns an interpolation of the 3D input image. 
     * @brief Computes the interpolation of the 3D input image with given dimensions and intepolation type.
     * @warning none. 
     */
    template< class D >
    Image< D > Interpolation( const Image< D > &img, float dx, float dy, float dz, 
                              const PixelInterpolation &interpolation_type );

  }

}

#include "ImageInterpolation.cpp"

#endif
