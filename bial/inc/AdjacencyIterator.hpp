/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Jul/05
 * @brief Adjacency iterator class used as a forward iteratior for adjacency relation.
 * <br> Future add-on's: none.
 */

#include "Adjacency.hpp"
#include "Common.hpp"
#include "Matrix.hpp"
#include "Vector.hpp"

#ifndef BIALADJACENCYITERATOR_H
#define BIALADJACENCYITERATOR_H

/* Declarations -------------------------------------------------------------------------------------------------------- */

namespace Bial {

  template< class D >
  class Image;
  class AdjacencyIterator;
  /**
   * @brief Adjacency iterator.
   */
  class AdjacencyIterator {

  private:
    /** @brief Adjacency relation. */
    const Adjacency &adjacency;
    /** @brief Dimensions of the image or matrix. */
    Vector< size_t > dim_size;
    /** @brief Size of the image or matrix. */
    size_t data_limit;
    /** @brief Index of the source element in the image or matrix. */
    size_t pixel_index;
    /** @brief Index of the adjacent element in the image or matrix. */
    size_t adj_index;
    /** @brief Index of the current adjacent element in adjacency relation matrix. */
    size_t position; 
    /** @brief Vector containing the displacements to the adjacent pixels. */
    // Vector< int > displacement;

  public:


    /**
     * @date 2012/Sep/24
     * @param adj: An adjacency relation.
     * @param dim: Matrix dimensions.
     * @param index: Index of reference pixel in the adjacency relation.
     * @return none.
     * @brief Basic Constructor.
     * @warning none.
     */
    AdjacencyIterator( const Adjacency &adj, const Vector< size_t > &dim, size_t index );

    /**
     * @date 2012/Sep/24
     * @param adj: An adjacency relation.
     * @param mat: A matrix.
     * @param index: Index of reference pixel in the adjacency relation.
     * @return none.
     * @brief Basic Constructor.
     * @warning none.
     */
    template< class D > AdjacencyIterator( const Adjacency &adj, const Matrix< D > &mat, size_t index );

    /**
     * @date 2014/Apr/14
     * @param adj: An adjacency relation.
     * @param img: An image.
     * @param index: Index of reference pixel in the adjacency relation.
     * @return none.
     * @brief Basic Constructor.
     * @warning none.
     */
    template< class D > AdjacencyIterator( const Adjacency &adj, const Image< D > &mat, size_t index );

    /**
     * @date 2012/Sep/24
     * @param none
     * @return none.
     * @brief Sets position to first valid index.
     * @warning none.
     */
    void begin( );

    /**
     * @date 2012/Sep/24
     * @param none
     * @return none.
     * @brief Sets position to adjacency.size( ). 
     * @warning none.
     */
    void end( );

    /**
     * @date 2013/Oct/23
     * @param none.
     * @return Reference to this.
     * @brief Sets position to the next valid element or to space size if finished. 
     * @warning none.
     */
    AdjacencyIterator &operator++( );

    /**
     * @date 2013/Oct/23
     * @param none.
     * @return Copy to this, moved to next valid element.
     * @brief Sets position to the next valid element or to space size if finished. 
     * @warning none.
     */
    AdjacencyIterator operator++( int );

    /**
     * @date 2012/Sep/24
     * @param cmp: iterator to which this iterator is compared.
     * @return True, if this position is the same as cmp position.
     * @brief Checks if this position is the same as cmp position. 
     * @warning none.
     */
    bool operator==( const AdjacencyIterator &cmp ) const;

    /**
     * @date 2012/Sep/24
     * @param cmp: iterator to which this iterator is compared.
     * @return True, if this position is different of cmp position.
     * @brief Checks if this position is different of cmp position. 
     * @warning none.
     */
    bool operator!=( const AdjacencyIterator &cmp ) const;

    /**
     * @date 2012/Sep/24
     * @param none.
     * @return Ponter to the next valid element.
     * @brief Returns a ponter to the next valid element. 
     * @warning none.
     */
    size_t operator*( ) const;

    /**
     * @date 2015/Jun/10
     * @param none.
     * @return The position of current adjacent element.
     * @brief Returns the position of current adjacent element. 
     * @warning none.
     */
    size_t Position( ) const;

    friend class Adjacency;
  };

  /**
   * @date 2015/Jul/03
   * @param adj: An adjacency relation.
   * @param vct: A vector with the dimensions of the image or matrix.
   * @param pixel_index: The pixel for which the adjacents must be looked to.
   * @return Iterator to first valid adjacent element.
   * @brief returns an iterator to first valid adjacent element. 
   * @warning none.
   */
  template< class D >
  AdjacencyIterator begin( const Adjacency &adj, const Vector< D > &vct, size_t pixel_index );

  /**
   * @date 2015/Jul/03
   * @param adj: An adjacency relation.
   * @param vct: A vector with the dimensions of the image or matrix.
   * @param pixel_index: The pixel for which the adjacents must be looked to.
   * @return Iterator to position after the end of elements.
   * @brief Returns an iterator to position after the end of elements.
   * @warning none.
   */
  template< class D >
  AdjacencyIterator end( const Adjacency &adj, const Vector< D > &vct, size_t pixel_index );

  /**
   * @date 2012/Sep/24
   * @param adj: An adjacency relation.
   * @param mtx: A matrix for data dimensions.
   * @param pixel_index: The pixel for which the adjacents must be looked to.
   * @return Iterator to first valid adjacent element.
   * @brief returns an iterator to first valid adjacent element. 
   * @warning none.
   */
  template< class D >
  AdjacencyIterator begin( const Adjacency &adj, const Matrix< D > &mtx, size_t pixel_index );

  /**
   * @date 2012/Sep/24
   * @param adj: An adjacency relation.
   * @param mtx: A matrix for data dimensions.
   * @param pixel_index: The pixel for which the adjacents must be looked to.
   * @return Iterator to position after the end of elements.
   * @brief Returns an iterator to position after the end of elements.
   * @warning none.
   */
  template< class D >
  AdjacencyIterator end( const Adjacency &adj, const Matrix< D > &mtx, size_t pixel_index );

  /**
   * @date 2014/Apr/14
   * @param adj: An adjacency relation.
   * @param img: A matrix for data dimensions.
   * @param pixel_index: The pixel for which the adjacents must be looked to.
   * @return Iterator to first valid adjacent element.
   * @brief returns an iterator to first valid adjacent element. 
   * @warning none.
   */
  template< class D >
  AdjacencyIterator begin( const Adjacency &adj, const Image< D > &img, size_t pixel_index );

  /**
   * @date 2014/Apr/14
   * @param adj: An adjacency relation.
   * @param img: A matrix for data dimensions.
   * @param pixel_index: The pixel for which the adjacents must be looked to.
   * @return Iterator to position after the end of elements.
   * @brief Returns an iterator to position after the end of elements.
   * @warning none.
   */
  template< class D >
  AdjacencyIterator end( const Adjacency &adj, const Image< D > &img, size_t pixel_index );

}

/* Template functions or member functions of template classes that cannot be explicitly instantiated. ------------------ */

/* Call to cpp file. --------------------------------------------------------------------------------------------------- */

#include "AdjacencyIterator.cpp"

#endif
