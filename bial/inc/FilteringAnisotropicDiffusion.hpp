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
  class AdjacencyIterator;
  class DiffusionFunction;

  namespace Filtering {
    
    /**
     * @date 2018/Oct/22 
     * @param adj: Adjacency relation. 
     * @param adj_size: Size of adjacency relation. Just required for faster computation.
     * @param img_dims: Number of dimensions of the image.
     * @return Returns weight vector as a structure to compute the filter. 
     * @brief Computes the weight vector which is a multiplier applied to 
     each adjacent pixel according to its relative position to the source pixel being filtered.
     * @warning none. 
     */
    Vector< double > WeightVector( const Adjacency &adj, size_t adj_size, size_t img_dims );

    /**
     * @date 2018/Oct/22 
     * @param adj: Adjacency relation. 
     * @param adj_size: Size of adjacency relation. Just required for faster computation.
     * @param img_dims: Number of dimensions of the image.
     * @return Returns integration constant depending ont he adjacency size and filtering function. 
     * @brief Computes integration constant used during flow computation.
     * @warning none. 
     */
    double IntegrationConstant( const Adjacency &adj, size_t adj_size, size_t img_dims );

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
     * @date 2018/Oct/22 
     * @param img: Input image.
     * @param mask: Vector with index of mask pixels, a subset of the input image pixels to be filtered.
     * @param diff_function: diffusion function. 
     * @param kappa: initial kappa constant to control the gradient range to be filtered. 
     * @param adj: Adjacency relation. 
     * @param adj_itr: Adjacency iterator. Passed here for faster computation.
     * @param weight: Weight vector. Use WeightVector function to compute it.
     * @param integraction_constant: Use IntegrationConstant function to compute it.
     * @return Returns filtered image by anisotropic diffusion. 
     * @brief Computes and returns a filtered image by anisotropic diffusion. 
     This is a faster implementation to run only one iteration on a subset of pixels given by the mask.
     It is used for the optimum anisotropic filter for best edge and flat region kappa computation.
     * @warning none.
     */
    template< class D >
    Image< D > QuickAnisotropicDiffusion( const Image< D > &img, const Vector< size_t > &mask,
					  const DiffusionFunction *diff_func, float kappa,
					  const Adjacency &adj, const AdjacencyIterator &adj_itr,
					  const Vector< double > & weight, double integration_constant );
    
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
