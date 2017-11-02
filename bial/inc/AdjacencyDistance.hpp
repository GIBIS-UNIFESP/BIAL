/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2017/Oct/27
 * @brief Adjacency distance. Precomputed distance from source to adjacent pixels.
 */

#include "Common.hpp"
#include "Adjacency.hpp"
#include "Vector.hpp"

#ifndef BIALADJACENCYDISTANCE_H
#define BIALADJACENCYDISTANCE_H

/* Declarations -------------------------------------------------------------------------------------------------------- */

namespace Bial {

  class AdjacencyDistance {

  protected:

    /**
     * @brief
     * Vector with pre-computed distances. It is the magnitude of the distances.
     */
    Vector< float > distance;
    
  public:

    /**
     * @date 2012/Jul/06
     * @param none
     * @return none.
     * @brief Basic Constructor.
     * @warning Uninitialized adjacency is created. 
     */
    AdjacencyDistance( const Adjacency &adj );

    /**
     * @date 2012/Jul/06
     * @param elem: Adjacency element index.
     * @param dim: Number of dimension of the element.
     * @return Data pointed by index ( elem, dim ).
     * @brief Returns data pointed by index ( elem, dim ). 
     * @warning Adjacency dimensions and bounds are not verified. 
     */
    const float &operator[]( size_t elem ) const;

  };

}

#include "AdjacencyDistance.cpp"

#endif
