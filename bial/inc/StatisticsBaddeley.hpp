/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Dec/04
 * @brief Baddeley metric.
 */

#ifndef BIALSTATISTICSBADDELEY_H
#define BIALSTATISTICSBADDELEY_H

#include "Common.hpp"

namespace Bial {

  template< class D >
  class Image;

  namespace Statistics {

    /**
     * @date 2016/Feb/01
     * @param src_distance_img: A distance transform from a binary boundary image.
     * @param tgt_distance_img: A distance transform from a binary boundary image.
     * @return Baddeley's measure, modified to run only over the union of boundary pixels.
     * @brief Computes Baddeley's measure. See A.J. Baddeley, An error metric for binary images, Robust Computer Vision:
     * @brief Quality of Vision Algorithms, Wichmann Verlag Karlsruhe, 1992, pp. 59–78.
     * @warning none.
     */
    template< class D >
    float Baddeley( const Image< D > &src_distance_img, const Image< D > &tgt_distance_img );

  }

}

#include "StatisticsBaddeley.cpp"

#endif
