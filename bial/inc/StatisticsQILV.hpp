/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2018/Nov/29
 * @brief Quality Index based on Local Variance.
 */

#ifndef BIALSTATISTICSQILV_H
#define BIALSTATISTICSQILV_H

#include "Common.hpp"

namespace Bial {

  template< class D >
  class Image;

  namespace Statistics {

    /**
     * @date 2018/Nov/29
     * @param src_img: Source image.
     * @param tgt_img: Target image.
     * @return Quality Index based on Local Variance between source and target images.
     * @brief Computes and returns the Quality Index based on Local Variance between source and target images.
     * @warning none.
     */
    template< template< class D > class C, class D >
    float QILV( const C< D > &src_label, const C< D > &tgt_label );

  }

}

#include "StatisticsQILV.cpp"

#endif
