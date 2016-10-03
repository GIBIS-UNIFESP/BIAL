 /* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Jul/12 
 * @brief: Segmentation by intensity hysteresis.
 */

#ifndef BIALSEGMENTATIONHYSTERESIS_H
#define BIALSEGMENTATIONHYSTERESIS_H

#include "Common.hpp"
#include "Vector.hpp"

namespace Bial {

  template< class D >
  class Image;

  namespace Segmentation {

    /**
     * @date 2013/Nov/26 
     * @param img: Image for thresholding. 
     * @param lower: lower intensity threshold value. 
     * @param higher: higher intensity threshold value. 
     * @return None. 
     * @brief Computes Hysteresis having first threshold in higher and propagating to connected pixels in lower.
     * @brief lower can not be greater than higher. All valid pixels receive 1.
     * @warning none. 
     */
    template< class D >
    Image< D > HighHysteresis( const Image< D > &img, const D lower, const D higher );

  }

}

#include "SegmentationHysteresis.cpp"

#endif
