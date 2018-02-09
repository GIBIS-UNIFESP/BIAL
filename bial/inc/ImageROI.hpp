/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date: 2014/Jan/07
 * @brief Image region of interest. Minimal sub image coordinates without zero frame.
 */

#ifndef BIALIMAGEROI_H
#define BIALIMAGEROI_H

#include "Common.hpp"
#include "Vector.hpp"

namespace Bial {

  template< class D > class Image;

  namespace ImageOp {

    /**
     * @date 2014/Jan/07
     * @param img: Input image.
     * @return Lower and higher coordinates of the region of interest (ROI).
     * @brief Computes lower and higher coordinates of the region of interest (ROI). The ROI is the smallest box
     * that contains all non-zero pixels.
     * @warning none.
     */
    template< class D >
    std::tuple< Vector< size_t >, Vector< size_t > > ROI( const Image< D > &img );

  }

}

#include "ImageROI.cpp"

#endif
