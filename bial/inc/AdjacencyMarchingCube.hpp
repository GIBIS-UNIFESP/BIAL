/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Jul/05
 * @brief Adjacency relation for marching cubes algorithm.
 * <br> Future add-on's: none.
 */

#include "Adjacency.hpp"
#include "Common.hpp"

#ifndef BIALADJACENCYMARCHINGCUBES_H
#define BIALADJACENCYMARCHINGCUBES_H

/* Declarations -------------------------------------------------------------------------------------------------------- */

namespace Bial {

  namespace AdjacencyType {

    /**
     * @date 2015/Aug/03
     * @param img: Image of the marching cube
     * @return A marching cube Adjacency
     * @brief Create and return a 3D box adjacency known as Marching Cube.
     * @warning none.
     */
    Adjacency MarchingCube( );

  }

}

#include "AdjacencyMarchingCube.cpp"

#endif
