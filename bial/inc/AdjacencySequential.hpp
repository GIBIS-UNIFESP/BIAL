/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2018/Dec/17
 * @brief Adjacency relation with sequential clock-wise indexes.
 * <br> Future add-on's: none.
 */

#include "Adjacency.hpp"
#include "Common.hpp"

#ifndef BIALADJACENCYSEQUENTIAL_H
#define BIALADJACENCYSEQUENTIAL_H

/* Declarations -------------------------------------------------------------------------------------------------------- */

namespace Bial {

  namespace AdjacencyType {

    /**
     * @date 2015/Apr/10
     * @param radius: Adjacency radius. Value: >= 1.0f, < 2.0f.
     * @return Adjacency with sequential (clock-wise) adjacents around the central pixel, not including the central pixel itself.
     * @brief Returns adjacency 8 with indexes ordered in such way that adjacent's indexes differ in one bit. 
     * @warning none.
     */
    Adjacency Sequential( float radius );

  }
  
}

#include "AdjacencySequential.cpp"

#endif
