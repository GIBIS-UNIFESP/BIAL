/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Oct/18 
 * @brief Computes the complement of a binary image restricted or not to a mask.
 */

#ifndef BIALBINARYCOMPLEMENT_H
#define BIALBINARYCOMPLEMENT_H

#include "Common.hpp"

namespace Bial {

  template< class D > 
  class Image;

  namespace Binary {

    /**
     * @date 2013/Sep/05 
     * @param img: A binary image. 
     * @return none. 
     * @brief Inverts the intensity of each pixel of image img
     * using its maximum value, i.e., data[ x ] =
     * ( data[ x ] + 1 ) % 2. 
     * @warning none. 
     */
    template< class D >
    void Complement( Image< D > &img );

    /**
     * @date 2013/Sep/05 
     * @param img: An image. 
     * @param msk: An image. 
     * @return none. 
     * @brief Same as Complement( img ), but restricted to the
     * pixels of msk image which intensities are not
     * zero. 
     * @warning none. 
     */
    template< class D >
    void Complement( Image< D > &img, const Image< D > &msk );

  }

}

#include "BinaryComplement.cpp"

#endif
