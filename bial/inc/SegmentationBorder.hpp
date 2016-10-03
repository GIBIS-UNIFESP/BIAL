 /* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Jul/12 
 * @brief: Segmentation of object border.
 */

#ifndef BIALSEGMENTATIONBORDER_H
#define BIALSEGMENTATIONBORDER_H

#include "Common.hpp"
#include "Vector.hpp"

namespace Bial {

  class Adjacency;
  template< class D >
  class Image;

  namespace Segmentation {

    /**
     * @date 2014/Jan/07 
     * @param img: Input image. 
     * @param adj: An adjacency relation. 
     * @return Image with border pixels only. 
     * @brief Pixels belong to the border if they have at last one adjacent pixel with distinct intensity. 
     * @warning none. 
     */
    template< class D >
    Image< D > BorderPixels( const Image< D > &img, const Adjacency &adj );

  }

}

#include "SegmentationBorder.cpp"

#endif
