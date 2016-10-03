/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/27 
 * @brief Functions to sample uniformly from feature vector.
 */

#include "Common.hpp"
#include "Vector.hpp"

#ifndef BIALSAMPLEUNIFORM_H
#define BIALSAMPLEUNIFORM_H

namespace Bial {

  template< class D >
  class Feature;
  class Sample;

  namespace SampleType {

    /**
     * @date 2013/Jul/07 
     * @param feats: Feature vector. 
     * @param step: Number of elements between samples. 
     * @return A Vector with ids. 
     * @brief Generates a uniformly sampled Vector with ids. 
     * @warning none. 
     */
    template< class D >
    Sample Uniform( const Feature< D > &feats, size_t step = 1 );

  }

}

#include "SampleUniform.cpp"

#endif
