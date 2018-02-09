/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/21 
 * @brief Computes morphological gradient, that is, open - close.
 */

#ifndef BIALGRADIENTMORPHOLOGICAL_H
#define BIALGRADIENTMORPHOLOGICAL_H

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
     * @date 2012/Sep/21 
     * @param image: Input image. 
     * @return Gradient image. 
     * @brief Computes the gradient image (dilation - erosion)
     * using spherical adjacency relation of one pixel of
     * radius. 
     * @warning none. 
     */
    template< class D >
    Image< D > Morphological( const Image< D > &image );

    /**
     * @date 2012/Sep/21 
     * @param image: Input image. 
     * @param adjacency: Adjacency relation used by dilation and
     *             erosion opperations. 
     * @return Gradient image. 
     * @brief Computes the gradient image. 
     * @warning none. 
     */
    template< class D >
    Image< D > Morphological( const Image< D > &image, const Adjacency &adjacency );

  }

}

#include "GradientMorphological.cpp"

#endif
