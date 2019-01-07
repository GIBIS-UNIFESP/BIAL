/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date: 2013/Oct/29
 * @brief RGB color space convertion methods.
 */

#include "Common.hpp"

#ifndef BIALCOLORRGB_H
#define BIALCOLORRGB_H

#include "Color.hpp"

namespace Bial {

  template< class D >
  class Image;

  namespace ColorSpace {

    /**
     * @date 2013/Aug/06
     * @param img: The input image.
     * @return An ARGB image.
     * @brief Converts gray image to ARGB format. Color channels receive the intensity.
     * @warning none.
     */
    template< class D >
    Image< Color > GraytoARGB( const Image< D > &img );

    /**
     * @date 2013/Aug/06
     * @param img: The input image.
     * @return Gray scale image
     * @brief Converts ARGB image to gray scale using luminosity.
     * gray=( 0.21*red + 0.72*green + 0.07*blue )/2.
     * @warning Only for 3-D colored.
     */
    template< class D >
    Image< D > ARGBtoGraybyLuminosity( const Image< Color > &img );

    /**
     * @date 2013/Aug/06
     * @param img: The input image.
     * @return Gray scale image
     * @brief Converts ARGB image to gray scale using lightness.
     * gray=(max(red,green,blue)+min(red,green,blue))/2.
     * @warning Only for 3-D colored.
     */
    template< class D >
    Image< D > ARGBtoGraybyLightness( const Image< Color > &img );

    /**
     * @date 2013/Aug/06
     * @param img: The input image.
     * @return Gray scale image
     * @brief Converts ARGB image to gray scale using brightness.
     * gray=(red+green+blue)/2.
     * @warning Only for 3-D colored.
     */
    template< class D >
    Image< D > ARGBtoGraybyBrightness( const Image< Color > &img );

  }

}

#endif
