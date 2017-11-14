/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/11 
 * @brief Signal equalization functions.
 */

#ifndef BIALSIGNALEQUALIZE_H
#define BIALSIGNALEQUALIZE_H

#include "Common.hpp"
#include "Vector.hpp"

namespace Bial {

  class Signal;

  namespace SignalOp {

    /**
     * @date 2013/Oct/07 
     * @param sgn: Input signal.
     * @return none. 
     * @brief Equalizes input signal. 
     * @warning none. 
     */
    void Equalize( Signal &sgn );

    /**
     * @date 2013/Oct/07 
     * @param sgn: Input signal.
     * @return Equalized signal. 
     * @brief Returns a copy of input signal, equalized. 
     * @warning none. 
     */
    Signal Equalize( const Signal &sgn );

    /**
     * @date 2013/Oct/07 
     * @param sgn: Input signal.
     * @return none.
     * @brief Equalizes input signal by segmented histogram in two parts. 
     * @warning NOT IMPLEMENTED YET.
     */
    void SegmentEqualize( Signal &sgn );

    /**
     * @date 2013/Oct/07
     * @param sgn: Input signal.
     * @return Equalized signal by segmented histogram in two parts. 
     * @brief Returns a copy of input signal, equalized. 
     * @warning NOT IMPLEMENTED YET.
     */
    Signal SegmentEqualize( const Signal &sgn );

  }

}

#include "SignalEqualize.cpp"

#endif
