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

  /**
   * @date 2017/Mar/24 
   * @param os: output stream.
   *            tissues: an image.
   * @return Reference to os.
   * @brief Prints the image in a friendly way.
   * @warning none.
   */
  template< class O >
  O &operator<<( O &os, const Brain::BrainIntensities &tissues );

}

namespace Bial {

  template< class O >
  O &operator<<( O &os, const Brain::BrainIntensities &tissues ) {
    os << "minimum: " << tissues.minimum << std::endl;
    os << "maximum: " << tissues.maximum << std::endl;
    os << "first_peak: " << tissues.first_peak << std::endl;
    os << "second_peak: " << tissues.second_peak << std::endl;
    os << "third_peak: " << tissues.third_peak << std::endl;
    os << "first_valley: " << tissues.first_valley << std::endl;
    os << "second_valley: " << tissues.second_valley << std::endl;
    return( os );
  }

}

#include "BrainIntensities.cpp"

#endif
