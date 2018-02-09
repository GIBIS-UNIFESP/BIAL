/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Aug/27 
 * @brief Functions to estimate and correct inhomogeneity. 
 */

#ifndef BIALBRAININHOMOGENEITY_H
#define BIALBRAININHOMOGENEITY_H

#include "Common.hpp"
#include "MRIModality.hpp"

namespace Bial {

  template< class D >
  class Image;

  namespace Brain {

    /**
     * @date 2014/Jan/07 
     * @param img: An MRI image of the head. 
     * @param msk: Brain mask. 
     * @param compression: compression rate to compute bias field faster (1, 2, or 4). 
     * @param modality: Image modality. Defined in Image.hpp. Must be MRI_T1, MRI_T2, or MRI_PD. 
     * @return Brain mask without outliers. 
     * @brief Remove outliers from brain mask which have extreme intensity values. Used to correct inhomogeneity.
     * @warning none. 
     */
    template< class D >
    Image< D > RemoveIntensityOutliers( const Image< D > &img, const Image< D > &msk, float compression,
                                        MRIModality modality );

    /**
     * @date 2014/Jan/08 
     * @param img: A MRI image. 
     * @param msk: Brain mask. 
     * @param cln_msk: Brain mask without outliers. 
     * @param radius: Radius for distance transform. (8.0 to 20.0) 
     * @param modality: Image modality. 
     * @return Image with bias field surface. 
     * @brief Computes bias surface based on white matter pixels in the image. 
     * @warning none. 
     */
    template< class D >
    Image< D > BiasSurfaceEstimation( const Image< D > &img, const Image< D > &msk, const Image< D > &cln_msk,
                                      float radius, MRIModality modality );

    /**
     * @date 2014/Jan/08 
     * @param img: A MRI image. 
     * @param msk: Brain mask. 
     * @param bias: Bias field surface. 
     * @param modality: Image modality. 
     * @return Bias field removed from image. 
     * @brief Removes estimated bias field from the input image. 
     * @warning none. 
     */
    template< class D >
    Image< D > BiasSurfaceRemoval( const Image< D > &img, const Image< D > &msk, const Image< D > &bias,
                                   MRIModality modality );

    /**
     * @date 2014/Jan/08 
     * @param img: A MRI image. 
     * @param msk: Brain mask. 
     * @param radius: Radius for distance transform. For searching local WM pixel (7.0 to 28.0) 
     * @param compression: compression rate to compute bias field faster (1, 2, or 4). 
     * @param modality: Image modality. 
     * @return Image with bias field surface. 
     * @brief Computes bias surface based on white matter pixels in the image. 
     * @warning none. 
     */
    template< class D >
    Image< D > InhomogeneityCorrection( const Image< D > &img, const Image< D > &msk, float radius = 15.5,
                                        size_t compression = 2, MRIModality modality = MRIModality::T1 );

  }
  
}

#include "BrainInhomogeneity.cpp"

#endif
