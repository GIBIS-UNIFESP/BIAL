/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/11 
 * @brief Signal normalization.
 */

#ifndef BIALSIGNALNORMALIZE_H
#define BIALSIGNALNORMALIZE_H

#include "Common.hpp"
#include "Vector.hpp"

namespace Bial {

  class Signal;

  namespace SignalOp {

    /**
     * @date 2013/Oct/07 
     * @param sgn: Input signal. 
     * @return none. 
     * @brief Normalizes input signal. 
     * @warning none. 
     */
    void Normalize( Signal &sgn );

    /**
     * @date 2013/Oct/07 
     * @param sgn: Input signal. 
     * @return Normalized signal. 
     * @brief Returns a copy of input signal, normalized. 
     * @warning none. 
     */
    Signal Normalize( const Signal &sgn );

  }
  
}

#include "SignalNormalize.cpp"

#endif
