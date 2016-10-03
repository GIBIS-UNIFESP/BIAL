/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Aug/27 
 * @brief Functions to strip the skull from the brain image.
 */

#ifndef BIALBRAINSKULLSTRIPPING_H
#define BIALBRAINSKULLSTRIPPING_H

#include "Common.hpp"
#include "Vector.hpp"

namespace Bial {

  template< class D >
  class Image;

  class Box;

  namespace Brain {

    /**
     * @date 2014/Oct/30 
     * @param img: A MRI image. 
     * @param orientation: Image orientation. Must be a composition of three values containing: LR, RL, SI, IS, AP,
     *             or PA. Look for NiftiHeader::Orientation() function for examples. 
     * @param best_rectangles: Rectangles containing the brain and external regions.  
     * @param similarity: Similarity image showing the most probable location of the brain box center. 
     * @return The center of gravity of the brain. 
     * @brief Finds the best location of a box inside the brain and of surrounding boxes outside of the brain. 
     * @warning none. 
     */
    template< class D >
    Vector< size_t > Location( const Image< D > &img, const std::string &orientation, Vector< Box > &best_rectangles,
                               Image< float > &similarity );

    /**
     * @date 2014/Oct/30 
     * @param img: A MRI image. 
     * @param orientation: Image orientation. Must be a composition of three values containing: LR, RL, SI, IS, AP,
     *             or PA. Look for NiftiHeader::Orientation() function for examples. 
     * @return Segmentation of the brain. 
     * @brief Segments the brain from the background. 
     * @warning none. 
     */
    template< class D >
    Image< D > SkullStripping( const Image< D > &img, const std::string &orientation );

  }

}

#include "BrainSkullStripping.cpp"

#endif
