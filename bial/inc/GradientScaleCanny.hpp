/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/21 
 * @brief Computes multiscale Canny gradient
 */

#ifndef BIALGRADIENTSCALECANNY_H
#define BIALGRADIENTSCALECANNY_H

#include "Common.hpp"

namespace Bial {

  template< class D >
  class Image;

  /**
   * @brief Computes image gradients.
   */
  namespace Gradient {

    /**
     * @date 2014/Jun/09 
     * @param img: Input image. 
     * @param window_scale: Scale by which image is divided to generate the window. 
     * @param lower_threshold: Lower hysteresis threshold. 
     * @param higher_threshold: Higher hysteresis threshold. 
     * @return Scaled Canny edge detection image. 
     * @brief Returns a scaled Canny edge detection image. Lower and higher threshold as frequencies from 0.0 to
     *1.0. 
     * @warning none. 
     */
    template< class D >
    Image< D > ScaleCanny( const Image< D > &img, size_t window_scale, float lower_threshold, float higher_threshold );

    /**
     * @date 2015/Jan/27 
     * @param img: Input image. 
     * @param base_grad: Gradient from higher levels with greater windows. 
     * @param window_scale: Scale by which image is divided to generate the window. 
     * @param lower_threshold: Lower hysteresis threshold. 
     * @return Sub scaled Canny edge detection image. 
     * @brief Returns a sub scaled Canny edge detection image. Lower threshold as a frequency from 0.0 to 1.0. 
     * @warning none. 
     */
    template< class D >
    Image< D > SubScaleCanny( const Image< D > &img, const Image< D > &base_grad, size_t window_scale, 
                              float lower_threshold );

    /**
     * @date 2015/Jan/26 
     * @param img: Input image. 
     * @param lower_threshold: Lower hysteresis threshold. 
     * @param higher_threshold: Higher hysteresis threshold. 
     * @param sigma: Standard deviation of Gaussian filter. 
     * @param scales: Number of scales to run Canny. 
     * @return Multi scale Canny edge detection image. 
     * @brief Returns multi scale Canny edge detection image. Lower and higher threshold as frequencies from 0.0
     * to 1.0. 
     * @warning none. 
     */
    template< class D >
    Image< D > MultiScaleCanny( const Image< D > &img, float lower_threshold, float higher_threshold, float sigma, 
                                size_t scales );

    /**
     * @date 2015/Jan/27 
     * @param img: Input image. 
     * @param lower_threshold: Lower hysteresis threshold. 
     * @param higher_threshold: Higher hysteresis threshold. 
     * @param sigma: Standard deviation of Gaussian filter. 
     * @param scales: Number of scales to run Canny. 
     * @return Multi scale Canny edge detection image. 
     * @brief Returns multi scale Canny edge detection image. Lower and higher threshold as frequencies from 0.0
     * to 1.0. 
     * @warning none. 
     */
    template< class D >
    Image< D > MultiSubScaleCanny( const Image< D > &img, float lower_threshold, float higher_threshold, float sigma,
                                   size_t scales );

    /**
     * @date 2015/Jun/02 
     * @param img: Input image. 
     * @param lower_threshold: Lower hysteresis threshold. 
     * @param higher_threshold: Higher hysteresis threshold. 
     * @param sigma: Standard deviation of Gaussian filter. 
     * @param scales: Number of scales to run Canny. 
     * @param fraction: Fraction of the maximum size edge that specifies the minimum edge size. (0.0, 1.0) 
     * @return Multi scale Canny edge detection image. 
     * @brief Returns multi scale Canny edge detection image. Lower and higher threshold as frequencies from 0.0
     * to 1.0. 
     * @warning none. 
     */
    template< class D >
    Image< D > MultiScaleSizeCanny( const Image< D > &img, float lower_threshold, float higher_threshold, 
                                    float sigma, size_t scales, float fraction );

  }

}

#include "GradientScaleCanny.cpp"

#endif
