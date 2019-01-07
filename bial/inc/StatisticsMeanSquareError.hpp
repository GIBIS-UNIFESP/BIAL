/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Dec/04
 * @brief Mean Square Error (MSE) metric.
 */

#ifndef BIALSTATISTICSMEANSQUAREERROR_H
#define BIALSTATISTICSMEANSQUAREERROR_H

#include "Common.hpp"

namespace Bial {

  template< class D >
  class Image;

  namespace Statistics {

    /**
     * @date 2013/Nov/14
     * @param src_img: Source image.
     * @param tgt_img: Target image.
     * @return Mean square error (MSE) between source and target images.
     * @brief Computes and returns the mean square error (MSE) between source and target images.
     * @warning none.
     */
    template< template< class D > class C, class D >
    float MeanSquareError( const C< D > &src_label, const C< D > &tgt_label );

  }

}

#include "StatisticsMeanSquareError.cpp"

#endif
