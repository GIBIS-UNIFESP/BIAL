/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2018/Nov/01
 * @brief Structural similarity (SSIM) metric.
 */

#ifndef BIALSTATISTICSSSIM_H
#define BIALSTATISTICSSSIM_H

#include "Common.hpp"

namespace Bial {

  template< class D >
  class Image;

  namespace Statistics {

    /**
     * @date 2013/Nov/14
     * @param src_img: Source image.
     * @param tgt_img: Target image.
     * @return Structural similarity (SSIM) between source and target images.
     * @brief Computes and returns the structural similarity (SSIM) between source and target images.
     * @warning none.
     */
    template< template< class D > class C, class D >
    float SSIM( const C< D > &src_label, const C< D > &tgt_label );

  }

}

#include "StatisticsSSIM.cpp"

#endif
