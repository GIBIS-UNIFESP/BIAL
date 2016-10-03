/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Aug/27 
 * @brief Functions to find the COG of a brain image. 
 */

#ifndef BIALBRAINCOG_H
#define BIALBRAINCOG_H
#include "Common.hpp"
#include "MRIModality.hpp"
#include "Vector.hpp"

namespace Bial {

  template< class D >
  class Image;

  namespace Brain {

    /**
     * @date 2013/Aug/27 
     * @param img: A brain image. 
     * @param orientation: Image orientation. Must be a composition of three values containing: LR, RL, SI, IS, AP,
     *             or PA. Look for NiftiHeader::Orientation() function for examples. 
     * @return The slice of the first slice in axial orientation that contains pixels from the top of the head. 
     * @brief It searches for the first superior axial slice with a pixel which intensity is higher than otsu
     * threshold. 
     * @warning none. 
     */
    template< class D >
    size_t HeadTopSlice( const Image< D > &img, const std::string &orientation );

    /**
     * @date 2013/Aug/27 
     * @param img: A brain image. 
     * @return Vector with COG coordinates. 
     * @brief Returns a Vector with COG coordinates, based on a binary mask of the image main object. 
     * @warning none. 
     */
    template< class D >
    Vector< size_t > COGbyIntensityThreshold( const Image< D > &img );

    /**
     * @date 2013/Aug/27 
     * @param img: A brain image. 
     * @return Vector with COG coordinates. 
     * @brief Returns a Vector with COG coordinates, based on intensity weighted values of the image. 
     * @warning none. 
     */
    template< class D >
    Vector< size_t > COGbyWeightedIntensity( const Image< D > &img );

    /**
     * @date 2013/Aug/27 
     * @param img: A brain image. 
     * @param orientation: Image orientation. Must be a composition of three values containing: LR, RL, SI, IS, AP,
     *             or PA. Look for NiftiHeader::Orientation() function for examples. 
     * @return Vector with COG coordinates. 
     * @brief Returns a Vector with COG coordinates, based on intensity weighted values of the image and
     * distance from the top of the head. 
     * @warning none. 
     */
    template< class D >
    Vector< size_t > COGbyHeadTopDistance( const Image< D > &img, const std::string &orientation );

    /**
     * @date 2013/Aug/27 
     * @param img: A brain image. 
     * @param orientation: Image orientation. Must be a composition of three values containing: LR, RL, SI, IS, AP,
     *             or PA. Look for NiftiHeader::Orientation() function for examples. 
     * @param modality: Image modality. Defined in Image.hpp. 
     * @return Vector with COG coordinates. 
     * @brief Returns a Vector with COG coordinates, based on intensity weighted values of the image and
     * distance from the top of the head. 
     * @warning none. 
     */
    template< class D >
    Vector< size_t > COGbyIntegralImage( const Image< D > &img, const std::string &orientation, MRIModality modality );

  }

}

#include "BrainCOG.cpp"

#endif
