/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Jun/26 
 * @brief Generates a feature vector composed by red, green, and blue median colors in the adjacency of pixels 
 * as the features. A vector is generated for each valid pixel.
 */

#include "Common.hpp"

#ifndef BIALCOLORMEDIANFEATURE_H
#define BIALCOLORMEDIANFEATURE_H

#include "Feature.hpp"

namespace Bial {

  class Adjacency;
  union Color;
  template< class D >
  class Image;

  /**
   * @date 2014/May/16 
   * @param src: Input source image to extract features. 
   * @param adj_rel: Adjacency relation of each pixel. 
   * @param proportion: Proportion of the pixels in adj to be taken. In ]0.0,1.0]. 
   * @return A median feature vector of all colors. 
   * @brief Assings a median values from the neighborhoods of each pixel from image src to a feature vector. 
   * @warning none. 
   */
  template< class D >
  Feature< D > ColorMedianFeature( const Image< Color > &src, const Adjacency &adj_rel, float proportion = 0.6 );

  /**
   * @date 2014/May/16 
   * @param src: Input source image to extract features. 
   * @param msk: Input mask image that restricts the
   *             extracting feature pixels. 
   * @param adj_rel: Adjacency relation of each pixel. 
   * @param proportion: Proportion of the pixels in adj to be taken. In ]0.0,1.0]. 
   * @return A median feature vector of all colors. 
   * @brief Assings a median values from the neighborhoods of each pixel from image src, restricted by a mask
   * msk to a feature vector. 
   * @warning Image element p is not considered if msk[ p ] == 0. 
   */
  template< class D >
  Feature< D > ColorMedianFeature( const Image< Color > &src, const Image< D > &msk, const Adjacency &adj_rel,
                                   float proportion = 0.6 );

  /**
   * @date 2014/May/16 
   * @param src: Input source image to extract features. 
   * @param adj_rel: Adjacency relation of each pixel. 
   * @param res: Resulting feature vector. 
   * @param thread: Thread number. 
   * @param total_threads: Number of threads. 
   * @return none. 
   * @brief Multi-thread implementation of ColorMedianFeature. 
   * @warning none. 
   */
  template< class D >
  void ColorMedianFeatureThread( const Image< Color > &src, const Adjacency &adj_rel, Feature< D > &res, size_t thread,
                                 size_t total_threads );

}

#include "ColorMedianFeature.cpp"

#endif
