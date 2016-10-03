/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Nov/19 
 * @brief Box kernel generator.
 */

#ifndef BIALKERNELBOX_H
#define BIALKERNELBOX_H

#include "Common.hpp"
#include "Kernel.hpp"

namespace Bial {

  namespace KernelType {

    /**
     * @date 2013/Nov/19 
     * @param dimensions: Number of kernel dimensions. 
     * @param side_lenght: Number of pixel in each side. 
     * @return A N-dimensional box kernel with 1 coefficients. 
     * @brief Creates and returns a N-dimensional box kernel with 1 coefficients. 
     * @warning none. 
     */
     Kernel Box( size_t dimensions, size_t side_lenght );

  }

}

#include "KernelBox.cpp"

#endif
