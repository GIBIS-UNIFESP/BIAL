/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/11 
 * @brief Computes the accumulated signal.
 */

#ifndef BIALSIGNALACCUMULATE_H
#define BIALSIGNALACCUMULATE_H

#include "Common.hpp"
#include "Vector.hpp"

namespace Bial {

  class Signal;

  namespace SignalOp {

    /**
     * @date 2013/Oct/07 
     * @param sgn: Input signal.
     * @return none. 
     * @brief Accumulates input signal. 
     * @warning none. 
     */
    void Accumulate( Signal &sgn );

    /**
     * @date 2013/Oct/07 
     * @param sgn: Input signal.
     * @return Accumulated signal. 
     * @brief Returns a copy of input signal, accumulated. 
     * @warning none. 
     */
    Signal Accumulate( const Signal &sgn );

    /**
     * @date 2012/Sep/13 
     * @param sgn: Input signal.
     * @param min_bin: left most bin. 
     * @return The sum of the frequencies from min_bin to the last bin (including the extremities). 
     * @brief Returns the sum of the frequencies from min_bin to the last bin (including the extremities). 
     * @warning none. 
     */
    double AccumulatedFrequency( const Signal &sgn, size_t min_bin = 0 );

    /**
     * @date 2012/Sep/13
     * @param sgn: Input signal.
     * @param min_bin: left most bin. 
     * @param max_bin: right most bin. 
     * @return The sum of the frequencies from min_bin to max_bin (including the extremities). 
     * @brief Returns the sum of the frequencies from min_bin to max_bin (including the extremities). 
     * @warning none. 
     */
    double AccumulatedFrequency( const Signal &sgn, size_t min_bin, size_t max_bin );

    /**
     * @date 2012/Sep/13
     * @param sgn: Input signal.
     * @param min_bin: left most bin. 
     * @return The sum of the frequencies weighted by their sampling time from min_bin to the last bin (including the
     * extremities). 
     * @brief Returns the sum of frequencies weighted by their sampling time from min_bin to the last bin
     * (including the extremities). 
     * @warning none. 
     */
    double AccumulatedWeightedFrequency( const Signal &sgn, size_t min_bin = 0 );

    /**
     * @date 2012/Sep/13
     * @param sgn: Input signal.
     * @param min_bin: left most bin. 
     * @param max_bin: right most bin. 
     * @return The sum of the frequencies weighted by their sampling time from min_bin to max_bin (including the
     * extremities). 
     * @brief Returns the sum of frequencies weighted by their sampling time from min_bin to max_bin (including
     * the extremities). 
     * @warning none. 
     */
    double AccumulatedWeightedFrequency( const Signal &sgn, size_t min_bin, size_t max_bin );

  }

}

#include "SignalAccumulate.cpp"

#endif
