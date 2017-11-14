/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Nov/19 
 * @brief Sobel kernel generator.
 */

#ifndef BIALKERNELSOBEL_H
#define BIALKERNELSOBEL_H

#include "Common.hpp"
#include "Kernel.hpp"

namespace Bial {

  namespace KernelType {

    /**
     * @date 2013/Nov/22 
     * @param dimensions: Number of kernel dimensions. 
     * @param direction: Gradient direction to be estimated. 
     * @return Sobel kernel. 
     * @brief Creates and returns a Sobel kernel. direction should be lower than dimensions value.  Each of
     * Sobel's kernel sides is 3 pixels long. It is computed from a triangle filter with smoothing (h(−1) =
     * 1, h(0) = 2, h(1) = 1) and central differences (h'(−1) =−1, h'(0) =0, h'(1) = 1) for derivative
     * reconstruction. E.g. h'x(x,y,z) =h'(x)h(y)h(z). 
     * @warning none. 
     */
    Kernel Sobel( size_t dimensions, size_t direction );

    /**
     * @date 2013/Nov/22 
     * @param dimensions: Number of kernel dimensions. 
     * @param direction: Gradient direction to be estimated. 
     * @return Normalized Sobel kernel. 
     * @brief Creates and returns a normalized Sobel kernel. direction should be lower than dimensions value.
     * Each of Sobel's kernel sides is 3 pixels long. It is computed from a triangle filter with smoothing
     * (h(−1) = 1, h(0) = 2, h(1) = 1) and central differences (h'(−1) =−1, h'(0) =0, h'(1) = 1) for
     * derivative reconstruction. E.g. h'x(x,y,z) =h'(x)h(y)h(z). Sum of elements is 1. 
     * @warning none. 
     */
    Kernel NormalizedSobel( size_t dimensions, size_t direction );

  }

}

#include "KernelSobel.cpp"

#endif
