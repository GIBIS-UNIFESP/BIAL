/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/12/12 
 * @brief Heart center of gravity estimation.
 */

#ifndef BIALHEARTCOG_H
#define BIALHEARTCOG_H

#include "Common.hpp"
#include "Vector.hpp"

namespace Bial {

  template< class D >
  class Image;
  class Box;

  namespace Heart {

    /*
     * Reference for hear dimensions:
     * Heart Size of Adults in a Natural Population-Tecumseh, Michigan: Variation by
     * Sex, Age, Height, and Weight
     * ALBERT OBERMAN, ALLEN R. MYERS, THOMAS M. KARUNAS and
     * FREDERICK H. EPSTEIN
     * Mean heart diameter = 13.5mm (sagittal direction) Using same dimension for other directions. Maybe axial should be
     * a little bit larger.
     */


    /**
     * @date 2014/Oct/14 
     * @param img: A heart image. 
     * @param orientation: Image orientation. Must be a composition of three values containing:
     *             LR, RL, SI, IS, AP, or PA. Look for NiftiHeader::Orientation() function
     *             for examples. 
     * @return Vector with COG coordinates. 
     * @brief Returns a Vector with COG coordinates, based on intensity weighted values of the image and
     * distance from the top of the head. 
     * @warning none. 
     */
    template< class D >
    Vector< size_t > WeightedCOG( const Image< D > &img, const std::string &orientation );

    /**
     * @date 2013/Aug/27 
     * @param img: A heart image. 
     * @param orientation: Image orientation. Must be a composition of three values containing:
     *             LR, RL, SI, IS, AP, or PA. Look for NiftiHeader::Orientation() function
     *             for examples. 
     * @param best_rectangles: Vector with best rectangles. 
     * @param similarity: Image with similarity of each center point. 
     * @return Vector with COG coordinates. 
     * @brief Returns a Vector with COG coordinates, based on intensity weighted values of the image and
     * distance from the top of the head. 
     * @warning none. 
     */
    template< class D >
    Vector< size_t > WeightedCOG( const Image< D > &img, const std::string &orientation, Vector< Box > &best_rectangles,
                                  Image< float > &similarity );
    
  }

}

#include "HeartCOG.cpp"

#endif
