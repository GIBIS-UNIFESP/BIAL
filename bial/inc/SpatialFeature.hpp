/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Jun/26 
 * @brief Feature vector composed by the coordinates of valid the matrix/image elements/pixels.
 * Valid elements/pixels have non-zero value/intensity.
 */

#include "Common.hpp"

#ifndef BIALSPATIALFEATURE_H
#define BIALSPATIALFEATURE_H

#include "Feature.hpp"

namespace Bial {

  template< class D >
  class Matrix;
  template< class D >
  class Image;

  /**
   * @date 2013/Jul/20 
   * @param src: Input source matrix to extract features. 
   * @return A spatial feature vector. 
   * @brief Features are the coordinates of non-zero samples in src matrix. 
   * @warning none. 
   */
  template< class D >
  Feature< D > SpatialFeature( const Matrix< D > &src );

  /**
   * @date 2014/Apr/16 
   * @param src: Input source image to extract features. 
   * @return A spatial feature vector. 
   * @brief Features are the coordinates of non-zero samples in src image. 
   * @warning none. 
   */
  template< class D >
  Feature< D > SpatialFeature( const Image< D > &src );

}

#include "SpatialFeature.cpp"

#endif
