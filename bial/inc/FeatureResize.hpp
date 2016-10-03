/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Jun/26 
 * @brief Functions to merge feature vectors.
 */

#include "Common.hpp"

#ifndef BIALFEATURERESIZE_H
#define BIALFEATURERESIZE_H

#include "Feature.hpp"

namespace Bial {

  namespace FeatureResize {

    /**
     * @date 2014/May/05 
     * @param feat1: A feature vector. 
     * @param feat2: A feature vector. 
     * @return Merged feature vector. 
     * @brief Merges two feature vectors. Joins the input vectors and keeping the number of features. 
     * @warning none. 
     */
    template< class D >
    Feature< D > MergeElements( const Feature< D > &feat1, const Feature< D > &feat2 );

    /**
     * @date 2014/May/05 
     * @param feat1: A feature vector. 
     * @param feat2: A feature vector. 
     * @return Merged feature vector. 
     * @brief Merges two feature vectors. Joins the features and keeping the number of vectors.
     * @warning none. 
     */
    template< class D >
    Feature< D > MergeFeatures( const Feature< D > &feat1, const Feature< D > &feat2 );

  }

}

#include "FeatureResize.cpp"

#endif
