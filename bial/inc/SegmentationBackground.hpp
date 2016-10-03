 /* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Jul/12 
 * @brief: Image background segmentation. Segments a large piece of dark pixels in the background.
 */

#ifndef BIALSEGMENTATIONBACKGROUND_H
#define BIALSEGMENTATIONBACKGROUND_H

#include "Common.hpp"
#include "Vector.hpp"

namespace Bial {

  template< class D >
  class Image;

  namespace Segmentation {

    /**
     * @date 2013/Nov/29 
     * @param img: Input image. 
     * @return Binary mask of the background. 
     * @brief Segments the darker clusters of the image, supposedly belonging to the background. 
     * @warning none. 
     */
    template< class D >
    Image< D > Background( const Image< D > &img );

    /**
     * @date 2013/Nov/29 
     * @param img: Input image. 
     * @param canny: Canny edges. (Suggested hysteresis thresholds: 0.8, 0.9). 
     * @return Binary mask of the background. 
     * @brief Segments the darker clusters of the image, supposedly belonging to the background. 
     * @warning none. 
     */
    template< class D >
    Image< D > Background( const Image< D > &img, const Image< D > &canny );

  }

}

#include "SegmentationBackground.cpp"

#endif
