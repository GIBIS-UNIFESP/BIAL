/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/27 
 * @brief Random sampling elements from feature vector. 
 */

#include "Common.hpp"
#include "Vector.hpp"

#ifndef BIALSAMPLERANDOM_H
#define BIALSAMPLERANDOM_H

namespace Bial {

  template< class D >
  class Feature;
  class Sample;

  namespace SampleType {

    /**
     * @date 2013/Jul/07 
     * @param subsamples: Number of subsamples. 
     * @param total_samples: Total number of samples. 
     * @return A Vector with ids of samples . 
     * @brief Generates a Vector with ids of random samples withing the giving range. 
     * @warning 'subsamples' must be smaller than or equal to 'total_samples'. 
     */
    Sample Random( size_t subsamples, size_t total_samples );

    /**
     * @date 2014/Nov/07 
     * @param feats: feature vector. 
     * @param subsamples: Number of subsamples. 
     * @return A sample with ids of samples. 
     * @brief Generates a vector with ids of random samples withing the giving range. <br> 
     * Repeated samples are inserted into the same position. 
     * @warning 'subsamples' must be smaller than or equal to 'total_samples'. 
     */
    template< class D >
    Sample RandomDistinct( const Feature< D > &feats, size_t subsamples );

  }

}

#include "SampleRandom.cpp"

#endif
