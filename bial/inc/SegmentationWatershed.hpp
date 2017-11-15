 /* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Jul/12 
 * @brief: Segmentation with watershed algorithm.
 */

#ifndef BIALSEGMENTATIONWATERSHED_H
#define BIALSEGMENTATIONWATERSHED_H

#include "Common.hpp"
#include "Vector.hpp"

namespace Bial {

  template< class D >
  class Image;

  namespace Segmentation {

    /**
     * @date 2013/Oct/08 
     * @param gradient: Gradient input image. 
     * @param radius: Radius for local minima detection.
     * @return Label image from Watershed segmentation algorithm. 
     * @brief Returns the label image from Watershed segmentation algorithm using local minima as the leaking
     * points. 
     * @warning Gradient image is changed. It returns the cost map. 
     */
    template< class D >
    Image< int > Watershed( Image< D > &gradient, float radius = 1.1f );

    /**
     * @date 2013/Oct/08 
     * @param gradient: Gradient input image. 
     * @param seeds: Seed vector. 
     * @return Label image from Watershed segmentation algorithm. 
     * @brief Returns the label image from Watershed segmentation algorithm using seeds as the leaking points. 
     * @warning Gradient image is changed. It returns the cost map. 
     */
    template< class D >
    Image< int > Watershed( Image< D > &gradient, const Vector< bool > &seeds );

    /**
     * @date 2013/Oct/08 
     * @param gradient: Gradient input image. 
     * @param obj_seeds, bkg_seeds: Seed vectors for object and back ground respectively. 
     * @return Label image from Watershed segmentation algorithm. 
     * @brief Returns the label image from Watershed segmentation algorithm using seeds as the leaking points. 
     * @warning Gradient image is changed. It returns the cost map. 
     */
    template< class D >
    Image< int > Watershed( Image< D > &gradient, const Vector< size_t > &obj_seeds,
                            const Vector< size_t > &bkg_seeds );

  }

}

#include "SegmentationWatershed.cpp"

#endif
