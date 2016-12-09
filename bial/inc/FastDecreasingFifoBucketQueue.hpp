/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2016/Dec/05
 * @brief
 * Content: Fast implementation of Bucket-sort queue class 
 * <br> Description: A bucket-sort queue. It is useful to sort a large set of elements whose weight domain is not very 
 * large, * e.g. IFT and OPF algorithms over MR images. 
 * Warning: In Boost library, there is a class named circular_buffer
 * that may do exactly the same as Vector< WeightNode >. 
 */

#ifndef BIALFASTDECREASINGFIFOBUCKETQUEUE_H
#define BIALFASTDECREASINGFIFOBUCKETQUEUE_H

#include "BucketQueue.hpp"
#include "Common.hpp"
#include "Vector.hpp"

namespace Bial {

  /** @brief Standard bucket queue class, that may work with both integer or real values. */
  class FastDecreasingFifoBucketQueue : public BucketQueue {

  protected:

    ldbl minimum; /** @brief maximum element currently inserted in the queue. */
    ldbl maximum; /** @brief minimum element currently inserted in the queue. */
    ldbl current; /** @brief last element weight accesssed from this queue. */

  public:

    /**
     * @date 2016/Dec/05
     * @param size: Total number of elements to be inserted in the queue. 
     * @param minimum_value: Smaller value among all data.
     * @param n_buckets: Number of buckets. Must be greater or equal the number of distinct values.
     * @param increasing_order: increasing or decreasing order of bucket queue output. 
     * @param fifo_tie: fifo or lifo tiebreak. 
     * @return none. 
     * @brief Basic Constructor. 
     * @warning none. 
     */
    FastDecreasingFifoBucketQueue( size_t size, ldbl minimum_value, size_t n_buckets );

  protected:

    /**
     * @date 2016/Dec/05
     * @param value: bucket weight. 
     * @return The index where the input weight falls. 
     * @brief Computes and returns the index where the input
     * weight falls. 
     * @warning none. 
     */
    size_t Index( ldbl value );

  public:

    /**
     * @date 2016/Dec/05
     * @param idt: identity of the element. 
     * @param wgt: weight of the element. 
     * @return none. 
     * @brief Inserts element of identity idt and weight wgt
     * into the bucket queue. 
     * @warning none. 
     */
    void Insert( size_t idt, ldbl wgt );

    /**
     * @date 2016/Dec/05
     * @param none. 
     * @return The next element in the bucket queue. 
     * @brief Removes the next element in the bucket queue. 
     * @warning none. 
     */
    int Remove( );

  protected:

    /**
     * @date 2016/Dec/05
     * @param idt: identity of the element. 
     * @param wgt: weight of the element. 
     * @return none. 
     * @brief Removes element of identity idt and weight wgt of the bucket queue. 
     * @warning none. 
     */
    void Remove( size_t idt, ldbl wgt );

  public:

    /**
     * @date 2016/Dec/05
     * @param idt: identity of the element. 
     * @param cur_wgt: current weight of the element. 
     * @param new_wgt: the new weight assigned to the element. 
     * @return none. 
     * @brief Updates current weight of element of identity
     * idt. 
     * @warning none. 
     */
    void Update( size_t idt, ldbl cur_wgt, ldbl new_wgt );

  };

}

#include "FastDecreasingFifoBucketQueue.cpp"

#endif
