/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Jul/12 
 * @brief Mean filter.
 */

#include "Common.hpp"

#ifndef BIALFILTERINGMEAN_H
#define BIALFILTERINGMEAN_H

namespace Bial {

  template< class D >
  class Image;

  namespace Filtering {

    /**
     * @date 2014/Jan/08 
     * @param img: Input image. 
     * @param neighborhood_radius: radius of the neighborhood. 
     * @return Mean filtered image with the given radius. 
     * @brief Returns the mean filtered image with the given radius. 
     * @warning none. 
     */
    template< class D >
    Image< D > Mean( const Image< D > &img, float radius );

    /**
     * @date 2014/Jan/08 
     * @param img: Input image. 
     * @param msk: Restrictive mask. 
     * @param neighborhood_radius: radius of the neighborhood. 
     * @return Mean filtered image with the given radius restricted to non-zero mask pixels. 
     * @brief Returns the mean filtered image with the given radius restricted to non-zero mask pixels. 
     * @warning none. 
     */
    template< class D >
    Image< D > Mean( const Image< D > &img, const Image< D > &msk, float radius );

  }

}

#include "FilteringMean.cpp"

#endif
