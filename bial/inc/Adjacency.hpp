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
     * @param dims: number of dimensions of the adjacency relation. Should be greater or equal to the image that
     * will operate on.
     * @param size: number of elements of the adjacency relation.
     * @return none.
     * @brief Basic Constructor.
     * @warning Uninitialized adjacency is created. 
     */
    Adjacency( size_t dims, size_t size );

    /**
     * @date 2014/Jan/19
     * @param src_displacement: source displacement of the adjacency.
     * @param tgt_displacement: target displacement of the adjacency.
     * @return non.
     * @brief Basic Constructor. Build a n-dimensional box from src_displacement to tgt_displacement. 
     * @warning none.
     */
    Adjacency( const Vector< size_t > &src_displacement, const Vector< size_t > &tgt_displacement );

    /**
     * @date 2013/Nov/19
     * @param mat: A matrix.
     * @return none.
     * @brief Basic Constructor. Build an adjacency based on a matrix. Non-zero elements are set as adjacents a
     * coefficients are matrix values. Adjacency centralized in dim / 2.
     * @warning Uninitialized kernel is created.
     */
    template< class D > Adjacency( const Matrix< D > &mat );

    /**
     * @date 2013/Nov/19
     * @param img: An image.
     * @return none.
     * @brief Basic Constructor. Build an adjacency based on an image. Non-zero elements are set as adjacents a
     * coefficients are image values. Adjacency centralized in dim / 2.
     * @warning Uninitialized kernel is created.
     */
    template< class D > Adjacency( const Image< D > &img );
    Adjacency( const Image< Color > &img );

    /**
     * @date 2012/Jul/06
     * @param elem: Adjacency element index.
     * @param dim: Number of dimension of the element.
     * @return Data pointed by index ( elem, dim ).
     * @brief Returns data pointed by index ( elem, dim ). 
     * @warning Adjacency dimensions and bounds are not verified. 
     */
    int operator()( size_t elem, size_t dim ) const;

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
     * @date 2013/Oct/23
     * @param dim: Dimensions of the space domain where the adjacent element should be found.
     * @param index: The indexes of the source element.
     * @param adj_index: Adjacency relation index in the displacement dimensions.
     * @return A vector containing the indexes of the adjacent element to the given source element in space domain.
     * @brief Returns a vector containing the indexes of the adjacent element to the given source element in spa
     * domain. If the adjacent element is out of the space bounds it returns a copy of dim vector.
     * @warning No validation is done. 
     */
    Vector< size_t > operator()( const Vector< size_t > &dim, const Vector< size_t > &index, size_t adj_index ) const;

    /**
     * @date 2013/Oct/23
     * @param dim: Dimensions of the space where the element should be defined.
     * @param position: The global position of the source element.
     * @param adj_index: Adjacency relation index in the displacement dimensions.
     * @return The global index of the element in space 'dim' that is adjacent to element 'position' with respect to
     * index 'adj_index'.
     * @brief Returns the global index of the element in space 'dim' that is adjacent to element 'position' with
     * respect to index 'adj_index'. If the adjacent element is out of the space bounds, than matrix size
     * is returned.
     * @warning This method is slower than specialized Adj2 and Adj3. Call Adj2 and Adj3 methods instead, if
     * possible. 'this' adjacency relation and the space 'dim' must have the same number of dimensions. No
     * validation is done. 
     */
    size_t operator()( const Vector< size_t > &dim, size_t position, size_t adj_index ) const;

    /**
     * @date 2012/Jul/13
     * @param mat: The matrix where the adjacent element should be defined.
     * @param index: The indexes of the source element.
     * @param adj_index: Adjacency relation index in the displacement matrix.
     * @return A Vector containing the indexes of the element in matrix 'mat' that is adjacent to element 'ind
     * with respect to index 'adj_index'.
     * @brief Returns a Vector containing the indexes of the element in matrix 'mat' that is adjacent to 
     * element of indexes 'index' with respect to index 'adj_index'. If the adjacent element is out of the
     * matrix bounds, than NULL is returned.
     * @warning This method is slower than specialized Adj2 and Adj3. Call Adj2 and Adj3 methods instead, if
     * possible. 'this' adjacency relation, the index vector, and the matrix 'mat' must have the same number
     * of dimensions. No validation is done.
     */
    template< class D >
    Vector< size_t > operator()( const Matrix< D > &mat, const Vector< size_t > &index, size_t adj_index ) const;

    /**
     * @date 2012/Jul/13
     * @param mat: The matrix where the adjacent element should be defined.
     * @param position: The global position of the source element.
     * @param adj_index: Adjacency relation index in the displacement matrix.
     * @return The global index of the element in matrix 'mat' that is adjacent to element 'position' with respect to
     * index 'adj_index'.
     * @brief Returns the global index of the element in matrix 'mat' that is adjacent to element 'position' with
     * respect to index 'adj_index'. If the adjacent element is out of the matrix bounds, than matrix size
     * is returned.
     * @warning This method is slower than specialized Adj2 and Adj3. Call Adj2 and Adj3 methods instead, if
     * possible. 'this' adjacency relation and the matrix 'mat' must have the same number of dimensions. No
     * validation is done.
     */
    template< class D >
    size_t operator()( const Matrix< D > &mat, size_t position, size_t adj_index ) const;

    /**
     * @date 2014/Apr/14
     * @param img: The image where the adjacent element should be defined.
     * @param index: The indexes of the source element.
     * @param adj_index: Adjacency relation index in the displacement image.
     * @return A Vector containing the indexes of the element in image 'img' that is adjacent to element 'ind
     * with respect to index 'adj_index'.
     * @brief Returns a Vector containing the indexes of the element in image 'img' that is adjacent to
     * element of indexes 'index' with respect to index 'adj_index'. If the adjacent element is out of the
     * image bounds, than NULL is returned.
     * @warning This method is slower than specialized Adj2 and Adj3. Call Adj2 and Adj3 methods instead, if
     * possible. 'this' adjacency relation, the index vector, and the image 'img' must have the same number
     * of dimensions. No validation is done.
     */
    template< class D >
    Vector< size_t > operator()( const Image< D > &img, const Vector< size_t > &index, size_t adj_index ) const;

    /**
     * @date 2014/Apr/14
     * @param mat: The image where the adjacent element should be defined.
     * @param position: The global position of the source element.
     * @param adj_index: Adjacency relation index in the displacement image.
     * @return The global index of the element in image 'img' that is adjacent to element 'position' with respect to
     * index 'adj_index'.
     * @brief Returns the global index of the element in image 'img' that is adjacent to element 'position' with
     * respect to index 'adj_index'. If the adjacent element is out of the image bounds, than image size
     * is returned.
     * @warning This method is slower than specialized Adj2 and Adj3. Call Adj2 and Adj3 methods instead, if
     * possible. 'this' adjacency relation and the image 'img' must have the same number of dimensions. No
     * validation is done.
     */
    template< class D >
    size_t operator()( const Image< D > &img, size_t position, size_t adj_index ) const;

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
     * @date 2013/Oct/31
     * @param elem: Adjacency element index.
     * @param dim: Displacement dimension.
     * @return The displacement in dimention dim for adjacent elem.
     * @brief Returns the displacement in dimention dim for adjacent elem. 
     * @warning none.
     */
    int Displacement( size_t elem, size_t dim ) const;

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
     * @date 2013/Oct/23
     * @param dim: Dimensions of the space where the element should be defined.
     * @param index: The indexes of the source element.
     * @param adj_index: Adjacency relation index in the displacement dimensions.
     * @return A Vector containing the first two indexes of the element in space 'dim' that is adjacent to elem
     * 'index' with respect to index 'adj_index'.
     * @brief Returns a Vector containing the first two indexes of the element in space 'dim' that is
     * adjacent to element of indexes 'index' with respect to index 'adj_index'. If the adjacent element
     * is out of the space bounds, than NULL is returned.
     * @warning 'this' adjacency relation, the index vector, and the space 'dim' must have the same number of
     * dimensions. No validation is done. 
     */
    Vector< size_t > Adj2( const Vector< size_t > &dim, const Vector< size_t > &index, size_t adj_index ) const;

    /**
     * @date 2013/Oct/23
     * @param dim: Dimensions of the space where the element should be defined.
     * @param position: The global position of the source element.
     * @param adj_index: Adjacency relation index in the displacement dimensions.
     * @return The global index of the element in space 'dim' that is adjacent to element 'position' with respect to
     * index 'adj_index'.
     * @brief Returns the global index of the element in space 'dim' that is adjacent to element 'position' with
     * respect to index 'adj_index'. If the adjacent element is out of the space bounds, than matrix size
     * is returned.
     * @warning Input space must have two dimentions, and 'this' adjacency relation and the space 'dim' must have the
     * same number of dimensions. No validation is done. 
     */
    size_t Adj2( const Vector< size_t > &dim, size_t position, size_t adj_index ) const;

    /**
     * @date 2012/Jul/13
     * @param mat: The matrix where the adjacent element should be defined.
     * @param index: The indexes of the source element.
     * @param adj_index: Adjacency relation index in the displacement matrix.
     * @return A Vector containing the first two indexes of the element in matrix 'mat' that is adjacent to
     * element 'index' with respect to index 'adj_index'.
     * @brief Returns a Vector containing the first two indexes of the element in matrix 'mat' that is
     * adjacent to element of indexes 'index' with respect to index 'adj_index'. If the adjacent element
     * is out of the matrix bounds, than NULL is returned.
     * @warning 'this' adjacency relation, the index vector, and the matrix 'mat' must have the same number of
     * dimensions. No validation is done.
     */
    template< class D >
    Vector< size_t > Adj2( const Matrix< D > &mat, const Vector< size_t > &index, size_t adj_index ) const;

    /**
     * @date 2012/Jul/13
     * @param mat: The matrix where the adjacent element should be defined.
     * @param position: The global position of the source element.
     * @param adj_index: Adjacency relation index in the displacement matrix.
     * @return The global index of the element in matrix 'mat' that is adjacent to element 'position' with respect to
     * index 'adj_index'.
     * @brief Returns the global index of the element in matrix 'mat' that is adjacent to element 'position' with
     * respect to index 'adj_index'. If the adjacent element is out of the matrix bounds, than matrix size
     * is returned.
     * @warning Input matrix must have two dimentions, and 'this' adjacency relation and the matrix 'mat' must have the
     * same number of dimensions. No validation is done.
     */
    template< class D >
    size_t Adj2( const Matrix< D > &mat, size_t position, size_t adj_index ) const;

    /**
     * @date 2014/Apr/14
     * @param img: The image where the adjacent element should be defined.
     * @param index: The indexes of the source element.
     * @param adj_index: Adjacency relation index in the displacement image.
     * @return A Vector containing the first two indexes of the element in image 'img' that is adjacent to
     * element 'index' with respect to index 'adj_index'.
     * @brief Returns a Vector containing the first two indexes of the element in image 'img' that is
     * adjacent to element of indexes 'index' with respect to index 'adj_index'. If the adjacent element
     * is out of the image bounds, than NULL is returned.
     * @warning 'this' adjacency relation, the index vector, and the image 'img' must have the same number of
     * dimensions. No validation is done.
     */
    template< class D >
    Vector< size_t > Adj2( const Image< D > &img, const Vector< size_t > &index, size_t adj_index ) const;

    /**
     * @date 2014/Apr/14
     * @param img: The image where the adjacent element should be defined.
     * @param position: The global position of the source element.
     * @param adj_index: Adjacency relation index in the displacement image.
     * @return The global index of the element in image 'img' that is adjacent to element 'position' with respect to
     * index 'adj_index'.
     * @brief Returns the global index of the element in image 'img' that is adjacent to element 'position' with
     * respect to index 'adj_index'. If the adjacent element is out of the image bounds, than image size
     * is returned.
     * @warning Input image must have two dimentions, and 'this' adjacency relation and the image 'img' must have the
     * same number of dimensions. No validation is done.
     */
    template< class D >
    size_t Adj2( const Image< D > &img, size_t position, size_t adj_index ) const;

    /**
     * @date 2013/Oct/23
     * @param dim: Dimensions of the space where the element should be defined.
     * @param index: The indexes of the source element.
     * @param adj_index: Adjacency relation index in the displacement dimensions.
     * @return A Vector containing the first three indexes of the element in space 'dim' that is adjacent to
     * element 'index' with respect to index 'adj_index'.
     * @brief Returns a Vector containing the first three indexes of the element in space 'dim' that is
     * adjacent to element of indexes 'index' with respect to index 'adj_index'. If the adjacent element
     * is out of the space bounds, than NULL is returned.
     * @warning 'this' adjacency relation, the index vector, and the space 'dim' must have the same number of
     * dimensions. No validation is done. 
     */
    Vector< size_t > Adj3( const Vector< size_t > &dim, const Vector< size_t > &index, size_t adj_index ) const;

    /**
     * @date 2013/Oct/23
     * @param dim: Dimensions of the space where the element should be defined.
     * @param position: The global position of the source element.
     * @param adj_index: Adjacency relation index in the displacement dimensions.
     * @return The global index of the element in space 'dim' that is adjacent to element 'position' with respect to
     * index 'adj_index'.
     * @brief Returns the global index of the element in space 'dim' that is adjacent to element 'position' with
     * respect to index 'adj_index'. If the adjacent element is out of the space bounds, than matrix size
     * is returned.
     * @warning Input space must have three dimentions, and 'this' adjacency relation and the space 'dim' must have the
     * same number of dimensions. No validation is done. 
     */
    size_t Adj3( const Vector< size_t > &dim, size_t position, size_t adj_index ) const;

    /**
     * @date 2016/Dec/07
     * @param dim: Dimensions of the space where the element should be defined.
     * @param xy_size: Accumulated size of dimensions 0 and 1 of the image. I.e. size( 0 ) * size( 1 ).
     * @param position: The global position of the source element.
     * @param adj_index: Adjacency relation index in the displacement dimensions.
     * @return The global index of the element in space 'dim' that is adjacent to element 'position' with respect to
     * index 'adj_index'.
     * @brief Returns the global index of the element in space 'dim' that is adjacent to element 'position' with
     * respect to index 'adj_index'. If the adjacent element is out of the space bounds, than matrix size
     * is returned.
     * @warning Input space must have three dimentions, and 'this' adjacency relation and the space 'dim' must have the
     * same number of dimensions. No validation is done. Faster than the version that does not require xy_size.
     */
    size_t Adj3( const Vector< size_t > &dim, size_t xy_size, size_t position, size_t adj_index ) const;

    /**
     * @date 2012/Jul/13
     * @param mat: The matrix where the adjacent element should be defined.
     * @param index: The indexes of the source element.
     * @param adj_index: Adjacency relation index in the displacement matrix.
     * @return A Vector containing the first three indexes of the element in matrix 'mat' that is adjacent to
     * element 'index' with respect to index 'adj_index'.
     * @brief Returns a Vector containing the first three indexes of the element in matrix 'mat' that is
     * adjacent to element of indexes 'index' with respect to index 'adj_index'. If the adjacent element
     * is out of the matrix bounds, than NULL is returned.
     * @warning 'this' adjacency relation, the index vector, and the matrix 'mat' must have the same number of
     * dimensions. No validation is done.
     */
    template< class D >
    Vector< size_t > Adj3( const Matrix< D > &mat, const Vector< size_t > &index, size_t adj_index ) const;

    /**
     * @date 2012/Jul/13
     * @param mat: The matrix where the adjacent element should be defined.
     * @param position: The global position of the source element.
     * @param adj_index: Adjacency relation index in the displacement matrix.
     * @return The global index of the element in matrix 'mat' that is adjacent to element 'position' with respect to
     * index 'adj_index'.
     * @brief Returns the global index of the element in matrix 'mat' that is adjacent to element 'position' with
     * respect to index 'adj_index'. If the adjacent element is out of the matrix bounds, than matrix size
     * is returned.
     * @warning Input matrix must have three dimentions, and 'this' adjacency relation and the matrix 'mat' must have
     * the same number of dimensions. No validation is done.
     */
    template< class D >
    size_t Adj3( const Matrix< D > &mat, size_t position, size_t adj_index ) const;
    /**
     * @date 2014/Apr/14
     * @param img: The image where the adjacent element should be defined.
     * @param index: The indexes of the source element.
     * @param adj_index: Adjacency relation index in the displacement image.
     * @return A Vector containing the first three indexes of the element in image 'img' that is adjacent to
     * element 'index' with respect to index 'adj_index'.
     * @brief Returns a Vector containing the first three indexes of the element in image 'img' that is
     * adjacent to element of indexes 'index' with respect to index 'adj_index'. If the adjacent element
     * is out of the image bounds, than NULL is returned.
     * @warning 'this' adjacency relation, the index vector, and the image 'img' must have the same number of
     * dimensions. No validation is done.
     */
    template< class D >
    Vector< size_t > Adj3( const Image< D > &img, const Vector< size_t > &index, size_t adj_index ) const;

    /**
     * @date 2014/Apr/14
     * @param img: The image where the adjacent element should be defined.
     * @param position: The global position of the source element.
     * @param adj_index: Adjacency relation index in the displacement image.
     * @return The global index of the element in image 'img' that is adjacent to element 'position' with respect to
     * index 'adj_index'.
     * @brief Returns the global index of the element in image 'img' that is adjacent to element 'position' with
     * respect to index 'adj_index'. If the adjacent element is out of the image bounds, than image size
     * is returned.
     * @warning Input image must have three dimentions, and 'this' adjacency relation and the image 'img' must have
     * the same number of dimensions. No validation is done.
     */
    template< class D >
    size_t Adj3( const Image< D > &img, size_t position, size_t adj_index ) const;

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
