/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Dec/04
 * @brief Basic true and false positive and negative metrics.
 */

#ifndef BIALSTATISTICSPOSNEG_H
#define BIALSTATISTICSPOSNEG_H

#include "Common.hpp"

namespace Bial {

  template< class D >
  class Image;

  namespace Statistics {

    /**
     * @date 2015/May/26
     * @param src_label: A binary label data.
     * @param tgt_label: A binary label data.
     * @return The number of true positive, true negative, false positive, and false negative pixels as comparing
     * src_label to tgt_label.
     * @brief Computes and returns the number of true positive, true negative, false positive, and false negative
     * pixels as comparing src_label to tgt_label.
     * @warning none.
     */
    template< template< class D > class C, class D >
    std::tuple< size_t, size_t, size_t, size_t > PositiveNegative( const C< D > &src_label, 
                                                                   const C< D > &tgt_label );

    /**
     * @date 2019/Jan/07
     * @param src_label: A binary label data.
     * @param tgt_label: A binary label data.
     * @param slice: slice in z dimension.
     * @return The number of true positive, true negative, false positive, and false negative pixels as comparing
     * src_label to tgt_label.
     * @brief Computes and returns the number of true positive, true negative, false positive, and false negative
     * pixels as comparing src_label to tgt_label for a slice of a 3D image.
     * @warning none.
     */
    template< template< class D > class C, class D >
    std::tuple< size_t, size_t, size_t, size_t >
    PositiveNegative( const C< D > &src_label, const C< D > &tgt_label, size_t slice );

    /**
     * @date 2015/May/26
     * @param src_label: A binary label data.
     * @param tgt_label: A binary label data.
     * @param dist: A maximum threshold for the distance between src and tgt labels.
     * @return The number of true positive, true negative, false positive, and false negative pixels as comparing
     * src_label to tgt_label.
     * @brief Computes and returns the number of true positive, true negative, false positive, and false negative
     * pixels as comparing src_label to tgt_label. This function was created to compute the statistics comparing two
     * edge or boundaries, instead of two regions. Use the overloaded function without 'dist' parameter for comparing
     * regions.
     * @warning none.
     */
    template< class D >
    std::tuple< size_t, size_t, size_t, size_t > PositiveNegative( const Image< D > &src_label, 
                                                                   const Image< D > &tgt_label, float dist );

    /**
     * @date 2013/Nov/14
     * @param src_label: A binary label data.
     * @param tgt_label: A binary label data.
     * @return The number of true positive pixels as comparing src_label and tgt_label.
     * @brief Computes and returns the number of true positive pixels as comparing src_label and tgt_label.
     * @warning none.
     */
    template< template< class D > class C, class D >
    size_t TruePositive( const C< D > &src_label, const C< D > &tgt_label );

    /**
     * @date 2015/May/26
     * @param src_label: A binary label data.
     * @param tgt_label: A binary label data.
     * @param dist: Maximum distance between src and tgt labels.
     * @return The number of true positive pixels as comparing src_label and tgt_label.
     * @brief Computes and returns the number of true positive pixels as comparing src_label and tgt_label. <br>
     * This function was created to compute the statistics comparing two edge or boundaries, instead of two regions. <br>
     * Use the overloaded function without 'dist' parameter for comparing regions.
     * @warning none.
     */
    template< class D >
    size_t TruePositive( const Image< D > &src_label, const Image< D > &tgt_label, float dist );

    /**
     * @date 2013/Nov/14
     * @param src_label: A binary label data.
     * @param tgt_label: A binary label data.
     * @return The number of true negative pixels as comparing src_label and tgt_label.
     * @brief Computes and returns the number of true negative pixels as comparing src_label and tgt_label.
     * @warning none.
     */
    template< template< class D > class C, class D >
    size_t TrueNegative( const C< D > &src_label, const C< D > &tgt_label );

    /**
     * @date 2015/May/26
     * @param src_label: A binary label data.
     * @param tgt_label: A binary label data.
     * @param dist: Maximum distance between src and tgt labels.
     * @return The number of true negative pixels as comparing src_label and tgt_label.
     * @brief Computes and returns the number of true negative pixels as comparing src_label and tgt_label. This
     * function was created to compute the statistics comparing two edge or boundaries, instead of two regions. Use the
     * overloaded function without 'dist' parameter for comparing regions.
     * @warning none.
     */
    template< class D >
    size_t TrueNegative( const Image< D > &src_label, const Image< D > &tgt_label, float dist );

    /**
     * @date 2013/Nov/14
     * @param src_label: A binary label data.
     * @param tgt_label: A binary label data.
     * @return The number of false positive pixels as validating src_label to tgt_label.
     * @brief Computes and returns the number of false positive pixels as validating src_label to tgt_label.
     * @warning none.
     */
    template< template< class D > class C, class D >
    size_t FalsePositive( const C< D > &src_label, const C< D > &tgt_label );

    /**
     * @date 2015/May/26
     * @param src_label: A binary label data.
     * @param tgt_label: A binary label data.
     * @param dist: Maximum distance between src and tgt labels.
     * @return The number of false positive pixels as validating src_label to tgt_label.
     * @brief Computes and returns the number of false positive pixels as validating src_label to tgt_label. This
     * function was created to compute the statistics comparing two edge or boundaries, instead of two regions. Use the
     * overloaded function without 'dist' parameter for comparing regions.
     * @warning none.
     */
    template< class D >
    size_t FalsePositive( const Image< D > &src_label, const Image< D > &tgt_label, float dist );

    /**
     * @date 2013/Nov/14
     * @param src_label: A binary label data.
     * @param tgt_label: A binary label data.
     * @return The number of false negative pixels as validating src_label to tgt_label.
     * @brief Computes and returns the number of false negative pixels as validating src_label to tgt_label.
     * @warning none.
     */
    template< template< class D > class C, class D >
    size_t FalseNegative( const C< D > &src_label, const C< D > &tgt_label );

    /**
     * @date 2015/May/26
     * @param src_label: A binary label data.
     * @param tgt_label: A binary label data.
     * @param dist: Maximum distance between src and tgt labels.
     * @return The number of false negative pixels as validating src_label to tgt_label.
     * @brief Computes and returns the number of false negative pixels as validating src_label to tgt_label. This
     * function was created to compute the statistics comparing two edge or boundaries, instead of two regions. Use the
     * overloaded function without 'dist' parameter for comparing regions.
     * @warning none.
     */
    template< class D >
    size_t FalseNegative( const Image< D > &src_label, const Image< D > &tgt_label, float dist );

  }

}

#include "StatisticsPosNeg.cpp"

#endif
