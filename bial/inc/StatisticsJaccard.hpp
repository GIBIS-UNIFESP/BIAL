/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Dec/04
 * @brief Jaccard metric.
 */

#ifndef BIALSTATISTICSJACCARD_H
#define BIALSTATISTICSJACCARD_H

#include "Common.hpp"

namespace Bial {

  template< class D >
  class Image;

  namespace Statistics {

    /**
     * @date 2013/Nov/14
     * @param src_label: A binary label data.
     * @param tgt_label: A binary label data.
     * @return The value given by Jaccard metric as comparing src_label and tgt_label.
     * @brief Computes and returns the value given by Jaccard metric as comparing src_label and tgt_label.
     * @warning none.
     */
    template< template< class D > class C, class D >
    float Jaccard( const C< D > &src_label, const C< D > &tgt_label );

    /**
     * @date 2015/May/26
     * @param src_label: A binary label data.
     * @param tgt_label: A binary label data.
     * @param dist: Maximum distance between src and tgt labels.
     * @return The value given by Jaccard metric as comparing src_label and tgt_label.
     * @brief Computes and returns the value given by Jaccard metric as comparing src_label and tgt_label. This
     * function
     * was created to compute the statistics comparing two edge or boundaries, instead of two regions. Use the
     * overloaded function without 'dist' parameter for comparing regions.
     * @warning none.
     */
    template< class D >
    float Jaccard( const Image< D > &src_label, const Image< D > &tgt_label, float dist );

  }

}

#include "StatisticsJaccard.cpp"

#endif
