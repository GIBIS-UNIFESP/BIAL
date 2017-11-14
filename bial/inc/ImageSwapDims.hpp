/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date: 2014/Jan/07
 * @brief Image swap dimensions.
 */

#ifndef BIALIMAGESWAPDIMS_H
#define BIALIMAGESWAPDIMS_H

#include "Common.hpp"
#include "Vector.hpp"

namespace Bial {

  template< class D > class Image;

  namespace ImageOp {

    /**
     * @date 2014/Jan/07
     * @param img: Input image.
     * @param dim_0: A valid dimension of the image.
     * @param dim_1: Another valid dimension of the image.
     * @param invert_dir_0: Wheather dim_0 should be inverted or not.
     * @param invert_dir_1: Wheather dim_1 should be inverted or not.
     * @return Dimension inverted image
     * @brief Computes the image resultant from swapping dim_0 with dim_1.
     * @warning Works only to swap spatial dimensions.
     */
    template< class D >
    Image< D > SwapDims( const Image< D > &img, size_t dim_0, size_t dim_1, bool invert_dir_0 = false,
                         bool invert_dir_1 = false );

  }

}

#include "ImageSwapDims.cpp"

#endif
