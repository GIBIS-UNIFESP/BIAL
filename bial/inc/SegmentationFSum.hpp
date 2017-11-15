 /* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2016/Nov/03 
 * @brief: Segmentation with Sum Path Funcion.
 */

#ifndef BIALSEGMENTATIONFSUM_H
#define BIALSEGMENTATIONFSUM_H

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
     * @return Label image from FSum segmentation algorithm. 
     * @brief Returns the label image from FSum segmentation algorithm using local minima as the leaking
     * points. 
     * @warning Gradient image is changed. It returns the cost map. 
     */
    template< class D >
    Image< int > FSum( Image< D > &gradient, float radius = 1.1f );

    /**
     * @date 2013/Oct/08 
     * @param gradient: Gradient input image. 
     * @param seeds: Seed vector. 
     * @return Label image from FSum segmentation algorithm. 
     * @brief Returns the label image from FSum segmentation algorithm using seeds as the leaking points. 
     * @warning Gradient image is changed. It returns the cost map. 
     */
    template< class D >
    Image< int > FSum( Image< D > &gradient, const Vector< bool > &seeds );

    /**
     * @date 2013/Oct/08 
     * @param gradient: Gradient input image. 
     * @param obj_seeds, bkg_seeds: Seed vectors for object and back ground respectively. 
     * @return Label image from FSum segmentation algorithm. 
     * @brief Returns the label image from FSum segmentation algorithm using seeds as the leaking points. 
     * @warning Gradient image is changed. It returns the cost map. 
     */
    template< class D >
    Image< int > FSum( Image< D > &gradient, const Vector< size_t > &obj_seeds,
                            const Vector< size_t > &bkg_seeds );

  }

}

#include "SegmentationFSum.cpp"

#endif
