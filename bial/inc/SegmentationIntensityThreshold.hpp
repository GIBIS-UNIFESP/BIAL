 /* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Jul/12 
 * @brief: Segmentation by intensity thresholding.
 */

#ifndef BIALSEGMENTATIONINTENSITYTHRESHOLD_H
#define BIALSEGMENTATIONINTENSITYTHRESHOLD_H

#include "Common.hpp"
#include "Vector.hpp"

namespace Bial {

  template< class D >
  class Image;

  namespace Segmentation {

    /**
     * @date 2013/Sep/24 
     * @param img: Image for thresholding. 
     * @param Tmin: Minimum intensity. 
     * @param Tmax: Maximum intensity. 
     * @return None. 
     * @brief Computes a threshold over img. Given a pixel p, img[ p ] = 0 if img[ p ] < Tmin or img[ p ] > Tmax,
     * and img[ p ] = 1 otherwise. 
     * @warning Tmin must be lower or equal to Tmax. 
     */
    template< class D >
    void IntensityThreshold( Image< D > &img, const D Tmin, const D Tmax );

  }

}

#include "SegmentationIntensityThreshold.cpp"

#endif
