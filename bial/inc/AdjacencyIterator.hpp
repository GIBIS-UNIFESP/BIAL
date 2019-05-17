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

/* Declarations ----------------------------------------------------------------------------------------------------- */

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
    const Matrix< int > &relation;
    /** @brief Size of x dimension. */
    const size_t x_size;
    /** @brief Size of y dimension. */
    const size_t y_size;
    /** @brief Size of z dimension. */
    const size_t z_size;
    /** @brief Size of x * y dimensions. */
    const size_t xy_size;
    /** @brief Size of the image. */
    const size_t img_size;
    /** @brief Size of adjacency relation. */
    const size_t adj_size;
    /** @brief Number of dimensions in image and adjacency. */
    const size_t dims;
    /** @brief Vector containing the displacements in terms of global pixel index to the adjacent pixels. */
    Vector< int > displacement;

  public:

    /**
    * @date 2019/May/03
    * @param adj: The adjacency relation to iterate on.
    * @param x_size: Number of pixels in dimension x.
    * @param y_size: Number of pixels in dimension y.
    * @param z_size: Number of pixels in dimension z.
    * @return none.
    * @brief Basic Constructor.
    * @warning none.
    */
    AdjacencyIterator( const Adjacency &adj, size_t x_size, size_t y_size, size_t zsize = 1 );

     /**
     * @date 2014/Apr/14
     * @param img: An image used to compute the boundaries and other dimensions.
     * @param adj: The adjacency relation to iterate on.
     * @return none.
     * @brief Basic Constructor.
     * @warning none.
     */
    template< class D > 
    AdjacencyIterator( const Image< D > &img, const Adjacency &adj );

    /**
     * @date 2018/Nov/07
     * @param adj: The adjacency relation to iterate on.
     * @param img: An image used to compute the boundaries and other dimensions.
     * @return none.
     * @brief Basic Constructor.
     * @warning none.
     */
    template< class D > 
    AdjacencyIterator( const Adjacency &adj, const Image< D > &img );

    /**
    * @date 2019/May/03
    * @param copy: The adjacency iterator to be copied.
    * @return none.
    * @brief Copy Constructor.
    * @warning none.
    */
    AdjacencyIterator( const AdjacencyIterator &copy );

    /**
     * @date 2013/Oct/23
     * @param src_index: Index of source pixel in the adjacency relation.
     * @param adj_pos: Position of element in adjacency relation.
     * @return The index of the adjacent element in image.
     * @brief Returns the index of the adjacent element in image. No boundary verifications are done.
     * @warning none.
     */
    size_t operator()( size_t src_index, size_t adj_position ) const;

    /**
     * @date 2017/Apr/04
     * @param adj_pos: Position of element in adjacency relation.
     * @return The displacement of the adjacency position from the source.
     * @brief Returns the displacement of the adjacency position from the source..
     * @warning none.
     */
    int Displacement( size_t adj_position ) const;

    /**
     * @date 2013/Oct/23
     * @param src_index: Index of source pixel in the adjacency relation.
     * @param adj_pos: Position of element in adjacency relation.
     * @param adj_index: Returning index of adjacent pixel, if valid.
     * @return true if adjacent element is valid.
     * @brief Computes the index of the adjacent element in image. If the adjacent element is out of the image 
     * boundaries, returns false.
     * @warning For 2D adjacencies. Generic call with AdjIdx function.
     */
    bool AdjIdx2( size_t src_index, size_t adj_pos, size_t &adj_index ) const;

    /**
     * @date 2013/Oct/23
     * @param src_index: Index vector of source pixel in the adjacency relation.
     * @param adj_pos: Position of element in adjacency relation.
     * @param adj_index: Returning index vector of adjacent pixel, if valid.
     * @return true if adjacent element is valid.
     * @brief Computes the index vector of the adjacent element in image. If the adjacent element is out of the image
     * boundaries, returns false.
     * @warning For 2D adjacencies. Generic call with AdjVct function.
     */
    bool AdjVct2( const Vector< size_t > &src_index, size_t adj_pos, Vector< size_t > &adj_index ) const;

    /**
     * @date 2013/Oct/23
     * @param src_index: Index of source pixel in the adjacency relation.
     * @param adj_pos: Position of element in adjacency relation.
     * @param adj_index: Returning index of adjacent pixel, if valid.
     * @return true if adjacent element is valid.
     * @brief Computes the index of the adjacent element in image. If the adjacent element is out of the image 
     * boundaries, returns false.
     * @warning For 2D or 3D adjacencies.
     */
    bool AdjIdx( size_t src_index, size_t adj_pos, size_t &adj_index ) const;

    /**
     * @date 2013/Oct/23
     * @param src_index: Index vector of source pixel in the adjacency relation.
     * @param adj_pos: Position of element in adjacency relation.
     * @param adj_index: Returning index vector of adjacent pixel, if valid.
     * @return true if adjacent element is valid.
     * @brief Computes the index vector of the adjacent element in image. If the adjacent element is out of the image
     * boundaries, returns false.
     * @warning For 2D or 3D adjacencies.
     */
    bool AdjVct( const Vector< size_t > &src_index, size_t adj_pos, Vector< size_t > &adj_index ) const;

  };

}

