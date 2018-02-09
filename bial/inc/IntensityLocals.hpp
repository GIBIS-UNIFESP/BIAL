/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Sep/08
 * @brief Functions to compute intensity local minima or maxima.
 */

#include "Image.hpp"
#include "Vector.hpp"

#ifndef BIALINTENSITYLOCALS_H
#define BIALINTENSITYLOCALS_H

namespace Bial {

  class Adjacency;

  namespace Intensity {

    /**
     * @date 2012/Sep/24
     * @param image: Input image.
     * @param adjacency: An adjacency relation.
     * @return Image with local minima pixel indexes in image image within adjacency relation adjacency.
     * @brief Returns an image with local minima pixel indexes of image image within adjacency relation
     * adjacency.
     * @warning none.
     */
    template< class D >
    Vector< bool > LocalMinima( const Image< D > &image, const Adjacency &adjacency );

    /**
     * @date 2012/Sep/24
     * @param image: Input image.
     * @param adjacency: An adjacency relation.
     * @return Image with local maxima pixel indexes in image 'image' within adjacency relation 'adjacency'.
     * @brief Returns an image with local maxima pixel indexes of image 'image' within adjacency relation
     * 'adjacency'.
     * @warning none.
     */
    template< class D >
    Vector< bool > LocalMaxima( const Image< D > &image, const Adjacency &adjacency );

    /**
     * @date 2012/Sep/24
     * @param image: Input image.
     * @param mask: Image mask with domain restriction.
     * @param adjacency: An adjacency relation.
     * @return Image with local minima pixel indexes in image image within adjacency relation adjacency.
     * @brief Returns an image with local minima pixel indexes of image image within adjacency relation
     * adjacency.
     * @warning none.
     */
    template< class D, class D2 >
    Vector< bool > LocalMinima( const Image< D > &image, const Image< D2 > &mask, const Adjacency &adjacency );

    /**
     * @date 2012/Sep/24
     * @param image: Input image.
     * @param mask: Image mask with domain restriction.
     * @param adjacency: An adjacency relation.
     * @return Image with local maxima pixel indexes in image 'image' within adjacency relation 'adjacency'.
     * @brief Returns an image with local maxima pixel indexes of image 'image' within adjacency relation
     * 'adjacency'.
     * @warning none.
     */
    template< class D, class D2 >
    Vector< bool > LocalMaxima( const Image< D > &image, const Image< D2 > &mask, const Adjacency &adjacency );

  }

}

#include "IntensityLocals.cpp"

#endif
