/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2015/Apr/07 
 * @brief Edge detection with Livewire algorithm. 
 */

#ifndef BIALEDGELIVEWIRE_H
#define BIALEDGELIVEWIRE_H

#include "Common.hpp"
#include "Vector.hpp"

namespace Bial {

  template< class D >
  class Image;

  namespace Edge {

    /**
     * @date 2015/Jun/23 
     * @param img: Input image. 
     * @param seed: Seeds for propagation start. 
     * @param weight: MaxSumPathfunction weight. Lower values for results more similar to Riverbed. 
     * @param         Higher values for results more similar to River Bed. 
     * @return The cost and predecessor map of live-wire. 
     * @brief Computes the live-wire algorithm. 
     * @warning none. 
     */
    template< class D >
    std::tuple< Image< D >, Image< int > > LiveWire( const Image< D > &img, const Vector< bool > &seed, float weight );

    /**
     * @date 2015/Jun/23 
     * @param img: Input image. 
     * @param msk: Mask that restricts the domain of the algorithm on pixels whose value are different of 0. 
     * @param seed: Seeds for propagation start. 
     * @param weight: MaxSumPathfunction weight. Lower values for results more similar to Riverbed. 
     * @param         Higher values for results more similar to River Bed. 
     * @return The cost and predecessor map of live-wire. 
     * @brief Computes the live-wire algorithm. 
     * @warning none. 
     */
    template< class D >
    std::tuple< Image< D >, Image< int > > LiveWire( const Image< D > &img, const Image< D > &msk, 
                                                     const Vector< bool > &seed, float weight );

  }

}

#include "EdgeLiveWire.cpp"

#endif /* BIALEDGELIVEWIRE_H */
