/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2015/Apr/07 
 * @brief Edge detection with Riverbed algorithm. 
 */

#ifndef BIALEDGERIVERBED_H
#define BIALEDGERIVERBED_H

#include "Common.hpp"
#include "Vector.hpp"

namespace Bial {

  template< class D >
  class Image;

  namespace Edge {

    /**
     * @date 2015/Jun/09 
     * @param img: Input inverted gradient image. 
     * @param seed: Seeds for propagation start. 
     * @return The cost and predecessor map of river-bed. 
     * @brief Computes the river-bed algorithm. 
     * @warning none. 
     */
    template< class D >
    std::tuple< Image< D >, Image< int > > RiverBed( const Image< D > &img, const Vector< bool > &seed );

    /**
     * @date 2015/Jun/09 
     * @param img: Input image. 
     * @param msk: Mask that restricts the domain of the algorithm on pixels whose value are different of 0. 
     * @param seed: Seeds for propagation start. 
     * @return The cost and predecessor map of river-bed. 
     * @brief Computes the river-bed algorithm. 
     * @warning none. 
     */
    template< class D >
    std::tuple< Image< D >, Image< int > > RiverBed( const Image< D > &img, const Image< D > &msk,
                                                     const Vector< bool > &seed );

  }

}

#include "EdgeRiverBed.cpp"

#endif /* BIALEDGERIVERBED_H */
