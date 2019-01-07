/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date: 2014/Jan/07
 * @brief Image add/remove frame.
 */

#ifndef BIALIMAGEFRAME_H
#define BIALIMAGEFRAME_H

#include "Common.hpp"
#include "Vector.hpp"

namespace Bial {

  template< class D > class Image;

  namespace ImageOp {

    /**
     * @date 2014/Jan/07
     * @param img: Input image.
     * @param low_coord: Lower coordinates of the frame.
     * @param hgh_coord: Higher coordinates of the frame.
     * @return Image without the frame.
     * @brief Computes and returns image without the frame given by lower and higher coordinates.
     * @warning none.
     */
    template< class D >
    Image< D > RemoveFrame( const Image< D > &img, const Vector< size_t > &low_coord, 
                            const Vector< size_t > &hgh_coord );

    /**
     * @date 2014/Jan/16
     * @param img: Input image.
     * @param width: Frame width.
     * @return Image without the frame.
     * @brief Computes and returns image without the frame.
     * @warning none.
     */
    template< class D >
    Image< D > RemoveFrame( const Image< D > &img, size_t width );

    /**
     * @date 2014/Jan/07
     * @param img: Input image.
     * @param base_coord: Lower coordinates of the image with the frame.
     * @param full_size: Full size of the computed image.
     * @return Image with frame.
     * @brief Computes and returns image with zero intensity pixels frame. base_coord gives the lower coordinates
     * the img is inserted. full_size is the size of the framed image.
     * @warning none.
     */
    template< class D >
    Image< D > AddFrame( const Image< D > &img, const Vector< size_t > &base_coord, const Vector< size_t > &full_size );

    /**
     * @date 2014/Jan/07
     * @param img: Input image.
     * @param width: Frame width.
     * @return Image with frame.
     * @brief Computes and returns image with zero value frame. Frame with given width is added to every
     * extremity.
     * @warning none.
     */
    template< class D >
    Image< D > AddFrame( const Image< D > &img, size_t width );

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

#endif
