/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2013/Sep/04
 * @brief binary search in sorted vector.
 */

#include "Common.hpp"
#include "Vector.hpp"

#ifndef BIALSORTINGBINARYSEARCH_H
#define BIALSORTINGBINARYSEARCH_H

namespace Bial {
  /**
   * @brief Extensions for sorting methods.
   */
  namespace Sorting {

    /**
     * @date 2013/Oct/04
     * @param data: A sorted random access data structure that implements size( ) and operator[]( size_t )
     * functions. E.G. Vector, std::deque, Matrix, Image.
     * @param val: Value to be found.
     * @param min: Minimum search index.
     * @return A vector index.
     * @brief First vector index found in a binary search for 'val' in 'data' vector between min and the last
     * indexes (extremities included). If the 'data' does not have any 'val' element, the function returns
     * the next index to the expected position, limited by the index range.
     * @warning none.
     */
    template< template< class D > class C, class D >
    size_t BinarySearch( const C< D > &data, D val, size_t min = 0 );

    /**
     * @date 2013/Oct/04
     * @param data: A sorted random access data structure that implements size( ) and operator[]( size_t )
     * functions. E.G. Vector, std::deque, Matrix, Image.
     * @param val: Value to be found.
     * @param min: Minimum search index.
     * @param max: Maximum search index.
     * @return A vector index.
     * @brief First vector index found in a binary search for 'val' in 'data' vector between min and max indexes
     * (extremities included). If the 'data' does not have any 'val' element, the function returns the next
     * index to the expected position, limited by the index range.
     * @warning none.
     */
    template< template< class D > class C, class D >
    size_t BinarySearch( const C< D > &data, D val, size_t min, size_t max );

  }

}

#include "SortingBinarySearch.cpp"

#endif
