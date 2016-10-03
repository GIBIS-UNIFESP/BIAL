/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/21 
 * @brief Operations to be executed after defining the gradient. 
 */

#ifndef BIALGRADIENTPOSOPERATIONS_H
#define BIALGRADIENTPOSOPERATIONS_H

#include "Common.hpp"
#include "Vector.hpp"

namespace Bial {

  template< class D >
  class Image;

  /**
   * @brief Computes image gradients.
   */
  namespace Gradient {

    /**
     * @date 2015/Apr/10 
     * @param canny: Input Canny gradient image. 
     * @return none. 
     * @brief Returns Canny gradient with one pixel wide holes closed. 
     * @warning none. 
     */
    template< class D >
    Image< D > CloseHoles( const Image< D > &canny );

    /**
     * @date 2015/Jul/08 
     * @param canny: A Canny gradient image. 
     * @return none. 
     * @brief Removes redundant points from components leaving the minimal width components. 
     * @warning Only implemented for 2D images. 
     */
    template< class D >
    void RemoveRedundantPoint( Image< D > &canny );

    /**
     * @date 2015/Jul/08 
     * @param canny: A Canny gradient image. 
     * @return none. 
     * @brief Removes points of bifurcation. It tries to keep straighter lines when possible. 
     * @warning Only implemented for 2D images. 
     */
    template< class D >
    void RemoveBifurcation( Image< D > &canny );

    /**
     * @date 2015/Jul/08 
     * @param label: An image containing labeled Canny gradient segments. 
     * @return Vector with pixel indexs containing one extremity of each component. 
     * @brief Gets one of the extremities of each segment. 
     * @warning Only implemented for 2D images. 
     */
    Vector< size_t > Extremities( const Image< int > &label );

    /**
     * @date 2015/Apr/29 
     * @param canny: A Canny gradient image. 
     * @param min_seg_size: Minimum size of a segment. 
     * @return none. 
     * @brief Removes the edges that are smaller than the given size.  
     * @warning none.
     */
    template< class D >
    void RemoveSmallSegments( Image< D > &canny, size_t min_seg_size );

  }

}

#include "GradientPosOperations.cpp"

#endif
