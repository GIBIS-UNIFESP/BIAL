/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/26 
 * @brief OPF cluster matching verification algorithms. 
 */

#include "Common.hpp"
#include "Vector.hpp"

#ifndef BIALOPFCLUSTERMATCHING_H
#define BIALOPFCLUSTERMATCHING_H

namespace Bial {

  namespace OPF {

    /**
     * @date 2014/Apr/30 
     * @param sim_label: Vector of labels. 
     * @return The minimum error of corresponding clusters between pairs of containers (i.e. images, vectors).
     * @brief Computes and returns the minimum error of corresponding clusters between pairs of containers.
     *        e.g. Given a ground truth labeled image and a test labeled image, this function tries to maximize the
     *        matching of cluster labels in order to achieve the minimum error. If both images have the same clusters,
     *        but with different labels assigned to them, the error will be zero. The error is the frequency of
     *        incorrect labeled pixels.
     * @warning none. 
     */
    template< template< class D > class C >
    float ClusterMatchingError( const Vector< C< int > > &sim_label );

  }

}

#include "OPFClusterMatching.cpp"

#endif
