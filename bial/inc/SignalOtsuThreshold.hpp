/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/11 
 * @brief Computes otsu threshold of signals signal.
 */

#ifndef BIALSIGNALOTSUTHRESHOLD_H
#define BIALSIGNALOTSUTHRESHOLD_H

#include "Common.hpp"

namespace Bial {

  class Signal;

  namespace SignalOp {

    /**
     * @date 2012/Sep/13 
     * @param sgn: Input signal.
     * @return Bin corresponding to Otsu threshold. 
     * @brief Computes otsu threshold of input signal. 
     * @warning none. 
     */
    size_t OtsuThreshold( const Signal &sgn );

  }

}

#include "SignalOtsuThreshold.cpp"

#endif
