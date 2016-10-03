/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Nov/19 
 * @brief Gabor kernel generator.
 */

#ifndef BIALKERNELGABOR_H
#define BIALKERNELGABOR_H

#include "Kernel.hpp"

namespace Bial {

  namespace KernelType {

    /**
     * @date 2015/Jun/03 
     * @param sigma: standard deviation of Gabor curve. 
     * @param dimensions: Number of kernel dimensions. 
     * @param direction: Gradient direction to be estimated. 
     * @return Gabor kernel. 
     * @brief Creates and returns a Gabor kernel. direction should be lower than dimensions value. 
     * @warning none. 
     */
    Kernel Gabor( float sigma, size_t dimensions, size_t direction );

    /**
     * @date 2015/Jun/03 
     * @param sigma: standard deviation of Gabor curve. 
     * @param dimensions: Number of kernel dimensions. 
     * @param direction: Gradient direction to be estimated. 
     * @return Normalized Gabor kernel. 
     * @brief Creates and returns a normalized Gabor kernel. Direction should be lower than dimensions value.
     * @warning none. 
     */
    Kernel NormalizedGabor( float sigma, size_t dimensions, size_t direction );

  }

}

#include "KernelGabor.cpp"

#endif
