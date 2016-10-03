/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @file Binary.hpp
 * @author Fábio Augusto Menocci Cappabianco 
 * @date 2012/Oct/18 
 * @version 1.0.00
 * @brief
 * Content: COG Function.
 * <br> Description: Function to compute COG of binary image. 
 * <br> Future add-on's: many. 
 */

#ifndef BIALBINARYCOG_H
#define BIALBINARYCOG_H
#include "Common.hpp"

namespace Bial {
  
  template< class D > 
  class Image;
  template< class D > 
  class Vector;

  namespace Binary {

    /**
     * @date 2012/Oct/18 
     * @param A binary image. 
     * @return The coordenates of the center of gravity of the input
     * image. 
     * @brief Computes the coordenates of the center of gravity of
     * the input image. 
     * @warning none. 
     */
    template< class D >
    Vector< size_t > COG( const Image< D > &img );
  
  }

}

#include "BinaryCOG.cpp"

#endif
