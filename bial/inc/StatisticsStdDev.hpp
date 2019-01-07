/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Dec/04
 * @brief Intensity standard deviation
 */

#ifndef BIALSTATISTICSSTDDEV_H
#define BIALSTATISTICSSTDDEV_H

#include "Common.hpp"

namespace Bial {

  namespace Statistics {

    /**
     * @date 2013/Nov/26
     * @param data: Input data.
     * @return The standard deviation of the intensities.
     * @brief Computes and returns the standard deviation of the intensities.
     * @warning none.
     */
    template< template< class D > class C, class D >
    float StandardDeviation( const C< D > &data );

    /**
     * @date 2013/Dec/06
     * @param data: Input data.
     * @param first: First element of input container.
     * @param size: Number of elements to be used.
     * @return The standard deviation of the intensities.
     * @brief Computes and returns the standard deviation of the intensities.
     * @warning none.
     */
    template< template< class D > class C, class D >
    float StandardDeviation( const C< D > &data, size_t first, size_t size );

    /**
     * @date 2013/Nov/26
     * @param data: Input data.
     * @param average: The average of the intensity of the input data.
     * @return The standard deviation of the intensities.
     * @brief Computes and returns the standard deviation of the intensities.
     * @warning none.
     */
    template< template< class D > class C, class D >
    float StandardDeviation( const C< D > &data, float average );

    /**
     * @date 2013/Dec/06
     * @param data: Input data.
     * @param first: First element of input container.
     * @param size: Number of elements to be used.
     * @param average: The average of the intensity of the input data.
     * @return The standard deviation of the intensities.
     * @brief Computes and returns the standard deviation of the intensities.
     * @warning none.
     */
    template< template< class D > class C, class D >
    float StandardDeviation( const C< D > &data, size_t first, size_t size, float average );

  }

}

#include "StatisticsStdDev.cpp"

#endif
