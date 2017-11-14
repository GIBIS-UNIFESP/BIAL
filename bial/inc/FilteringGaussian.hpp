/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Jul/12 
 * @brief Gaussian filter.
 */

#include "Common.hpp"

#ifndef BIALFILTERINGGAUSSIAN_H
#define BIALFILTERINGGAUSSIAN_H

namespace Bial {

  template< class D >
  class Image;

  namespace Filtering {

    /**
     * @date 2013/Nov/22 
     * @param img: Input image. 
     * @param neighborhood_radius: radius of the neighborhood. 
     * @return Median filtered image with the given radius. 
     * @brief Returns the median filtered image with the given radius. 
     * @warning none. 
     */
    template< class D >
    Image< D > Gaussian( const Image< D > &img, float radius = 2.0, float std_dev = 2.0 );

  }

}

#include "FilteringGaussian.cpp"

#endif
