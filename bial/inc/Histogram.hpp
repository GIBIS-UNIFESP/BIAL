/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/11 
 * @brief Creating histograms from data.
 */

#ifndef BIALHISTOGRAM_H
#define BIALHISTOGRAM_H

#include "Common.hpp"

namespace Bial {

  class Signal;

  namespace SignalType {

    /**
     * @date 2012/Sep/11 
     * @param data: Input data. 
     * @param data_step: Data step between two consecutive samples. 
     * @return Histogram of data. 
     * @brief Static constructor. 
     * @warning none. 
     */
    template< template< class D > class C, class D >
    Signal Histogram( const C< D > &data, double data_step = 1.0 );

    /**
     * @date 2014/Feb/14 
     * @param data: Input data. 
     * @param data_step: Data step between two consecutive samples. 
     * @return Histogram of data starting the first bin in zero, if no negative value is given. 
     * @brief Static constructor. 
     * @warning none. 
     */
    template< template< class D > class C, class D >
    Signal ZeroStartHistogram( const C< D > &data, double data_step = 1.0 );

    /**
     * @date 2014/Apr/14 
     * @param data: Input data. 
     * @param data_step: Data step between two consecutive samples. 
     * @return Fuzzy histogram of data. 
     * @brief Static constructor. 
     * @warning none. 
     */
    template< template< class D > class C, class D >
    Signal FuzzyHistogram( const C< D > &data, double data_step = 1.0 );

    /**
     * @date 2012/Sep/11 
     * @param data: Input data. 
     * @param data_step: Data step between two consecutive samples. 
     * @return Normalized histogram of data. 
     * @brief Static constructor. 
     * @warning none. 
     */
    template< template< class D > class C, class D >
    Signal NormalizedHistogram( const C< D > &data, double data_step = 1.0 );

    /**
     * @date 2012/Sep/11 
     * @param data: Input data. 
     * @param data_step: Data step between two consecutive samples. 
     * @return Accumulated histogram of data. 
     * @brief Static constructor. 
     * @warning none. 
     */
    template< template< class D > class C, class D >
    Signal AccumulatedHistogram( const C< D > &data, double data_step = 1.0 );

    /**
     * @date 2012/Sep/11 
     * @param data: Input data. 
     * @param data_step: Data step between two consecutive samples. 
     * @return Equalized histogram of data. 
     * @brief Static constructor. 
     * @warning none. 
     */
    template< template< class D > class C, class D >
    Signal EqualizedHistogram( const C< D > &data, double data_step = 1.0 );

  }

}

#include "Histogram.cpp"

#endif
