/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2015/Apr/07
 * @brief Hough transform for circles
 */

#ifndef BIALHOUGHCIRCLE_H
#define BIALHOUGHCIRCLE_H

#include "Common.hpp"
#include "Image.hpp"

namespace Bial {

  class Circle;

  /** @brief  Class is used to compare elements in increasing order to sort hough circle candidates. */
  template< class D >
  struct HoughIncreaseCompare {
    const Image< D > &aux;
    const D *data;

    /**
     * @date 2015/Jun/23
     * @param _aux: auxiliary image to get a pointer to its elements.
     * @return none.
     * @brief Basic constructor.
     * @warning none.
     */
    HoughIncreaseCompare( const Image< D > &_aux );

    /**
     * @date 2015/Jun/23
     * @param l1, l2: elements to be compared.
     * @return true if l1 is lower than l2. If they are equal, returns true if index l1 is lower than index l2.
     * @brief Comparison operator between elements l1 and l2.
     * @warning none.
     */
    bool operator()( size_t l1, size_t l2 ) const;
  };

  /** @brief  Class is used to compare elements in decreasing order to sort hough circle candidates. */
  template< class D >
  struct HoughDecreaseCompare {
    const Image< D > &aux;
    const D *data;

    /**
     * @date 2015/Jun/23
     * @param _aux: auxiliary image to get a pointer to its elements.
     * @return none.
     * @brief Basic constructor.
     * @warning none.
     */
    HoughDecreaseCompare( const Image< D > &_aux );

    /**
     * @date 2015/Jun/23
     * @param l1, l2: elements to be compared.
     * @return true if l1 is higher than l2. If they are equal, returns true if index l1 is higher than index l2.
     * @brief Comparison operator between elements l1 and l2.
     * @warning none.
     */
    bool operator()( size_t l1, size_t l2 ) const;
  };

  /* Hough class. It should contain all hough transforms. */
  namespace Hough {

    /**
     * @date 2015/Jun/23
     * @param img: Input image.
     * @param scale: The scale in which hough transform is executed. Should be greater than 0.0.
     * @param min_dist: Minimum distance between detected circles.
     * @param min_radius, max_radius: Range of the radius of the circles.
     * @param canny_threshold: Canny higher threshold. Lower threshold is set to canny_threshold / 2.
     * @param detection_threshold: Threshold to accept or reject a circle. Suggested range: 10 to 200.
     * @param canny_levels: Number of levels in multi-level canny implementation. Suggestion: 1 to 3.
     * @param clean_edges: Use heuristic to remove border segments that will not contribute to find the circles.
     * @return A vector containing all detected circles.
     * @brief Hough transform to detect circles in the input image.
     * @warning none.
     */
    template< class D >
    Vector< Circle > HoughCircles( Image< D > &img, float scale, float min_dist, size_t min_radius, size_t max_radius,
                                   float canny_threshold, size_t detection_threshold, int canny_levels, 
                                   int clean_edges );
  }

}

#include "HoughCircle.cpp"

#endif
