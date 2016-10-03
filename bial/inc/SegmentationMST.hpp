 /* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Jul/12 
 * @brief: Segmentation with minimum spanning tree.
 */

#ifndef BIALSEGMENTATIONMST_H
#define BIALSEGMENTATIONMST_H

#include "Common.hpp"
#include "Vector.hpp"

namespace Bial {

  template< class D >
  class Image;
  template< class D >
  class Feature;

  namespace Segmentation {

    /**
     * @date 2013/Sep/27 
     * @param img: Input image to be segmented. Used to get image dimensions. 
     * @param feature: Feature vector. 
     * @param regions: Number of segmented regions. 
     * @return Segmented image in the number of expected regions. 
     * @brief Segmentation by minimum spanning forest algorithm. 
     * @warning Number of regions must be greater than 0 and lower than the number of samples. 
     */
    template< class D >
    Image< int > MinimumSpanningForest( const Image< D > &img, const Feature< D > feature, size_t regions );

  }

}

#include "SegmentationMST.cpp"

#endif
