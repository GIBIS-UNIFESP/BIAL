/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/21 
 * @brief Computes directional gradient. This gradient is the contribution of the intensity differences from all 
 * adjacents. 
 */

#ifndef BIALGRADIENTDIRECTIONAL_H
#define BIALGRADIENTDIRECTIONAL_H

#include "Common.hpp"

namespace Bial {

  template< class D >
  class Image;
  class Adjacency;
  /**
   * @brief Computes image gradients.
   */
  namespace Gradient {

    /**
     * @date 2013/Dec/19 
     * @param img: Input image. 
     * @param adj: An adjacency relation. 
     * @return Gradient composed by the sum of differences between a pixel and its adjacents. 
     * @brief Returns a gradient composed by the sum of differences between a pixel and its adjacents. 
     * @warning none. 
     */
    template< class D >
    Image< D > Directional( const Image< D > &img, const Adjacency &adj );

  }

}

#include "GradientDirectional.cpp"

#endif
