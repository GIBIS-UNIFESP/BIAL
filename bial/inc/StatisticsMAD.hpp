/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Dec/04
 * @brief Median absolute deviation computation.
 */

#ifndef BIALSTATISTICSMAD_H
#define BIALSTATISTICSMAD_H

#include "Common.hpp"

namespace Bial {

  template< class D >
  class Image;

  namespace Statistics {

    /**
     * @date 2012/Dec/04
     * @param data: Input data.
     * @return The median absolute deviation.
     * @brief Computes and returns the median absolute deviation.
     * @warning none.
     */
    template< template< class D > class C, class D >
    D MedianAbsoluteDeviation( const C< D > &data );

  }

}

#include "StatisticsMAD.cpp"

#endif
