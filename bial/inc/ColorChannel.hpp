/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date: 2013/Oct/29
 * @brief Operations over color channels.
 */

#include "Common.hpp"

#ifndef BIALCOLORCHANNEL_H
#define BIALCOLORCHANNEL_H

#include "Color.hpp"

namespace Bial {

  template< class D >
  class Image;
  template< class D >
  class Vector;

  namespace ColorSpace {

    /**
     * @date 2014/Apr/11
     * @param img_vector: vector of images.
     * @return Resultant merged color image.
     * @brief Static constructor of a color image, from an vector of monochromatic images.
     * @warning Images in vector must have the same dimension sizes.
     */
    template< class D >
    Image< Color > Merge( Vector< Image< D > > &img_vector );

    /**
     * @date 2013/Oct/23
     * @param img: A 3D or 4D image.
     * @return An image vector with split images in the last dimension.
     * @brief Create a vector of (N-1)-D images from an N-D image.
     * @warning none.
     */
    template< class D >
    Vector< Image< D > > Split( const Image< Color > &img );

    /**
     * @date 2016/Jan/13
     * @param img: A 3D or 4D image.
     * @param channel: A channel.
     * @return An image with the chosen channel from input image.
     * @brief Returns the the chosen channel from input image as a gray image.
     * @warning none.
     */
    template < class D >
    Image< D > Channel( const Image< Color > &img, size_t channel );
    template < class D >
    Image< D > Channel( const Image< RealColor > &img, size_t channel );

  }

}

#include "ColorChannel.cpp"

#endif
