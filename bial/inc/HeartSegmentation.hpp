/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/12/12 
 * @brief Heart segmentation functions 
 */

#ifndef BIALHEARTSEGMENTATION_H
#define BIALHEARTSEGMENTATION_H

#include "Common.hpp"

namespace Bial {

  template< class D >
  class Image;

  namespace Heart {

    /**
     * @date 2014/Oct/14 
     * @param img: A heart image. 
     * @param orientation: Image orientation. Must be a composition of three values containing:
     *             LR, RL, SI, IS, AP, or PA. Look for NiftiHeader::Orientation() function for examples. 
     * @return Binary mask of segmented heart. 
     * @brief Returns a binary mask of segmented heart. 
     * @warning none. 
     */
    template< class D >
    Image< D > Segmentation( const Image< D > &img, const std::string &orientation );

  };

}

#include "HeartSegmentation.cpp"

#endif
