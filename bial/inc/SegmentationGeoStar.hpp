 /* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Jul/12 
 * @brief: Segmentation with oriented geodesic star algorithm. 
 *         Combines geodesic distance and sum of arcs with orientation.
 */

#ifndef BIALSEGMENTATIONGEOSTAR_H
#define BIALSEGMENTATIONGEOSTAR_H

#include "Common.hpp"
#include "Vector.hpp"

namespace Bial {

  template< class D >
  class Image;
  template< class D >
  class Matrix;
  template< class D >
  class Feature;
  /**
   * @brief Image segmentation methods.
   */
  namespace Segmentation {

    /**
     * @date 2013/Nov/05 
     * @param image: Input image. 
     * @param obj_seeds: Object seed coordinate vector. 
     * @param bkg_seeds: Background seed coordinate vector. 
     * @param alpha: Oriented edges strength. -1.0 to 1.0. Close to -1.0 (light to dark edges). Close to 0.0
     *             (non-oriented edges). Close to 1.0 (dark to light edges). 
     * @param beta: Geodesic distance force. 0.0 (geodesic distance) to 4.0 (sum function). 
     * @return Label image from oriented geodesic segmentation algorithm. 
     * @brief Returns the label from oriented geodesic segmentation algorithm. Use positive alpha values for light
     * object and dark background, and negative alpha values for dark object and light background. 
     * @warning none. 
     */
    template< class D >
    Image< D > OrientedGeodesicStar( const Image< D > &image, const Vector< size_t > &obj_seeds,
                                     const Vector< size_t > &bkg_seeds, double alpha = 0.0, double beta = 0.5 );

  }

}

#include "SegmentationGeoStar.cpp"

#endif
