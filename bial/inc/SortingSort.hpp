/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2013/Sep/04
 * @brief Sorting algorithm that outputs the order of the original indexes after sorting.
 */

#include "Common.hpp"
#include "Vector.hpp"

#ifndef BIALSORTINGSORT_H
#define BIALSORTINGSORT_H

namespace Bial {
  /**
   * @brief Extensions for sorting methods.
   */
  namespace Sorting {

    /**
     * @date 2013/Dec/11
     * @param data: A random access data structure that implements size( ) and operator[]( size_t )
     * functions. E.G. Vector, std::deque, Matrix, Image.
     * @param min_idx: Minimum index of the elements to be sorted.
     * @param max_idx: Maximum index of the elements to be sorted. 
     * @param increasing: If sorting in increasing or decreasing order.
     * @return Vector with the sorted indexes.
     * @brief Auxiliary recursive function for quick sort.
     * @warning Do not call this function. Use the other sort functions in this file.
     */
    template< template< class D > class C, class D >
    void Sort( C< D > &data, Vector< size_t > &order, size_t min_idx, size_t max_idx, bool increasing );

    /**
     * @date 2013/Sep/04
     * @param data: A random access data structure that implements size( ) and operator[]( size_t )
     * functions. E.G. Vector, std::deque, Matrix, Image.
     * @param increasing: If sorting in increasing or decreasing order.
     * @return Vector with the sorted indexes.
     * @brief Sorts data and returns a Vector with the sorted indexes.
     * @warning none.
     */
    template< template< class D > class C, class D >
    Vector< size_t > Sort( C< D > &data, bool increasing = true );

    /**
     * @date 2013/Sep/04
     * @param data: A random access data structure that implements size( ) and operator[]( size_t )
     * functions. E.G. Vector, std::deque, Matrix, Image.
     * @param order: vector with the sorting indexes.
     * @return none.
     * @brief Sorts data according to the sorting indexes in order vector. May be used with the return vector of
     *        Sort( C< D > &data, bool increasing ) function to sort other containers in the same order.
     * @warning none.
     */
    template< template< class D > class C, class D >
    void Sort( C< D > &data, const Vector< size_t > &order );

  }

}

#include "SortingSort.cpp"

#endif
