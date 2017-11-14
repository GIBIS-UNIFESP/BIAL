/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Aug/27 
 * @brief Image intensity manipulation functions.
 */

#ifndef BIALINTENSITYGLOBALS_H
#define BIALINTENSITYGLOBALS_H

#include "Common.hpp"
#include "Image.hpp"

namespace Bial {
  /**
   * @brief  Image intensity manipulation class. 
   */
  namespace Intensity {

    /**
     * @date 2013/Aug/27 
     * @param img: An image. 
     * @param msk: An image. 
     * @return Minimum intensity in img restricted to msk pixels which intensities are different form zero. 
     * @brief Returns the minimum intensity in img restricted to msk pixels which intensities are different form
     * zero. 
     * @warning none. 
     */
    template< class D >
    D Minimum( const Image< D > &img, const Image< D > &msk );

    /**
     * @date 2013/Aug/27 
     * @param img: An image. 
     * @param msk: An image. 
     * @return Maximum intensity in img restricted to msk pixels which intensities are different form zero. 
     * @brief Returns the maximum intensity in img restricted to msk pixels which intensities are different form
     * zero. 
     * @warning none. 
     */
    template< class D >
    D Maximum( const Image< D > &img, const Image< D > &msk );

    /**
     * @date 2013/Aug/27 
     * @param img: An image. 
     * @return none. 
     * @brief Inverts the intensity of each pixel of image img using its maximum value, i.e., data[ x ] =
     * Maximum( img ) - data[ x ]. 
     * @warning none. 
     */
    template< class D >
    void Complement( Image< D > &img );

    /**
     * @date 2013/Aug/27 
     * @param img: An image. 
     * @param msk: An image. 
     * @return none. 
     * @brief The same as Complement( img ), but restricted to the pixels of msk image which intensities are not
     * zero. 
     * @warning none. 
     */
    template< class D >
    void Complement( Image< D > &img, const Image< D > &msk );

  }

}

#include "IntensityGlobals.cpp"

#endif
