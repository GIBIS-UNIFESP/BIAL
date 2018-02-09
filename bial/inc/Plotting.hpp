/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2015/Jan/12 
 * @brief Plotting algorithms. Plots in GnuPlot format
 */

#include "Common.hpp"

#ifndef BIALPlotting_H
#define BIALPlotting_H

#include "Vector.hpp"

namespace Bial {

  class Signal;

  template< class D >
  class Feature;
  /**
   * @brief  Plotting algorithms. 
   */
  class Plotting {

  public:

    /**
     * @date 2015/Jan/12 
     * @param basename: File basename. A txt file for dots and a gnp file for gnuplot instructions. 
     * @param feat: A feature vector from which two features will be plotted. 
     * @param x, y: The number of the features that will be plotted. 
     * @return none. 
     * @brief Plots 2D scatter from x and y features from given feature vector with given arguments. 
     * @warning none. 
     */
    template< class D >
    static void GnuPlot2DScatter( const std::string &basename, const Feature< D > &feat, size_t x = 0, size_t y = 1 );

    /**
     * @date 2015/Mai/05 
     * @param basename: File basename. A txt file for dots and a gnp file for gnuplot instructions. 
     * @param signal: A vector of signals that will be plotted. 
     * @return none. 
     * @brief Plots 2D lines from signals. 
     * @warning none. 
     */
    static void GnuPlot2DLine( const std::string &basename, const Vector< Signal > &signal );

  };

}

#include "Plotting.cpp"

#endif
