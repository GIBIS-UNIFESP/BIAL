/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2018/Nov/29
 * @brief Image Quality Index metric.
 */

#ifndef BIALSTATISTICSIQI_H
#define BIALSTATISTICSIQI_H

#include "Common.hpp"

namespace Bial {

  template< class D >
  class Image;

  namespace Statistics {

    /**
     * @date 2018/Nov/29
     * @param src_img: Source image.
     * @param tgt_img: Target image.
     * @return Image Quality Index between source and target images.
     * @brief Computes and returns the Image Quality Index between source and target images.
     * @warning none.
     */
    template< template< class D > class C, class D >
    float IQI( const C< D > &src_label, const C< D > &tgt_label );

  }

}

#include "StatisticsIQI.cpp"

#endif
