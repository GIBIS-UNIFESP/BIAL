/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Jun/26 
 * @brief Generates a feature vector composed by red, green, and blue colors as the features.
 * A vector is generated for each valid pixel.
 */

#include "Common.hpp"
#include "Vector.hpp"

#ifndef BIALCOLORFEATURE_H
#define BIALCOLORFEATURE_H

#include "Feature.hpp"

namespace Bial {

  union Color;
  class Adjacency;
  template< class D >
  class Image;

  /**
   * @date 2014/May/01 
   * @param src: Input source image to extract features. 
   * @param adj: Adjacency relation of each pixel. 
   * @return A median feature vector. 
   * @brief Assings the values from the neighborhoods of each pixel from image src to a feature vector. 
   * @warning none. 
   */
  template< class D >
  Feature< D > ColorFeature( const Image< Color > &src, const Adjacency &adj );

  /**
   * @date 2014/May/01 
   * @param src: Input source image to extract features. 
   * @param adj_rel: Adjacency relation of each pixel. 
   * @param res: Resulting feature vector. 
   * @param thread: Thread number. 
   * @param total_threads: Number of threads. 
   * @return none. 
   * @brief Multi-thread implementation of ColorFeature. 
   * @warning none. 
   */
  template< class D >
  void ColorFeatureThread( const Image< Color > &src, const Adjacency &adj_rel, Feature< D > &res, size_t thread,
                           size_t total_threads );

}

#include "ColorFeature.cpp"

#endif
