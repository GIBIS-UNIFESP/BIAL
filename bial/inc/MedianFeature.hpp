/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Jun/26 
 * @brief Feature vector extracted from image. Gets the median intensity pixels around each valid pixel.
 * A feature vector is generated from each valid pixel.
 */

#include "Common.hpp"

#ifndef BIALMEDIANFEATURE_H
#define BIALMEDIANFEATURE_H

#include "Feature.hpp"

namespace Bial {

  class Adjacency;
  template< class D >
  class Image;

  /**
   * @date 2014/Apr/16 
   * @param src: Input source image to extract features. 
   * @param adj: Adjacency relation of each pixel. 
   * @param proportion: Proportion of the pixels in adj to be taken. In ]0.0,1.0]. 
   * @return A median feature vector. 
   * @brief Assings a median values from the neighborhoods of each pixel from image src to a feature vector. 
   * @warning none. 
   */
  template< class D >
  Feature< D > MedianFeature( const Image< D > &src, const Adjacency &adj, float proportion = 0.6 );

  /**
   * @date 2014/Apr/16 
   * @param src: Input source image to extract features. 
   * @param msk: Input mask image that restricts the
   *             extracting feature pixels. 
   * @param adj: Adjacency relation of each pixel. 
   * @param proportion: Proportion of the pixels in adj to be taken. In ]0.0,1.0]. 
   * @return A median feature vector. 
   * @brief Assings a median values from the neighborhoods of each pixel from image src, restricted by a mask
   * msk to a feature vector. 
   * @warning Image element p is not considered if msk[ p ] == 0. 
   */
  template< class D >
  Feature< D > MedianFeature( const Image< D > &src, const Image< D > &msk, const Adjacency &adj,
                              float proportion = 0.6 );

  /**
   * @date 2013/Nov/28 
   * @param src: Input source matrix to extract features. 
   * @param adj_rel: Adjacency relation of each pixel. 
   * @param res: Resulting feature vector. 
   * @param thread: Thread number. 
   * @param total_threads: Number of threads. 
   * @return none. 
   * @brief Multi-thread implementation of MedianFeature. 
   * @warning none. 
   */
  template< class D >
  void MedianFeatureThread( const Image< D > &src, const Adjacency &adj_rel, Feature< D > &res,
                            size_t thread, size_t total_threads );
  
}

#include "MedianFeature.cpp"

#endif
