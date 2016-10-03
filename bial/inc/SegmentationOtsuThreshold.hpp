 /* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Jul/12 
 * @brief: Segmentation by Otsu thresholding.
 */

#ifndef BIALSEGMENTATIONOTSUTHRESHOLD_H
#define BIALSEGMENTATIONOTSUTHRESHOLD_H

#include "Common.hpp"
#include "Vector.hpp"

namespace Bial {

  template< class D >
  class Image;

  namespace Segmentation {

    /**
     * @date 2012/Sep/08 
     * @param img: Image for thresholding. 
     * @return Otsu threshold of img. 
     * @brief Compute otsu threshold of img. 
     * @warning Output value may be approximated in the case of floating point images. 
     */
    template< class D >
    D OtsuThreshold( const Image< D > &img );

    /**
     * @date 2012/Nov/01 
     * @param img: Image for thresholding. 
     * @return Limiarized image. 
     * @brief Limiarize image using the OtsuThreshold( img ) limiar. 
     * @warning Support only binary limiarization. 
     *           Color Images will be limiarized by color channel 
     */
    template< class D >
    Image< D > Otsu( const Image< D > &img );

  }

}

#include "SegmentationOtsuThreshold.cpp"

#endif
