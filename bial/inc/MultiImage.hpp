/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2016/Sep/19
 * @brief MultiImage class declaration. Handler for images which data type is not known until run time.
 */

#include "Color.hpp"
#include "Common.hpp"
#include "Image.hpp"
#include "RealColor.hpp"

#ifndef BIALMULTIIMAGE_H
#define BIALMULTIIMAGE_H

namespace Bial {

  /**
   * @brief The MultiImageType class is used to identify which image type is open in MultiImage.
   */
  enum class MultiImageType : char {
    none,
    int_img,
    flt_img,
    clr_img,
    rcl_img
  };

  /**
   * @brief The MultiImage class may have only one of the following image types: int, float, Color, or RealColor.
   */
  class MultiImage {

  private:
    
    MultiImageType type;
    Image< int > *int_img;
    Image< float > *flt_img;
    Image< Color > *clr_img;
    Image< RealColor > *rcl_img;

  public:
    
    /**
     * @date 2016/Sep/29
     * @param none.
     * @return none.
     * @brief Basic Constructor.
     * @warning none.
     */
    MultiImage( );

    /**
     * @date 2016/Sep/29
     * @param img: Input image.
     * @return none.
     * @brief Basic Constructor. Makes a copy of the given image.
     * @warning none.
     */
    MultiImage( const Image< int > &img );
    MultiImage( const Image< float > &img );
    MultiImage( const Image< Color > &img );
    MultiImage( const Image< RealColor > &img );

    /**
     * @date 2016/Sep/29
     * @param mimg: Input multi image.
     * @return none.
     * @brief Copy Constructor.
     * @warning none.
     */
    MultiImage( const MultiImage &mimg );

    /**
     * @date 2016/Sep/29
     * @param mimg: Input multi image.
     * @return none.
     * @brief Move Constructor.
     * @warning none.
     */
    MultiImage( MultiImage &&mimg );

    /**
     * @date 2016/Sep/29
     * @param none.
     * @return none.
     * @brief Destructor.
     * @warning none.
     */
    ~MultiImage( );

    /**
     * @date 2016/Sep/29
     * @param mimg: Input multi image.
     * @return A copy of the input multi image.
     * @brief Copy assign operator.
     * @warning none.
     */
    MultiImage &operator=( const MultiImage &mimg );

    /**
     * @date 2016/Sep/29
     * @param mimg: Input multi image.
     * @return A new multi image based on input multi image elements.
     * @brief Move assign operator.
     * @warning none.
     */
    MultiImage &operator=( MultiImage &&mimg );

    /**
     * @date 2016/Sep/29
     * @param none.
     * @return A reference the current image type.
     * @brief Returns a reference the current image type.
     * @warning none.
     */
    const MultiImageType &Type( ) const;

    /**
     * @date 2016/Sep/29
     * @param none.
     * @return A reference to the integer image.
     * @brief Returns a reference to the integer image.
     * @warning Does not verify if integer image exists.
     */
    Image< int > &IntImage( ) const;

    /**
     * @date 2016/Sep/29
     * @param none.
     * @return A reference to the float image.
     * @brief Returns a reference to the float image.
     * @warning Does not verify if float image exists.
     */
    Image< float > &FltImage( ) const;

    /**
     * @date 2016/Sep/29
     * @param none.
     * @return A reference to the Color image.
     * @brief Returns a reference to the Color image.
     * @warning Does not verify if Color image exists.
     */
    Image< Color > &ClrImage( ) const;

    /**
     * @date 2016/Sep/29
     * @param none.
     * @return A reference to the RealColor image.
     * @brief Returns a reference to the RealColor image.
     * @warning Does not verify if RealColor image exists.
     */
    Image< RealColor > &RclImage( ) const;

  };

}

#include "MultiImage.cpp"

#endif
