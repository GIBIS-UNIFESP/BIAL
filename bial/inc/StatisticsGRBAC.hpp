/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2018/Dec/28
 * @brief General, Robust, Balanced, Adjustable, Coherent, and Consistent metric.
 */

#ifndef BIALSTATISTICSGRBAC_H
#define BIALSTATISTICSGRBAC_H

#include "Common.hpp"

namespace Bial {

  template< class D >
  class Image;

  namespace Statistics {

    /**
     * @date 2018/Dec/28
     * @param src: Source ground-truth image.
     * @param tgt: Target segmentation result image.
     * @param alpha: weight of FP (higher alpha) and FN (lower alpha). Range: -1.0 to 1.0.
     * @param delta: general weight of error. Lower delta implies in higher error impact to the accuracy. Range: 1.0 to 3.0.
     * @return General, robust, balanced, adjustable, consistent metric.
     * @brief Computes and returns the general, robust, balanced, adjustable, consistent metric.
     * @warning none.
     */
    template< template< class D > class C, class D >
    float GRBAC( const C< D > &src, const C< D > &tgt, float alpha, float delta );

    /**
     * @date 2019/Jan/07
     * @param src: Source ground-truth image.
     * @param tgt: Target segmentation result image.
     * @param alpha: weight of FP (higher alpha) and FN (lower alpha). Range: -1.0 to 1.0.
     * @param delta: general weight of error. Lower delta implies in higher error impact to the accuracy. Range: 1.0 to 3.0.
     * @param slice: slice in z dimension.
     * @return General, robust, balanced, adjustable, consistent metric.
     * @brief Computes and returns the general, robust, balanced, adjustable, consistent metric for a slice of a 3D image.
     * @warning none.
     */
    template< template< class D > class C, class D >
    float GRBAC( const C< D > &src, const C< D > &tgt, float alpha, float delta, size_t slice );

  }

}

#include "StatisticsGRBAC.cpp"

#endif
