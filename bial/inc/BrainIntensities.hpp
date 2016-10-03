/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Aug/27 
 * @brief Class for computing brain histogram based intensities, such as peaks, valleys, mean, minimum, and maximum.
 */

#ifndef BIALBRAININTENSITIES_H
#define BIALBRAININTENSITIES_H

#include "Common.hpp"

namespace Bial {

  template< class D >
  class Image;

  namespace Brain {

    /** @brief Robust statistics for minimum and maximum tissue intensities. */
    class BrainIntensities {
    public:
      double minimum;
      double maximum;
      double first_peak;
      double second_peak;
      double third_peak;
      double first_valley;
      double second_valley;

      /**
       * @date 2013/Sep/04 
       * @param img: A brain image. 
       * @return Tissue intensities for peaks and valleys of GM, WM, and CSF, minimum, maximum intensities. 
       * @brief  Compute tissue threshold intensities. Also corrects thresholds if tissue peaks in the histogram are mixed.
       * @warning Working for T1 images. 
       */
      template< class D > 
      BrainIntensities( const Image< D > &img );
    };

  }

}

#include "BrainIntensities.cpp"

#endif
