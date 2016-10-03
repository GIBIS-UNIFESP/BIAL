/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/11 
 * @brief Functions to compute signal hysteresis
 */

#ifndef BIALSIGNALHYSTERESIS_H
#define BIALSIGNALHYSTERESIS_H

#include "Common.hpp"

namespace Bial {
  /**
   * @brief Signal handeling declaration. 
   */
  class Signal;

  namespace SignalOp {

    /**
     * @date 2014/Jan/07 
     * @param sgn: Input signal.
     * @param krl_size: Size of the kernel to locate hysteresis. 
     * @param low_threshold: Low frequency threshold. 
     * @param hgh_threshold: High frequency threshold. 
     * @return Data value of the hysteresis in the low data value of the histogram. 
     * @brief Returns data value of the hysteresis in the low data value of the histogram. 
     * @warning none. 
     */
    double LowHysteresis( const Signal &sgn, size_t krl_size, float low_threshold, float hgh_threshold );

    /**
     * @date 2014/Jan/07
     * @param sgn: Input signal.
     * @param krl_size: Size of the kernel to locate hysteresis. 
     * @param low_threshold: Low frequency threshold. 
     * @param hgh_threshold: High frequency threshold. 
     * @return Data value of the hysteresis in the high data value of the histogram. 
     * @brief Returns data value of the hysteresis in the high data value of the histogram. 
     * @warning none. 
     */
    double HighHysteresis( const Signal &sgn, size_t krl_size, float low_threshold, float hgh_threshold );

  }

}

#include "SignalHysteresis.cpp"

#endif
