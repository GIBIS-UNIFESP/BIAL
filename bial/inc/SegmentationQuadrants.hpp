 /* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Jul/12 
 * @brief: Segmentation of a 2D image in quadrants.
 */

#ifndef BIALSEGMENTATIONQUADRANTS_H
#define BIALSEGMENTATIONQUADRANTS_H

#include "Common.hpp"
#include "Vector.hpp"

namespace Bial {

  template< class D >
  class Image;
  template< class D >
  class Matrix;

  namespace Segmentation {

    /**
     * @date 2015/Jun/16 
     * @param data: A matrix or an image. 
     * @return A matrix of matrices or images with its quadrants for inputs with 2 dimensions. 
     * @brief Easy way to create a matrix with the 2D image or matrix quadrants. 
     * @warning none. 
     */
    template< class D >
    Matrix< Image< D > > Quadrants( const Image< D > &data );

  }

}

#include "SegmentationQuadrants.cpp"

#endif
