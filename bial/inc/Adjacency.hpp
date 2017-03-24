/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Jul/05
 * @brief Adjacency relation to be used over an image or matrix.
 */

#include "Common.hpp"
#include "Matrix.hpp"
#include "Vector.hpp"

#ifndef BIALADJACENCY_H
#define BIALADJACENCY_H

/* Declarations -------------------------------------------------------------------------------------------------------- */

namespace Bial {

  template< class D >
  class Image;
  union Color;
  /**
   * @brief Adjacency relation and forward iteratior related to a matrix.
   */
  class Adjacency {

  protected:

    /**
     * @brief
     * 2D matrix. First dimension refers to the number of dimensions of adjacency relation and the second dimension
     * refers to the size of the adjacency relation, that is, the number of elements in it.
     * Definition: relation(dims, size)
     */
    Matrix< int > relation;
    
  public:

    /**
     * @date 2012/Jul/06
     * @param none
     * @return none.
     * @brief Basic Constructor.
     * @warning Uninitialized adjacency is created. 
     */
    Adjacency( );

    /**
     * @date 2012/Jul/06
     * @param size: number of elements of the adjacency relation.
     * @param dims: number of dimensions of the adjacency relation.
     * @return none.
     * @brief Basic Constructor.
     * @warning Uninitialized adjacency is created. 
     */
    Adjacency( size_t size, size_t dims );

    /**
     * @date 2012/Jul/06
     * @param elem: Adjacency element index.
     * @param dim: Number of dimension of the element.
     * @return Data pointed by index ( elem, dim ).
     * @brief Returns data pointed by index ( elem, dim ). 
     * @warning Adjacency dimensions and bounds are not verified. 
     */
    const int &operator()( size_t elem, size_t dim ) const;

    /**
     * @date 2012/Jul/06
     * @param elem: Adjacency element index.
     * @param dim: Number of dimension of the element.
     * @return Reference of data pointed by index ( elem, dim ).
     * @brief Returns the reference for data pointed by index ( elem, dim ). 
     * @warning Adjacency dimensions and bounds are not verified. 
     */
    int &operator()( size_t elem, size_t dim );

    /**
     * @date 2012/Jul/06
     * @param increasing_order: true for sorting in reverse mode.
     * @return A Vector containing the position of the elements prior to sorting.
     * @brief Sort adjacency elements in increasing (or decreasing) order with respect to the distance from t
     * reference element
     * @warning none.
     */
    Vector< size_t > SortByDistance( bool increasing_order = true );

    /**
     * @date 2012/Jul/13
     * @param none.
     * @return Number of dimensions of the adjacency relation.
     * @brief Returns the number of dimensions of the adjacency relation.
     * @warning none.
     */
    size_t Dims( ) const;

    /**
     * @date 2012/Jul/13
     * @param none.
     * @return Number of elements of the adjacency relation.
     * @brief Returns the number of elements of the adjacency relation.
     * @warning none.
     */
    size_t Size( ) const;

    /**
     * @date 2012/Jul/13
     * @param none.
     * @return Number of elements of the adjacency relation.
     * @brief Same as Size( ). Just for coherence with Image.size( ). 
     * @warning none.
     */
    size_t size( ) const;

    /**
     * @date 2016/Dec/12
     * @param none.
     * @return Reference to the adjacency relation matrix.
     * @brief Returns a reference to adjacency relation matrix. 
     * @warning none.
     */
    const Matrix< int > &Relation( ) const;

    /**
     * @date 2012/Jun/21
     * @param os: an output stream.
     * @return os output stream.
     * @brief Prints adjacency relation dimensions to output stream os. 
     * @warning none.
     */
    template< class O >
    O &PrintDimensions( O &os ) const;

    /**
     * @date 2014/Oct/13
     * @param os: an output stream.
     * @return The output stream.
     * @brief Prints adjacency to output stream os. 
     * @warning none.
     */
    template< class O >
    O &Print( O &os ) const;

  };

  /**
   * @param os: output stream.
   * @param adj: an adjacency relation.
   * @return Reference to os.
   * @brief Prints the adjacency relation in a friendly way. 
   * @warning none.
   */
  template< class O >
  O &operator<<( O &os, const Adjacency &adj );

}

#include "Adjacency.cpp"

#endif
