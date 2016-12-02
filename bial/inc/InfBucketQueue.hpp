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

#ifndef BIALBUCKETQUEUE_H
#define BIALBUCKETQUEUE_H

#include "BucketQueueElements.hpp"
#include "Common.hpp"
#include "Vector.hpp"

namespace Bial {

  /* Same behavior as BucketQueue class, but it supports infinite value elements. */
  class InfBucketQueue {

  protected:

    Vector< IdentityNode > identity; /** @brief vector containing information about each element ( state, previous, next ). */
    Vector< WeightNode > weight; /** @brief vector containing pointers to elements that fall into each bucket weight. */
    llint minimum; /** @brief maximum element currently inserted in the queue. */
    llint maximum; /** @brief minimum element currently inserted in the queue. */
    llint delta; /** @brief step between two consecutive buckets. */
    size_t elements; /** @brief Number of elements currently in the queue. */
    bool increasing; /** @brief Whether this queue removes elements increasingly or decreasingly. */
    bool fifo; /** @brief Whether this queue implements FIFO or LIFO tie-break policy. */
    size_t inf_elements; /* Number of elements equal to infinite */
    llint infinite_val; /* Infinite. The value used for infinite weight. */

  public:

    /**
     * @date 2015/Jun/24 
     * @param size: Total number of elements to be inserted in the queue. 
     * @param inf_val: Value used for infinite.
     * @param bucket_size: The size of the bucket. Elements whose weight difference is lower than the bucket_size
     *             may fall into the same bucket. 
     * @param increasing_order: increasing or decreasing order of bucket queue output. 
     * @param fifo_tie: fifo or lifo tiebreak. 
     * @return none. 
     * @brief Basic Constructor. 
     * @warning none. 
     */
    InfBucketQueue( size_t size, llint inf_val, llint bucket_size = 1.0, bool increasing_order = true, 
                    bool fifo_tie = true );

    /**
     * @date 2015/Jun/24 
     * @param value: bucket weight. 
     * @return The index where the input weight falls. 
     * @brief Computes and returns the index where the input
     * weight falls. 
     * @warning none. 
     */
    //using BucketQueue::Index;
    size_t Index( llint value );

  protected:

    /**
     * @date 2015/Jun/24 
     * @param new_size: new weight bucket queue size. 
     * @return none. 
     * @brief Grows bucket queue size to new_size. 
     * @warning none. 
     */
    void Grow( size_t new_size );

  public:

    /**
     * @date 2015/Jun/24 
     * @param idt: identity of the element. 
     * @param wgt: weight of the element. 
     * @return none. 
     * @brief Inserts element of identity idt and weight wgt
     * into the bucket queue. 
     * @warning none. 
     */
    //using BucketQueue::Insert;
    void Insert( size_t idt, llint wgt );

    /**
     * @date 2015/Jun/24 
     * @param none. 
     * @return The next element in the bucket queue. 
     * @brief Removes the next element in the bucket queue. 
     * @warning none. 
     */
    int Remove( );

  protected:

    /**
     * @date 2015/Jun/24 
     * @param idt: identity of the element. 
     * @param wgt: weight of the element. 
     * @return none. 
     * @brief Removes element of identity idt and weight wgt of the bucket queue. 
     * @warning none. 
     */
    //using BucketQueue::Remove;
    void Remove( size_t idt, llint wgt );

  public:

    /**
     * @date 2015/Jun/24 
     * @param idt: identity of the element. 
     * @param cur_wgt: current weight of the element. 
     * @param new_wgt: the new weight assigned to the element. 
     * @return none. 
     * @brief Updates current weight of element of identity
     * idt. 
     * @warning none. 
     */
    //using BucketQueue::Update;
    void Update( size_t idt, llint cur_wgt, llint new_wgt );

    /**
     * @date 2015/Jun/24 
     * @param none. 
     * @return Whether bucket queue is empty or not. 
     * @brief Returns true if bucket queue is empty. 
     * @warning none. 
     */
    bool Empty( );

    /**
     * @date 2015/Jun/24 
     * @param none. 
     * @return Number of elements in queue. 
     * @brief Returns the number of elements in queue. 
     * @warning none. 
     */
    size_t Elements( );

    /**
     * @date 2015/Jun/24 
     * @param none. 
     * @return Number of inifinite elements in queue. 
     * @brief Returns the number of inifinite elements in queue. 
     * @warning none. 
     */
    size_t InfiniteElements( );

    /**
     * @date 2015/Jun/24 
     * @param none. 
     * @return Increasing attribute value. 
     * @brief Returns increasing attribute value. 
     * @warning none. 
     */
    bool Increasing( ) const;

    /**
     * @date 2015/Jun/24 
     * @param idt: identity of the element. 
     * @return Element state in queue. 
     * @brief Returns element state in queue. 
     * @warning none. 
     */
    BucketState State( size_t idt ) const;

    /**
     * @date 2015/Jun/24 
     * @param idt: Identity of the element. 
     * @param new_state: New state. 
     * @return none. 
     * @brief Sets new state to element of index idt. 
     * @warning none. 
     */
    void State( size_t idt, BucketState new_state );

    /**
     * @date 2015/Jun/24 
     * @param idt: Identity of the element. 
     * @return none. 
     * @brief Sets finished state to element of index idt. 
     * @warning none. 
     */
    void Finished( size_t idt );

    /**
     * @date 2015/Jun/24 
     * @param none. 
     * @return The current number of buckets. 
     * @brief Returns the current number of buckets. 
     * @warning none. 
     */
    size_t Buckets( ) const;

    /**
     * @date 2016/Jan/27 
     * @param none.
     * @return none. 
     * @brief Sets all element states to NOT_VISITED. 
     * @warning none. 
     */
    void ResetState( );

  };

}

#include "InfBucketQueue.cpp"

#endif
