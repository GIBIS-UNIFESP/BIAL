/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Aug/27 
 * @brief Integral image construction and properties.
 */

#ifndef BIALINTEGRAL_H
#define BIALINTEGRAL_H

#include "Common.hpp"
#include "Image.hpp"
#include "NiftiHeader.hpp"
#include "Vector.hpp"

namespace Bial {
  /**
   * @brief A class to handle integral image. 
   */
  class Integral {

  public:

    /**
     * @date 2013/Aug/27 
     * @param img: An image. 
     * @return Integral image based on img. 
     * @brief Returns the integral image based on img. 
     * @warning none. 
     */
    template< class D >
    static Image< double > IntegralImage( const Image< D > &img );

    /**
     * @date 2013/Aug/29 
     * @param integral: An integral image. 
     * @param window_end: Greater coordinates of the window. 
     * @param window_size: window dimensions in pixels. 
     * @return Integral intensity value of window, with posterior extremity in window_end, in the integral image. 
     * @brief Returns the integral intensity value of window, with posterior extremity in window_end in the
     * integral image. 
     * @warning none. 
     */
    static double WindowIntegralValue( const Image< double > &integral,
                                       const Vector< size_t > &window_end,
                                       const Vector< size_t > &window_size );

    /**
     * @date 2013/Aug/29 
     * @param integral: An integral image. 
     * @param window_end: Greater coordinates of the window. 
     * @param window_size_mm: window dimensions in millimeters. 
     * @return Integral intensity value of window, with posterior extremity in window_end, in the integral image. 
     * @brief Returns the integral intensity value of window, with posterior extremity in window_end in the
     * integral image. 
     * @warning none. 
     */
    static double WindowIntegralValue( const Image< double > &integral,
                                       const Vector< size_t > &window_end,
                                       const Vector< float > &window_size_mm );

    /**
     * @date 2013/Aug/29 
     * @param window_size: window dimensions in pixels. 
     * @return Volume of the window. 
     * @brief Returns the volume of the window. 
     * @warning none. 
     */
    static double WindowVolume( const Vector< size_t > &window_size );

    /**
     * @date 2013/Aug/29 
     * @param img: An image. 
     * @param window_size_mm: window dimensions in millimeters. 
     * @return Volume of the window according to img pixel dimensions in millimeters. 
     * @brief Returns the volume of the window according to img pixel dimensions in millimeters. 
     * @warning none. 
     */
    template< class D >
    static double WindowVolume( const Image< D > &img, const Vector< float > &window_size_mm );

    /**
     * @date 2013/Aug/29 
     * @param pixel_size: pixel dimensions. 
     * @param window_size_mm: window dimensions in millimeters. 
     * @return A window with the same dimensions as the input described in number of pixels instead of in
     * millimeters. 
     * @brief Returns a window with the same dimensions as the input described in number of pixels instead of in
     * millimeters. 
     * @warning none. 
     */
    static Vector< size_t > PixelWindow( const Vector< float > &pixel_size, const Vector< float > &window_size_mm );

    /**
     * @date 2013/Aug/29 
     * @param integral: An integral image. 
     * @param window_end: Greater coordinates of the window. 
     * @param window_size: Window dimensions in pixels. 
     * @return Density value of window, with posterior extremity in window_end in the integral image. 
     * @brief Returns the density value of window, with posterior extremity in window_end in the integral
     * image. 
     * @warning none. 
     */
    static double WindowDensity( const Image< double > &integral,
                                 const Vector< size_t > &window_end,
                                 const Vector< size_t > &window_size );

    /**
     * @date 2013/Aug/29 
     * @param integral: An integral image. 
     * @param window_end: Greater coordinates of the window. 
     * @param window_size_mm: Window dimensions in millimeters. 
     * @return Density value of window, with posterior extremity in window_end in the integral image. 
     * @brief Returns the density value of window, with posterior extremity in window_end in the integral
     * image. 
     * @warning none. 
     */
    static double WindowDensity( const Image< double > &integral,
                                 const Vector< size_t > &window_end,
                                 const Vector< float > &window_size_mm );
  };

}

#include "Integral.cpp"

#endif
