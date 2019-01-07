/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2018/Nov/29
 * @brief Entropy difference metric.
 */

#ifndef BIALSTATISTICSENTROPYDIFFERENCE_H
#define BIALSTATISTICSENTROPYDIFFERENCE_H

#include "Common.hpp"

namespace Bial {

  template< class D >
  class Image;

  namespace Statistics {

    /**
     * @date 2018/Nov/29
     * @param src_img: Source image.
     * @param tgt_img: Target image.
     * @return Entropy difference between source and target images.
     * @brief Computes and returns the entropy difference between source and target images.
     * @warning none.
     */
    template< template< class D > class C, class D >
    float EntropyDifference( const C< D > &src_label, const C< D > &tgt_label );

  }

}

#include "StatisticsEntropyDifference.cpp"

#endif
