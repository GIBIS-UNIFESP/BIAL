/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/06 
 * @brief
 * Content: Bucket-sort queue class 
 * <br> Description: A bucket-sort queue. It is useful to sort a large set
 * of elements whose weight domain is not very large,
 * e.g. IFT and OPF algorithms over MR images. 
 * <br> Future add-on's: Deal with negative values. 
 *                  Add fast operation functions in which queue does not grow. 
 * Warning: In Boost library, there is a class named circular_buffer
 * that may do exactly the same as Vector< WeightNode >. 
 */

#ifndef BIALGROWINGBUCKETQUEUE_H
#define BIALGROWINGBUCKETQUEUE_H

#include "BucketQueue.hpp"
#include "Common.hpp"
#include "Vector.hpp"

namespace Bial {

  /** @brief Standard bucket queue class, that may work with both integer or real values. */
  class GrowingBucketQueue : public BucketQueue {

  protected:

    ldbl minimum; /** @brief maximum element currently inserted in the queue. */
    ldbl maximum; /** @brief minimum element currently inserted in the queue. */
    ldbl delta; /** @brief step between two consecutive buckets. */

  public:

    /**
     * @date 2012/Sep/06 
     * @param size: Total number of elements to be inserted in the queue. 
     * @param bucket_size: the size of the bucket. Elements whose weight difference is lower than the bucket_size
     *             may fall into the same bucket. 
     * @param increasing_order: increasing or decreasing order of bucket queue output. 
     * @param fifo_tie: fifo or lifo tiebreak. 
     * @return none. 
     * @brief Basic Constructor. 
     * @warning none. 
     */
    GrowingBucketQueue( size_t size, ldbl bucket_size = 1.0, bool increasing_order = true, bool fifo_tie = true );

  protected:

    /**
     * @date 2013/Oct/11 
     * @param value: bucket weight. 
     * @return The index where the input weight falls. 
     * @brief Computes and returns the index where the input
     * weight falls. 
     * @warning none. 
     */
    size_t Index( ldbl value );

    /**
     * @date 2012/Sep/06 
     * @param new_size: new weight bucket queue size. 
     * @return none. 
     * @brief Grows bucket queue size to new_size. 
     * @warning none. 
     */
    void Grow( size_t new_size );

  public:

    /**
     * @date 2012/Sep/06 
     * @param idt: identity of the element. 
     * @param wgt: weight of the element. 
     * @return none. 
     * @brief Inserts element of identity idt and weight wgt
     * into the bucket queue. 
     * @warning none. 
     */
    void Insert( size_t idt, ldbl wgt );

    /**
     * @date 2012/Sep/06 
     * @param none. 
     * @return The next element in the bucket queue. 
     * @brief Removes the next element in the bucket queue. 
     * @warning none. 
     */
    int Remove( );

  protected:

    /**
     * @date 2012/Sep/06 
     * @param idt: identity of the element. 
     * @param wgt: weight of the element. 
     * @return none. 
     * @brief Removes element of identity idt and weight wgt of the bucket queue. 
     * @warning none. 
     */
    void Remove( size_t idt, ldbl wgt );

  public:

    /**
     * @date 2012/Sep/06 
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

#include "GrowingBucketQueue.cpp"

#endif
