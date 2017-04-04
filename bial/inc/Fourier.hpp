/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2017/Mar/28
 * @brief Fourier Transform functions. Handles 2D and 3D images.
 */

#include "Common.hpp"
#include "Vector.hpp"

#ifndef BIALFOURIER_H
#define BIALFOURIER_H

namespace Bial {

  template< class D >
  class Image;
  
  /**
   * @date 2017/Mar/28
   * @param img: Input image over which FFT or IFFT is applied.
   * @param forward_transform: true for foward transform and false for the inverse.
   * @return The transformed image.
   * @brief Computes the forward or inverse 2D-FFT over the input image.
   * @warning none
   */
  Image< bial_complex > FFT( const Image< bial_complex > &img, bool forward_transform = true );

  /**
   * @date 2017/Mar/28
   * @param aux: An input row, column, or depth of the image data.
   * @param forward_transform: true for foward transform and false for the inverse.
   * @return true if transform was executed successfully.
   * @brief Auxiliar function to computes the forward or inverse FFT over the input image inplace.
   * @warning none
   */
  void FFT( Vector< bial_complex > &aux, bool forward_transform );

  /**
   * @date 2017/Mar/28
   * @param img: Input image.
   * @return The centralized zero padded image.
   * @brief Pads zeros and centralizes the input image to the closests larger power of 2 dimension sizes.
   * @warning none
   */
  template< class D >
  Image< D > ZeroPad( const Image< D > &img );

  /**
   * @date 2017/Mar/28
   * @param src_img: Input padded source image.
   * @param tgt_img: Resultant unppaded image.
   * @return none.
   * @brief Removes the zeros around a padded input image. It copies only the pixels in the tgt_img domain.
   * @warning none
   */
  template< class D >
  void ZeroUnPad( const Image< D > &src_img, Image< D > &tgt_img );
  
}

#include "Fourier.cpp"

#endif
