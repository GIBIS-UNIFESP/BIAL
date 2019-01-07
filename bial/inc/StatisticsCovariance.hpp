/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Dec/04
 * @brief Intensity covariance
 */

#ifndef BIALSTATISTICSCOVARIANCE_H
#define BIALSTATISTICSCOVARIANCE_H

#include "Common.hpp"

namespace Bial {

  namespace Statistics {

    /**
     * @date 2013/Nov/26
     * @param src_data: Source data.
     * @param tgt_data: Target data.
     * @return The covariance of the intensities between source and target data.
     * @brief Computes and returns the covariance of the intensities.
     * @warning none.
     */
    template< template< class D > class C, class D >
    float Covariance( const C< D > &src_data, const C< D > &tgt_data );

  }

}

#include "StatisticsCovariance.cpp"

#endif
