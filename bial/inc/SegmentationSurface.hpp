 /* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Jul/12 
 * @brief: Segmentation of 3D surface.
 */

#ifndef BIALSEGMENTATIONSURFACE_H
#define BIALSEGMENTATIONSURFACE_H

#include "Common.hpp"
#include "Vector.hpp"

namespace Bial {

  template< class D >
  class Image;

  namespace Segmentation {

    /**
     * @date 2014/Jan/07 
     * @param img: Input 3D image. 
     * @return Image with surface border pixels. 
     * @brief Pixels belong to the border of a surface if they have less than 7 adjacent object pixels. 
     * @warning Used for 3D images. 
     */
    template< class D >
    Image< D > SurfaceBorderPixels( const Image< D > &img );

  }

}

#include "SegmentationSurface.cpp"

#endif
