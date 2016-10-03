 /* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Jul/12 
 * @brief: Segmentation of 3D image into octants.
 */

#ifndef BIALSEGMENTATIONOCTANTS_H
#define BIALSEGMENTATIONOCTANTS_H

#include "Common.hpp"
#include "Vector.hpp"

namespace Bial {

  template< class D >
  class Image;
  template< class D >
  class Matrix;
  template< class D >
  class Feature;
  /**
   * @brief Image segmentation methods.
   */
  namespace Segmentation {

    /**
     * @date 2015/Jun/16 
     * @param data: A matrix or an image. 
     * @return A matrix of matrices or images with its octants for inputs with 3 dimensions. 
     * @brief Easy way to create a matrix with the 3D image or matrix octants. Octants order: x, y, z. 
     * @warning none. 
     */
    template< class D >
    Matrix< Image< D > > Octants( const Image< D > &data );

    /**
     * @date 2015/Aug/05 
     * @param data: A matrix of matrices or images with octants with 3 dimensions to compose the output. 
     * @return Merged octants in a single matrix or image. 
     * @brief Easy way to merge a matrix of image or matrice octants into a single image or matrix with the
     * 3D. Octants order: x, y, z. 
     * @warning none. 
     */
    template< class D >
    Image< D > Octants( const Matrix< Image< D > > &data );

  }

}

#include "SegmentationOctants.cpp"

#endif
