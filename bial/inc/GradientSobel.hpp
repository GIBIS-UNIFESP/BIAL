/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/21 
 * @brief Computes sobel gradient. 
 */

#ifndef BIALGRADIENTSOBEL_H
#define BIALGRADIENTSOBEL_H
#include "Common.hpp"

namespace Bial {

  template< class D >
  class Image;
  /**
   * @brief Computes image gradients.
   */
  namespace Gradient {

    /**
     * @date 2013/Nov/22 
     * @param img: Input image. 
     * @param direction: One of the dimensions of the input image. 
     * @return Directional Sobel filter over image. 
     * @brief Returns the directional Sobel filter over image. 
     * @warning none. 
     */
    template< class D >
    Image< D > DirectionalSobel( const Image< D > &img, size_t direction );

    /**
     * @date 2013/Nov/22 
     * @param img: Input image. 
     * @param magnitude: Sobel magnitude image. 
     * @param direction: Output Sobel direction image. 
     * @return Directional Sobel filter over image. 
     * @brief Returns the directional Sobel filter over image. 
     * @brief The directions are coded: 
     * @brief 2D and 3D: No-dir:0 E:1 W:2 S:4 SE:5 SW:6 N:8 NE:9 NW:10
     * @brief 3D: U:16 UE:17 UW:18 US:20 NSE:21 USW:22 UN:24 UNE:25 UNW:26
     * @brief 3D: D:32 DE:33 DW:34 DS:36 DSE:37 DSW:38 DN:40 DNE:41 DNW:42
     * @warning none. 
     */
    template< class D >
    void Sobel( const Image< D > &img, Image< D > *magnitude = nullptr, Image< int > *direction = nullptr );

  };

}

#include "GradientSobel.cpp"

#endif
