/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/11 
 * @brief Computes median filter over the signal.
 */

#ifndef BIALSIGNALMEDIANFILTER_H
#define BIALSIGNALMEDIANFILTER_H

#include "Common.hpp"
#include "Vector.hpp"

namespace Bial {

  class Signal;

  namespace SignalOp {

    /**
     * @date 2012/Sep/11 
     * @param sgn: Input signal.
     * @param kernel_size: Size of the kernel for filtering.
     * @return none. 
     * @brief Filters input signal by a median kernel of size kernel_size. 
     * @warning none. 
     */
    template< class T >
    void MedianFilter( T &sgn, size_t kernel_size );

    /**
     * @date 2012/Sep/11 
     * @param sgn: Input signal. 
     * @param kernel_size: Size of the kernel for filtering.
     * @return Signal filtered by median kernel of size kernel_size. 
     * @brief Returns a copy of input signal filtered by median kernel of size kernel_size. 
     * @warning none. 
     */
    template< class T >
    T MedianFilter( const T &sgn, size_t kernel_size );

  }

}

#include "SignalMedianFilter.cpp"

#endif
