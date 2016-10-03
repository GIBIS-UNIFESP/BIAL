/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Jul/12 
 * @brief Image correlation.
 */

#include "Common.hpp"

#ifndef BIALCORRELATION_H
#define BIALCORRELATION_H

namespace Bial {

  template< class D >
  class Image;
  class Kernel;

  /**
   * @date 2013/Nov/21 
   * @param img: Input image. 
   * @param krn: A kernel. 
   * @return Image with correlation between img and krn. 
   * @brief Returns an image with correlation between img and krn. 
   * @warning none. 
   */
  template< class D >
  Image< D > Correlation( const Image< D > &img, const Kernel &krn );

  /**
   * @date 2013/Nov/26 
   * @param img: Input image. 
   * @param krn: A kernel. 
   * @param res: Resulting image. 
   * @param thread: number of the thread. 
   * @param total_threads: total number of threads. 
   * @return none. 
   * @brief Multithread implementation of correlation. 
   * @warning none. 
   */
  template< class D >
  void CorrelationThreads( const Image< D > &img, const Kernel &krn, Image< D > &res, size_t thread,
                           size_t total_threads );

}

#include "Correlation.cpp"

#endif
