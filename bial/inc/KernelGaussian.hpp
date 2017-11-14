/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Nov/19 
 * @brief Gaussian kernel generator.
 */

#ifndef BIALKERNELGAUSSIAN_H
#define BIALKERNELGAUSSIAN_H

#include "Common.hpp"
#include "Kernel.hpp"

namespace Bial {

  namespace KernelType {

    /**
     * @date 2013/Nov/19 
     * @param dimensions: Number of kernel dimensions. 
     * @param radius: Kernel adjacency radius. 
     * @param std_dev: standard deviation of the Gaussian. 
     * @param amplitude: Gaussian maximum amplitude. 
     * @return Gaussian kernel. 
     * @brief Creates and returns a Gaussian kernel. 
     * @warning none. 
     */
    Kernel Gaussian( size_t dimensions, float radius, float std_dev, float amplitude = -1.0f );
    
    /**
     * @date 2013/Nov/21 
     * @param dimensions: Number of kernel dimensions. 
     * @param radius: Kernel adjacency radius. 
     * @param std_dev: standard deviation of the Gaussian. 
     * @return Normalized Gaussian kernel. 
     * @brief Creates and returns a normalized Gaussian kernel. 
     * @warning none. 
     */
    Kernel NormalizedGaussian( size_t dimensions, float radius, float std_dev );
    
  }

}

#include "KernelGaussian.cpp"

#endif
