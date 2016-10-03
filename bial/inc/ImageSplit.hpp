/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date: 2014/Jan/07
 * @brief Image splitting.
 */

#ifndef BIALIMAGESPLIT_H
#define BIALIMAGESPLIT_H

#include "Common.hpp"
#include "Vector.hpp"

namespace Bial {

  template< class D > class Image;

  namespace ImageOp {

    /**
     * @date 2013/Oct/23
     * @param img: A 3D or 4D input image.
     * @param dimension: Dimension of the image to be split in vectors.
     * @return An image vector with split images.
     * @brief Create a vector of ( dim < N )-D images from an N-D image.
     * @warning .
     */
    template< class D >
    Vector< Image< D > > Split( const Image< D > &img, size_t dimension );

  }

}

#include "ImageSplit.cpp"

#endif