/* Template functions or member functions of template classes that cannot be explicitly instantiated. ------------------ */

#include "AdjacencyIterator.cpp"

#endif



  // /**
  //  * @date 2015/Jul/03
  //  * @param adj: An adjacency relation.
  //  * @param vct: A vector with the dimensions of the image or matrix.
  //  * @param pixel_index: The pixel for which the adjacents must be looked to.
  //  * @return Iterator to first valid adjacent element.
  //  * @brief returns an iterator to first valid adjacent element. 
  //  * @warning none.
  //  */
  // template< class D >
  // AdjacencyIterator begin( const Adjacency &adj, const Vector< D > &vct, size_t pixel_index );

  // /**
  //  * @date 2015/Jul/03
  //  * @param adj: An adjacency relation.
  //  * @param vct: A vector with the dimensions of the image or matrix.
  //  * @param pixel_index: The pixel for which the adjacents must be looked to.
  //  * @return Iterator to position after the end of elements.
  //  * @brief Returns an iterator to position after the end of elements.
  //  * @warning none.
  //  */
  // template< class D >
  // AdjacencyIterator end( const Adjacency &adj, const Vector< D > &vct, size_t pixel_index );

  // /**
  //  * @date 2012/Sep/24
  //  * @param adj: An adjacency relation.
  //  * @param mtx: A matrix for data dimensions.
  //  * @param pixel_index: The pixel for which the adjacents must be looked to.
  //  * @return Iterator to first valid adjacent element.
  //  * @brief returns an iterator to first valid adjacent element. 
  //  * @warning none.
  //  */
  // template< class D >
  // AdjacencyIterator begin( const Adjacency &adj, const Matrix< D > &mtx, size_t pixel_index );

  // /**
  //  * @date 2012/Sep/24
  //  * @param adj: An adjacency relation.
  //  * @param mtx: A matrix for data dimensions.
  //  * @param pixel_index: The pixel for which the adjacents must be looked to.
  //  * @return Iterator to position after the end of elements.
  //  * @brief Returns an iterator to position after the end of elements.
  //  * @warning none.
  //  */
  // template< class D >
  // AdjacencyIterator end( const Adjacency &adj, const Matrix< D > &mtx, size_t pixel_index );

  // /**
  //  * @date 2014/Apr/14
  //  * @param adj: An adjacency relation.
  //  * @param img: A matrix for data dimensions.
  //  * @param pixel_index: The pixel for which the adjacents must be looked to.
  //  * @return Iterator to first valid adjacent element.
  //  * @brief returns an iterator to first valid adjacent element. 
  //  * @warning none.
  //  */
  // template< class D >
  // AdjacencyIterator begin( const Adjacency &adj, const Image< D > &img, size_t pixel_index );

  // /**
  //  * @date 2014/Apr/14
  //  * @param adj: An adjacency relation.
  //  * @param img: A matrix for data dimensions.
  //  * @param pixel_index: The pixel for which the adjacents must be looked to.
  //  * @return Iterator to position after the end of elements.
  //  * @brief Returns an iterator to position after the end of elements.
  //  * @warning none.
  //  */
  // template< class D >
  // AdjacencyIterator end( const Adjacency &adj, const Image< D > &img, size_t pixel_index );


    // //typedef size_t ( PathFunction< C, D >::*RemoveFn )( size_t index, BucketState state );

    // template< template< class D > class C, class D >
    // typename PathFunction< C, D >::RemoveFn PathFunction< C, D >::RemoveFunction( ) {
    //   return( &PathFunction< C, D >::RemoveSimple );
    // }

    // typename PathFunction< C, D >::RemoveFn RemoveFunction( );
