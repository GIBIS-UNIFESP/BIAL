/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/11 
 * @brief Functions to deal with empty bins in a signal. Useful to locate key points.
 */

#ifndef BIALSIGNALEMPTYBINS_H
#define BIALSIGNALEMPTYBINS_H

#include "Common.hpp"
#include "Vector.hpp"

namespace Bial {

  class Signal;

  namespace SignalOp {

    /**
     * @date 2012/Sep/11 
     * @param sgn: Input signal. 
     * @return Copy of input signal, without the empty bins. 
     * @brief Returns a copy of input signal, without the empty bins. 
     * @warning none. 
     */
    Signal RemoveEmptyBins( const Signal &sgn );

    /**
     * @date 2012/Sep/12 
     * @param source: The source signal to be mapped. Signal without empty bins.
     * @param target: The target signal with empty bins. Indexes from source corresponding to target are returned.
     * @return A Vector contaning the indexes of the target bins which corresponds to the bins with the same frequency 
     *         in source signals.
     * @brief Returns a Vector contaning the indexes of the target bins which corresponds to the bins with the same 
     *        frequency in source signals. Used to get corresponding bins with a signal which empty bins were removed.
     * @warning none. 
     */
    Vector< size_t > MapBins( const Signal &source, const Signal &target );

  }

}

#include "SignalEmptyBins.cpp"

#endif
