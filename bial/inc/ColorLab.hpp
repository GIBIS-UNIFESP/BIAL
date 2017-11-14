/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date: 2013/Oct/29
 * @brief CIELab color space convertion methods.
 */

#include "Common.hpp"

#ifndef BIALCOLORLAB_H
#define BIALCOLORLAB_H

#include "Color.hpp"

namespace Bial {

  template< class D >
  class Image;

  namespace ColorSpace {

    /**
     * @brief CIE X White Reference ( Obs.= 2°, Illuminant= D65 )
     */
    constexpr double XwRef = 95.047;
    /**
     * @brief CIE Y White Reference ( Obs.= 2°, Illuminant= D65 )
     */
    constexpr double YwRef = 100.000;
    /**
     * @brief CIE Z White Reference ( Obs.= 2°, Illuminant= D65 )
     */
    constexpr double ZwRef = 108.883;
    /**
     * @brief Const. used in conversions between CIELAB and XYZ
     */
    constexpr double Epsilon = 0.008856; 

    /**
     * @date 2013/Oct/29
     * @param img: RGB color Image
     * @return Image
     * @brief Converts a RGB image to XYZ colorspace.
     * @warning  Images must have 3 dimensions, and the "color" ChannelType in 3rd dimension.  <br>
     *           XYZ channel intensity range is approximately between 0 and 100. <br>
     *           Color conversion will be better in floating point images.
     */
    Image< RealColor > ARGBtoXYZ( const Image< Color > &img );
    
    /**
     * @date 2013/Oct/30
     * @param img: XYZ color Image
     * @return Image
     * @brief Converts a XYZ image to RGB colorspace.
     * @warning  Images must have 3 dimensions, and the "color" ChannelType in 3rd dimension. <br>
     *           Resultant RGB image intensity range is between 0 and 25. <br>
     *           Color conversion will be better in floating point images.
     */
    Image< Color > XYZtoARGB( const Image< RealColor > &img );

    /**
     * @date 2016/Jan/13
     * @param img: RGB color Image
     * @return Image
     * @brief Converts a RGB image to CIELab colorspace.
     * @warning  Images must have 3 dimensions, and the "color" ChannelType in 3rd dimension.  <br>
     *           XYZ channel intensity range is approximately between 0 and 100. <br>
     *           Color conversion will be better in floating point images.
     */
    Image< RealColor > ARGBtoCIELab( const Image< Color > &img );

    /**
     * @date 2016/Jan/13
     * @param img: XYZ color Image
     * @return Image
     * @brief Converts a CIELab image to RGB colorspace.
     * @warning  Images must have 3 dimensions, and the "color" ChannelType in 3rd dimension. <br>
     *           Resultant RGB image intensity range is between 0 and 25. <br>
     *           Color conversion will be better in floating point images.
     */
    Image< Color > CIELabtoARGB( const Image< RealColor > &img );
    
    /**
     * @date 2013/Oct/30
     * @param img: XYZ color Image
     * @return Image
     * @brief Converts a XYZ image to CIE-Lab colorspace.
     * @warning  Images must have 3 dimensions, and the "color" ChannelType in 3rd dimension. <br>
     *           Color conversion will be better in floating point images.
     */
    Image< RealColor > XYZtoCIELab( const Image< RealColor > &img );
    
    /**
     * @date 2013/Oct/30
     * @param img: Lab color Image
     * @return Image
     * @brief Converts a CIE-Lab image to XYZ colorspace.
     * @warning  Images must have 3 dimensions, and the "color" ChannelType in 3rd dimension. <br>
     *           Color conversion will be better in floating point images.
     */
    Image< RealColor > CIELabtoXYZ( const Image< RealColor > &img );

    /**
     * @date 2014/Apr/22
     * @param color: A color value.
     * @return Converted color value
     * @brief Auxiliar function to convert between RGB and XYZ color representations.
     * @warning none.
     */
    double ARGB_XYZ( double color );

    /**
     * @date 2014/Apr/22
     * @param color: A color value.
     * @return Converted color value
     * @brief Auxiliar function to convert between XYZ and RGB color representations.
     * @warning none.
     */
    double XYZ_ARGB( double color );

    /**
     * @date 2014/Apr/22
     * @param color: A color value.
     * @return Converted color value
     * @brief Auxiliar function to convert between XYZ and CIELab color representations.
     * @warning none.
     */
    double XYZ_CIELab( double color );

    /**
     * @date 2014/Apr/22
     * @param color: A color value.
     * @return Converted color value
     * @brief Auxiliar function to convert between CIELab and XYZ color representations.
     * @warning none.
     */
    double CIELab_XYZ( double color );

  }

}

#include "ColorLab.cpp"

#endif
