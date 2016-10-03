/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/21 
 * @brief Computes Gabor gradient.
 */

#ifndef BIALGRADIENTGABOR_H
#define BIALGRADIENTGABOR_H

#include "Common.hpp"

namespace Bial {

  template< class D >
  class Image;

  namespace Gradient {

    /**
     * @date 2015/Jun/03 
     * @param img: Input image. 
     * @param sigma: Standard deviation of the Gaussian. 
     * @param magnitude: Gabor magnitude image. 
     * @param direction: Output Gabor direction image. 
     * @return Directional Gabor filter over image. 
     * @brief Returns the directional Gabor filter over image. 
     * @warning none. 
     */
    template< class D >
    void Gabor( const Image< D > &img, float sigma, Image< D > *magnitude = nullptr,
                       Image< int > *direction = nullptr );

  };

}

#include "GradientGabor.cpp"

#endif
