/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Dec/04
 * @brief Peak signal to noise ration (PSNR) metric.
 */

#ifndef BIALSTATISTICSPSNR_H
#define BIALSTATISTICSPSNR_H

#include "Common.hpp"

namespace Bial {

  template< class D >
  class Image;

  namespace Statistics {

    /**
     * @date 2013/Nov/14
     * @param src_img: Source image.
     * @param tgt_img: Target image.
     * @return Peak signal to noise ration (PSNR) between source and target images.
     * @brief Computes and returns the peak signal to noise ration (PSNR) between source and target images.
     * @warning none.
     */
    template< template< class D > class C, class D >
    float PSNR( const C< D > &src_label, const C< D > &tgt_label );

  }

}

#include "StatisticsPSNR.cpp"

#endif
