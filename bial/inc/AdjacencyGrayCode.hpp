/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Jul/05
 * @brief Adjacency relation with indexes following gray code sequency.
 * <br> Future add-on's: none.
 */

#include "Adjacency.hpp"
#include "Common.hpp"
#include "Adjacency.hpp"

#ifndef BIALADJACENCYGRAYCODE_H
#define BIALADJACENCYGRAYCODE_H

/* Declarations -------------------------------------------------------------------------------------------------------- */

namespace Bial {

  namespace AdjacencyType {

    /**
     * @date 2015/Apr/10
     * @param none.
     * @return adjacency 8 with indexes ordered in such way that adjacent's indexes differ in one bit.
     * @brief Returns adjacency 8 with indexes ordered in such way that adjacent's indexes differ in one bit. 
     * @warning none.
     */
    Adjacency Gray8( );

  }
  
}

#include "AdjacencyGrayCode.cpp"

#endif
