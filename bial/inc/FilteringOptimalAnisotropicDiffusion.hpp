/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Jul/12 
 * @brief Image filtering methods.
 */

#include "Common.hpp"
#include "Matrix.hpp"
#include "Vector.hpp"

#ifndef BIALFILTERINGOPTIMALANISOTROPICDIFFUSION_H
#define BIALFILTERINGOPTIMALANISOTROPICDIFFUSION_H

namespace Bial {

  template< class D >
  class Image;
  class Kernel;
  class Adjacency;
  class DiffusionFunction;

  namespace Filtering {

    /**
     * @date 2013/Nov/29 
     * @param source: Input image. 
     * @param mask: Input mask with edge region. 
     * @param diff_function: diffusion function. 
     * @param radius: Adjacency radius for diff_function. 
     * @return Best edge kappa to filter source image. 
     * @brief Finds the best edge kappa to filter source image. 
     * @warning none. 
     */
    template< class D >
    float EdgeRegionKappa( const Image< D > &source, const Image< D > &mask, const DiffusionFunction *diff_func,
                                  float radius );

    /**
     * @date 2013/Nov/29 
     * @param source: Input image. 
     * @param mask: Input mask with edge region. 
     * @param diff_function: diffusion function. 
     * @param radius: Adjacency radius for diff_function. 
     * @param kappa: Best edge kappa used for initialization. 
     * @return Best flat kappa to filter source image. 
     * @brief Finds the best flat kappa to filter source image. 
     * @warning none. 
     */
    template< class D >
    float FlatRegionKappa( const Image< D > &source, const Image< D > &mask, const DiffusionFunction *diff_func,
                                  float radius, float kappa );

    /**
     * @date 2013/Dec/06 
     * @param img: Input image. 
     * @param diff_function: diffusion function. 
     * @param radius: radius of the adjacency relation. 
     * @param conservativeness: Conservativeness function. Expected values: 0.0 to 1.0. Lower values keep weak
     *             edges and higher values remove stronger noise incidence. 
     * @return none. 
     * @brief Threads implementation for anisotropic diffusion filter. 
     * @warning none. 
     */
    template< class D >
    Image< D > OptimalAnisotropicDiffusion( Image< D > img, const DiffusionFunction *diff_func, float radius,
                                                   float conservativeness );

    /**
     * @date 2013/Dec/03 
     * @param img: Input image. 
     * @param diff_function: diffusion function. 
     * @param radius: radius of the adjacency relation. 
     * @param conservativeness: Conservativeness function. Expected values: 0.0 to 1.0. Lower values keep weak
     *             edges and higher values remove stronger noise incidence. 
     * @param canny: Canny edges segmentation. 
     * @param backg: Image background segmentation. 
     * @return none. 
     * @brief Threads implementation for anisotropic diffusion filter. 
     * @warning none. 
     */
    template< class D >
    Image< D > OptimalAnisotropicDiffusion( Image< D > img, const DiffusionFunction *diff_func, float radius,
                                            float conservativeness, const Image< D > &canny, 
                                            const Image< D > &backg );

  }

}

#include "FilteringOptimalAnisotropicDiffusion.cpp"

#endif
