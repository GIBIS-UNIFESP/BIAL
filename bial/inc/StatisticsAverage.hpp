/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Dec/04
 * @brief Average image intensity.
 */

#ifndef BIALSTATISTICSAVERAGE_H
#define BIALSTATISTICSAVERAGE_H

#include "Common.hpp"

namespace Bial {

  namespace Statistics {

    /**
     * @date 2013/Nov/26
     * @param data: Input data.
     * @return The average of the intensities.
     * @brief Computes and returns the average of the intensities.
     * @warning none.
     */
    template< template< class D > class C, class D >
    float Average( const C< D > &data );

    /**
     * @date 2013/Dec/06
     * @param data: Input data.
     * @param first: First element of input container.
     * @param size: Number of elements to be used.
     * @return The average of the intensities.
     * @brief Computes and returns the average of the intensities.
     * @warning none.
     */
    template< template< class D > class C, class D >
    float Average( const C< D > &data, size_t first, size_t size );

  }

}

#include "StatisticsAverage.cpp"

#endif
