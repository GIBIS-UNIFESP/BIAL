/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Aug/27 
 * @brief Functions to segment brain tissues into GM/WM/CSF.
 */

#ifndef BIALBRAINTISSUESEGMENTATION_H
#define BIALBRAINTISSUESEGMENTATION_H

#include "Common.hpp"
#include "MRIModality.hpp"
#include "Vector.hpp"

namespace Bial {

  template< class D >
  class Image;
  class Signal;

  namespace Brain {

    /**
     * @date 2013/Sep/04 
     * @param image: An MRI image of the head. 
     * @param mask: Brain or tissue mask. 
     * @param label: Brain or tissues segmented in labeled clusters. 
     * @param intensity: Threshold between low and high intensity tissues. 
     * @return none. 
     * @brief Set labeled clusters into low and high intensity tissues. Low intensity tissue pixels have their
     * label set to 0 and high intensity tissue pixels have their label set to 1.  
     * @warning none. 
     */
    template< class D >
    void TissueLabeling( const Image< D > &image, const Image< D > &mask, Image< int > &label, D intensity );

    /**
     * @date 2013/Sep/04 
     * @param img: An MRI image of the head. 
     * @param msk: Brain mask. 
     * @param tissue_intensity: Value between low and high intensity tissues. 
     * @param scl_min, scl_max: Minimum and maximum number of neighbers for OPF clustering. 
     * @param hierarchical: Selects between spectral (false) or hierarchicar clustering. 
     * @return none. 
     * @brief Set labeled clusters into low and high intensity tissues. Low intensity tissue pixels have their
     * label set to 0 and high intensity tissue pixels have their label set to 1.  
     * @warning none. 
     */
    template< class D >
    Image< int > TissueSegmentation( const Image< D > &img, const Image< D > &msk, D tissue_intensity, float scl_min,
                                     float scl_max, bool hierarchical );

    /**
     * @date 2013/Sep/04 
     * @param img: An MRI image of the head. 
     * @param msk: Brain mask. 
     * @param modality: Image modality. Defined in Image.hpp. Must be MRI_T1, MRI_T2, or MRI_PD. 
     * @param xxx_scl_min, xxx_scl_max: Minimum and maximum number of neighbers for OPF clustering to segment
     *             xxx tissue. 
     * @param hierarchical: Selects between spectral (false) or hierarchicar clustering. 
     * @return none. 
     * @brief Segment brain into CSF, GM, and WM tissues.  
     * @warning none. 
     */
    template< class D >
    Image< int > TissueSegmentation( const Image< D > &img, const Image< D > &msk, MRIModality modality,
                                     float csf_scl_min = 0.1, float csf_scl_max = 0.2, float gm_scl_min = 0.2,
                                     float gm_scl_max = 0.3, bool hierarchical = false );

  }
  
}

#include "BrainTissueSegmentation.cpp"

#endif
