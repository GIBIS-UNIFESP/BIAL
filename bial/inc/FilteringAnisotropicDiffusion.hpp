/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Jul/12 
 * @brief Anisotropic diffusion filtering to remove high-frequency noise.
 */

#include "Common.hpp"

#ifndef BIALFILTERINGANISOTROPICDIFFUSION_H
#define BIALFILTERINGANISOTROPICDIFFUSION_H

namespace Bial {

  template< class D >
  class Vector;
  template< class D >
  class Image;
  class Adjacency;
  class DiffusionFunction;

  namespace Filtering {

    /**
     * @date 2013/Nov/18 
     * @param img: Input image. 
     * @param diff_function: diffusion function. 
     * @param init_kappa: initial kappa constant to control the gradient range to be filtered. 
     * @param radius: radius of the adjacency relation. 
     * @return Returns filtered image by anisotropic diffusion. 
     * @brief Computes and returns a filtered image by anisotropic diffusion. 
     * @warning none. 
     */
    template< class D >
    Image< D > AdaptiveAnisotropicDiffusion( Image< D > img, const DiffusionFunction *diff_func, float init_kappa,
                                             float radius = 1.0 );

    /**
     * @date 2013/Nov/18 
     * @param img: Input image. 
     * @param diff_function: diffusion function. 
     * @param kappa: constant to control the gradient range to be filtered. 
     * @param iterations: the number of iterations. 
     * @param radius: radius of the adjacency relation. 
     * @return Returns filtered image by anisotropic diffusion. 
     * @brief Computes and returns a filtered image by anisotropic diffusion. 
     * @warning none. 
     */
    template< class D >
    Image< D > AnisotropicDiffusion( Image< D > img, const DiffusionFunction *diff_func, float kappa, size_t iterations,
                                     float radius = 1.0 );

    /**
     * @date 2013/Nov/27 
     * @param img: Input image. 
     * @param res: Output image.
     * @param weight: pre-computed weight vector based on relative distance between adjacents.
     * @param integration_constant: Constant of integration based on adjacency size. 
     * @param diff_function: diffusion function. 
     * @param kappa: constant to control the gradient range to be filtered. 
     * @param adj: An adjacency relation. 
     * @param thread: Thread number. 
     * @param total_threads: Number of threads. 
     * @return none. 
     * @brief Threads implementation for anisotropic diffusion filter. 
     * @warning none. 
     */
    template< class D >
    void AnisotropicDiffusionThread( Image< D > &img, Image< D > &res, const Vector< double > &weight,
				     double integration_constant, const DiffusionFunction *diff_func, float kappa,
				     Adjacency &adj, size_t thread, size_t total_threads );

  }

}

#include "FilteringAnisotropicDiffusion.cpp"

#endif
