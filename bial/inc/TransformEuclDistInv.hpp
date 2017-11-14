/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2014/Jan/07
 * @brief Inverse Euclidean distance transform
 */

#ifndef BIALTRANSFORMEUCLDISTINV_H
#define BIALTRANSFORMEUCLDISTINV_H

#include "Common.hpp"

namespace Bial {

  template< class D >
  class Image;

  namespace Transform {

    /**
     * @date 2014/Jan/07
     * @param border: Border of binary image.
     * @param mask: Binary image.
     * @param root_dist: Minimum distance to a root pixel .
     * @return The root map of the inverse paths to a pixel that is at root_dist distance from the border.
     * @brief Computes the distance transform and then follows the paths backwards to a pixel that is at root_dist
     * distance from the border.
     * @warning This function should use pixel dimensions but is not using yet.
     */
    template< class D >
    Image< D > InverseEDT( const Image< D > &border, const Image< D > &mask, int root_dist );

  }

}

#include "TransformEuclDistInv.cpp"

#endif
