/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @file RandomQueue.hpp
 * @author Fábio Augusto Menocci Cappabianco 
 * @date 2012/Sep/12 
 * @version 1.0.00
 * @brief
 * Content: Random-access queue class declaration. 
 * <br> Description: A random-acess queue. Its size is fixed. As soon as it gets full, the first inserted element is
 * automatically replaced by the next inserted element.<br> 
 * It is useful to find the median element as walking through signal or image data, e.g. MedianFilter method of 
 * Signal class. 
 * Insertion and removal are made in limiar time with respect to the queue size. <br>
 *  Access to ordered keys by position is made in constant time. 
 * <br> Future add-on's: 
 * - Optimize Update method by usign a binary search for the key.
 * - A constructor with maximum size, sorting order, and an initial vector, sorted or not sorted. 
 */

#ifndef BIALRANDOMQUEUE_H
#define BIALRANDOMQUEUE_H

#include "Common.hpp"
#include "Vector.hpp"

namespace Bial {
  /**
   * @brief A random-acess queue. Its size is fixed. As soon as it gets full, the first inserted element is
   * automatically replaced by the next inserted element.<br> 
   * It is useful to find the median element as walking through signal or image data, e.g. MedianFilter method of 
   * Signal class. 
   * Insertion and removal are made in limiar time with respect to the queue size. <br>
   *  Access to ordered keys by position is made in constant time. 
   */
  class RandomQueue {

  protected:
    Vector< double > _data;
    Vector< size_t > order;
    double *qk_data;
    size_t occupancy;
    size_t last;
    bool increasing;

  public:

    /**
     * @date 2012/Sep/12 
     * @param size: maximum capacity of the queue. 
     * @param increasing_order: increasing or decreasing order of random queue elements sorting. 
     * @return none. 
     * @brief Basic Constructor. 
     * @warning none. 
     */
    RandomQueue( size_t size, bool increasing_order = true );

    /**
     * @date 2012/Sep/12 
     * @param elm: A sorted index. 
     * @return Key at the p-th position. 
     * @brief Returns key at the elm-th position in increasing or decreasing order, e.g., if elm = 0 and
     * increasing is true, then the smallest key is returned. 
     * @warning Queue bounds are not verified. 
     */
    double operator[]( size_t elm ) const;

    /**
     * @date 2012/Sep/12 
     * @param none. 
     * @return The current size of the queue. 
     * @brief Returns the current size of the queue. 
     * @warning none. 
     */
    size_t Size( ) const;

    /**
     * @date 2012/Sep/12 
     * @param none. 
     * @return The maximum capacity of the queue. 
     * @brief Returns the maximum capacity of the queue. 
     * @warning none. 
     */
    size_t Capacity( ) const;

    /**
     * @date 2012/Sep/12 
     * @param key: A key. 
     * @return none. 
     * @brief Inserts the input key into the random queue. If the queue is full, then the new key will
     * automatically replace the oldest key in the queue. 
     * @warning none. 
     */
    void Insert( double key );

    /**
     * @date 2012/Sep/12 
     * @param none. 
     * @return The removed key. 
     * @brief Removes and returns the oldest key in the queue. 
     * @warning none. 
     */
    double Remove( );

    /**
     * @date 2012/Sep/12 
     * @param key: A key. 
     * @param new_key: The new value to be assigned to key. 
     * @return none. 
     * @brief Updates the input key with new_key value. If there is more than one key with the expected value,
     * then one of them (randomly chosen) will updated. 
     * @warning none. 
     */
    void Update( double key, double new_key );

    /**
     * @date 2012/Sep/12 
     * @param none. 
     * @return The minimum key in the queue. 
     * @brief Returns the minimum key in the queue. If the number of keys is even, then the mean value of the two
     * minimum keys is returned. 
     * @warning Queue occupancy is not verified. 
     */
    double Minimum( ) const;

    /**
     * @date 2012/Sep/12 
     * @param none. 
     * @return The median key in the queue. 
     * @brief Returns the median key in the queue. If the number of keys is even, then the mean value of the two
     * median keys is returned. 
     * @warning Queue occupancy is not verified. 
     */
    double Median( ) const;

    /**
     * @date 2012/Sep/12 
     * @param none. 
     * @return The maximum key in the queue. 
     * @brief Returns the maximum key in the queue. If the number of keys is even, then the mean value of the two
     * maximum keys is returned. 
     * @warning Queue occupancy is not verified. 
     */
    double Maximum( ) const;

    /**
     * @date 2012/Sep/12 
     * @param none. 
     * @return True, if random queue is completely filled. 
     * @brief Returns true, if random queue is completely filled. 
     * @warning none. 
     */
    bool Full( );

    /**
     * @date 2012/Sep/13 
     * @param none. 
     * @return True, if random queue is empty. 
     * @brief Returns true, if random queue is empty. 
     * @warning none. 
     */
    bool Empty( );

    /**
     * @date 2012/Sep/12 
     * @param os: an output stream. 
     * @return none. 
     * @brief Prints random queue current keys. 
     * @warning none. 
     */
    template< class O >
    void Print( O &os );

  };

}

/* Implementation --------------------------------------------------------------------------------------------------- */

namespace Bial {

  template< class O >
  void RandomQueue::Print( O &os ) {
    try {
      for( size_t it = 0; it < occupancy; ++it ) {
        os << QK_DATA( it ) << " ";
      }
      os << std::endl;
    }
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

}

#include "RandomQueue.cpp"

#endif
