/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/27 
 * @brief Methods for sampling elements. 
 */

#include "Common.hpp"
#include "Vector.hpp"

#ifndef BIALSAMPLE_H
#define BIALSAMPLE_H

namespace Bial {

  template< class D >
  class Feature;
  /**
   * @brief  Methods for sampling elements. 
   */
  class Sample {

  private:

    Vector< Vector< size_t > > sample;

  public:

    /**
     * @date 2014/Nov/08 
     * @param none. 
     * @return none. 
     * @brief Basic Constructor. With empty sample vector. 
     * @warning none. 
     */
    Sample( ) {
    }

    /**
     * @date 2014/Nov/08 
     * @param none. 
     * @return none. 
     * @brief Basic Constructor. Initializes vector with a matrix of the number of elements per 1. 
     * @warning none. 
     */
    Sample( size_t elements );

    /**
     * @date 2014/Nov/08 
     * @param elm: The element to be accessed. 
     * @return Constant reference to the desired element. 
     * @brief Returns constant reference to the desired element. The main or first element in that position is
     * returned. 
     * @warning Image bounds are not verified. 
     */
    const size_t &operator[]( size_t elm ) const;

    /**
     * @date 2014/Nov/08 
     * @param elm: The element to be accessed. 
     * @return Reference to the desired element. 
     * @brief Returns reference to the desired element. The main or first element in that position is returned. 
     * @warning Image bounds are not verified. 
     */
    size_t &operator[]( size_t elm );

    /**
     * @date 2014/Nov/08 
     * @param elm: The element to be accessed. 
     * @return Constant reference to the desired element. 
     * @brief Returns constant reference to the desired element. The main or first element in that position is
     * returned. 
     * @warning Image bounds are not verified. 
     */
    const size_t &operator()( size_t elm ) const;

    /**
     * @date 2014/Nov/08 
     * @param elm: The element to be accessed. 
     * @return Reference to the desired element. 
     * @brief Returns reference to the desired element. The main or first element in that position is returned. 
     * @warning Image bounds are not verified. 
     */
    size_t &operator()( size_t elm );

    /**
     * @date 2014/Nov/08 
     * @param main_elm: The i-th non-repeated element. 
     * @param repeated_elm: The j-th repeated element of the i-th non-repeated element. 
     * @return Constant reference to the j-th repeated element of the i-th non-repead element. 
     * @brief Returns a constant reference to the j-th repeated element of the i-th non-repead element. 
     * @warning Image bounds are not verified. 
     */
    const size_t &operator()( size_t main_elm, size_t repeated_elm ) const;

    /**
     * @date 2014/Nov/08 
     * @param main_elm: The i-th non-repeated element. 
     * @param repeated_elm: The j-th repeated element of the i-th non-repeated element. 
     * @return The reference to the j-th repeated element of the i-th non-repead element. 
     * @brief Returns the reference to the j-th repeated element of the i-th non-repead element. 
     * @warning Image bounds are not verified. 
     */
    size_t &operator()( size_t main_elm, size_t repeated_elm );

    /**
     * @date 2014/Nov/09 
     * @param none. 
     * @return The number of non-repeated samples. 
     * @brief Returns the number of non-repeated samples. 
     * @warning none. 
     */
    size_t Size( ) const;

    /**
     * @date 2014/Nov/09 
     * @param none. 
     * @return The number of non-repeated samples. 
     * @brief Returns the number of non-repeated samples. 
     * @warning none. 
     */
    size_t size( ) const;

    /**
     * @date 2014/Nov/09 
     * @param elm: The i-th non-repeated element. 
     * @return The number of repeated elements to the i-th non-repeated samples. 
     * @brief Returns the number of repeated elements to the i-th non-repeated samples. 
     * @warning none. 
     */
    size_t Size( size_t elm ) const;

    /**
     * @date 2014/Nov/09 
     * @param elm: The i-th non-repeated element. 
     * @return The number of repeated elements to the i-th non-repeated samples. 
     * @brief Returns the number of repeated elements to the i-th non-repeated samples. 
     * @warning none. 
     */
    size_t size( size_t elm ) const;

    /**
     * @date 2014/Nov/09 
     * @param none. 
     * @return none. 
     * @brief Inserts a new entry with no elements. 
     * @warning none. 
     */
    void push_back( );

    /**
     * @date 2014/Nov/09 
     * @param pos: The i-th non-repeated element.
     * @param new_elm: The value of new element to be inserted in pos entry.
     * @return none. 
     * @brief Inserts a new element in a given position. 
     * @warning none. 
     */
    void push_back( size_t pos, size_t new_elm  );

  };

}

#include "Sample.cpp"

#endif
