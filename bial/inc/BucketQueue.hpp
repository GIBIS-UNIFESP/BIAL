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

  /** @brief Standard bucket queue class, that may work with both integer or real values. */
  class BucketQueue {

  protected:

    Vector< IdentityNode > identity; /** @brief vector containing information about each element ( state, previous, next ). */
    Vector< WeightNode > weight; /** @brief vector containing pointers to elements that fall into each bucket weight. */
    ldbl minimum; /** @brief maximum element currently inserted in the queue. */
    ldbl maximum; /** @brief minimum element currently inserted in the queue. */
    ldbl delta; /** @brief step between two consecutive buckets. */
    size_t elements; /** @brief Number of elements currently in the queue. */
    bool increasing; /** @brief Whether this queue removes elements increasingly or decreasingly. */
    bool fifo; /** @brief Whether this queue implements FIFO or LIFO tie-break policy. */

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
    BucketQueue( size_t size, ldbl bucket_size = 1.0, bool increasing_order = true, bool fifo_tie = true );

    /**
     * @date 2015/Jun/24 
     * @param none. 
     * @return none. 
     * @brief Virutal Destructor. 
     * @warning none. 
     */
    virtual ~BucketQueue( ) {
    }

  protected:

    /**
     * @date 2013/Oct/11 
     * @param value: bucket weight. 
     * @return The index where the input weight falls. 
     * @brief Computes and returns the index where the input
     * weight falls. 
     * @warning none. 
     */
    virtual size_t Index( ldbl value );

    /**
     * @date 2012/Sep/06 
     * @param new_size: new weight bucket queue size. 
     * @return none. 
     * @brief Grows bucket queue size to new_size. 
     * @warning none. 
     */
    virtual void Grow( size_t new_size );

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
    virtual void Insert( size_t idt, ldbl wgt );

    /**
     * @date 2012/Sep/06 
     * @param none. 
     * @return The next element in the bucket queue. 
     * @brief Removes the next element in the bucket queue. 
     * @warning none. 
     */
    virtual int Remove( );

  protected:

    /**
     * @date 2012/Sep/06 
     * @param idt: identity of the element. 
     * @param wgt: weight of the element. 
     * @return none. 
     * @brief Removes element of identity idt and weight wgt of the bucket queue. 
     * @warning none. 
     */
    virtual void Remove( size_t idt, ldbl wgt );

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
    virtual void Update( size_t idt, ldbl cur_wgt, ldbl new_wgt );

    /**
     * @date 2012/Sep/06 
     * @param none. 
     * @return Whether bucket queue is empty or not. 
     * @brief Returns true if bucket queue is empty. 
     * @warning none. 
     */
    virtual bool Empty( );

    /**
     * @date 2013/Oct/14 
     * @param none. 
     * @return Number of elements in queue. 
     * @brief Returns the number of elements in queue. 
     * @warning none. 
     */
    virtual size_t Elements( );

    /**
     * @date 2012/Sep/06 
     * @param none. 
     * @return Increasing attribute value. 
     * @brief Returns increasing attribute value. 
     * @warning none. 
     */
    virtual bool Increasing( ) const;

    /**
     * @date 2012/Sep/19 
     * @param idt: identity of the element. 
     * @return Element state in queue. 
     * @brief Returns element state in queue. 
     * @warning none. 
     */
    virtual BucketState State( size_t idt ) const;

    /**
     * @date 2012/Sep/25 
     * @param idt: Identity of the element. 
     * @param new_state: New state. 
     * @return none. 
     * @brief Sets new state to element of index idt. 
     * @warning none. 
     */
    virtual void State( size_t idt, BucketState new_state );

    /**
     * @date 2012/Sep/25 
     * @param idt: Identity of the element. 
     * @return none. 
     * @brief Sets finished state to element of index idt. 
     * @warning none. 
     */
    virtual void Finished( size_t idt );

    /**
     * @date 2012/Sep/21 
     * @param none. 
     * @return The current number of buckets. 
     * @brief Returns the current number of buckets. 
     * @warning none. 
     */
    virtual size_t Buckets( ) const;

    /**
     * @date 2016/Jan/27 
     * @param none.
     * @return none. 
     * @brief Sets all element states to NOT_VISITED. 
     * @warning none. 
     */
    virtual void ResetState( );
  };

}

#include "BucketQueue.cpp"

#endif
