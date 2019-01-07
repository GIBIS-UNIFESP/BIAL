/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Jul/12 
 * @brief Median filter.
 */

#include "Common.hpp"

#ifndef BIALFILTERINGMEDIAN_H
#define BIALFILTERINGMEDIAN_H

namespace Bial {

  template< class D >
  class Image;
  class Adjacency;

  namespace Filtering {

    /**
     * @date 2013/Aug/08 
     * @param img: Input image. 
     * @param neighborhood_radius: radius of the neighborhood. 
     * @return Median filtered image with the given radius. 
     * @brief Returns the median filtered image with the given radius. 
     * @warning none. 
     */
    template< class D >
    Image< D > Median( const Image< D > &img, float radius );

    /**
     * @date 2013/Aug/08 
     * @param img: Input image. 
     * @param adj: Adjacency relation. 
     * @param res: Resulting image. 
     * @param thread: number of the thread. 
     * @param total_threads: total number of threads. 
     * @return none. 
     * @brief Multi-thread implementation of the median filtered image with the given radius. 
     * @warning none. 
     */
    template< class D >
    void MedianThreads( const Image< D > &img, const Adjacency &adj, Image< D > &res, size_t thread,
                        size_t total_threads );
    
  }

}

#endif
