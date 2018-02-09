/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/21 
 * @brief Computes Canny gradient. 
 */

#ifndef BIALGRADIENTCANNY_H
#define BIALGRADIENTCANNY_H
#include "Common.hpp"

namespace Bial {

  template< class D >
  class Image;
  /**
   * @brief Computes image gradients.
   */
  namespace Gradient {

    /**
     * @date 2013/Nov/21 
     * @param suppressed_sobel: Suppressed gradient image given. 
     * @param lower_threshold: Lower hysteresis threshold. 
     * @param higher_threshold: Higher hysteresis threshold. 
     * @return Canny edge detection image. 
     * @brief Returns Canny edge detection image. Lower and higher threshold as frequencies from 0.0 to 1.0. 
     * @warning none. 
     */
    template< class D >
    Image< D > Canny( const Image< D > &suppressed_sobel, float lower_threshold, float higher_threshold );

    /**
     * @date 2013/Nov/21 
     * @param img: Input image. 
     * @param lower_threshold: Lower hysteresis threshold. 
     * @param higher_threshold: Higher hysteresis threshold. 
     * @param sigma: Standard deviation of Gaussian filter. 
     * @return Canny edge detection image. 
     * @brief Returns Canny edge detection image. Lower and higher threshold as frequencies from 0.0 to 1.0. 
     * @warning none. 
     */
    template< class D >
    Image< D > Canny( const Image< D > &img, float lower_threshold, float higher_threshold, float sigma );

    /**
     * @date 2013/Aug/08 
     * @param img: Input image. 
     * @return Sobel magnitude image with non-maximum intensities suppressed. 
     * @brief Returns the sobel magnitude image with non-maximum intensities suppressed. 
     * @warning none. 
     */
    template< class D >
    Image< D > NonMaxSobelSuppression( const Image< D > &img );

    /**
     * @date 2013/Aug/08 
     * @param magnitude: Sobel magnitude image. 
     * @param direction: Sobel magnitude direction. 
     * @return Sobel magnitude image with non-maximum intensities suppressed. 
     * @brief Returns the sobel magnitude image with non-maximum intensities suppressed. 
     * @warning none. 
     */
    template< class D >
    Image< D > NonMaxSobelSuppression( const Image< D > &magnitude, const Image< int > &direction );

  };

}

#include "GradientCanny.cpp"

#endif
