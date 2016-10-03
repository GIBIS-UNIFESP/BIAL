/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date: 2013/Oct/29
 * @brief HSI color space convertion methods.
 */

#include "Common.hpp"

#ifndef BIALCOLORHSI_H
#define BIALCOLORHSI_H

#include "Color.hpp"

namespace Bial {

  template< class D >
  class Image;

  namespace ColorSpace {

    /**
     * @date 2016/Jan/13
     * @param pxl: RGB format pixel
     * @return HSI format pixel
     * @brief Converts a RGB format pixel to HSI format.
     * @warning none.
     */
    RealColor ARGBtoAHSI( const Color &pxl );

    /**
     * @date 2016/Jan/13
     * @param pxl: HSI format pixel
     * @return RGB format pixel
     * @brief Converts a HSI format pixel to RGB format.
     * @warning none.
     */
    Color AHSItoARGB( const RealColor &pxl );

    /**
     * @date 2016/Jan/13
     * @param img: RGB format image
     * @return HSI format Image
     * @brief Converts a RGB format image to HSI format.
     * @warning none.
     */
    Image< RealColor > ARGBtoAHSI( const Image< Color > &img );

    /**
     * @date 2016/Jan/13
     * @param img: HSI format image
     * @return RGB format Image
     * @brief Converts a HSI format image to RGB format.
     * @warning none.
     */
    Image< Color > AHSItoARGB( const Image< RealColor > &img );

  }

}

#include "ColorHSI.cpp"

#endif
