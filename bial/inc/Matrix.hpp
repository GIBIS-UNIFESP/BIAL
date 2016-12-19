/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Jun/21
 * @brief N dimensional matrix handeling. When acessed by (x, y), or (x, y, z) operator, index x corresponds to
 * the index of the column, and index y corresponds to the index of the row. This standard comes from text printing
 * order and image display and storage in most of the computer systems.
 */

#include <numeric>
#include "Common.hpp"

#ifndef BIALMATRIX_H
#define BIALMATRIX_H

#include "Vector.hpp"

namespace Bial {

  class Adjacency;
  template< class D >
  class Feature;
  class Kernel;
  // template< class D > template< class O > O& operator<< ( O& os, const Matrix< D >& mat );

  /**
   * @brief  dimensional matrix handeling. When acessed by (x, y), or (x, y, z) operator, index x corresponds to
   * the index of the column, and index y corresponds to the index of the row. This standard comes from text printing
   * order and image display and storage in most of the computer systems.
   */
  template< class D >
  class Matrix {

    friend class Adjacency;
    friend class Kernel;
    template< class D2 >
    friend class Matrix;
    template< class D2 >
    friend class Image;

  protected:

    /** @brief  data vector */
    Vector< D > _data;
    /** @brief  Quick access reference for data. */
    D *qk_data;
    /** @brief  data size */
    size_t _size;
    /** @brief  number of matrix dimensions */
    size_t dims;
    /** @brief  size of matrix dimensions. */
    Vector< size_t > dim_size;
    /** @brief  accumulated size of matrix dimensions. */
    Vector< size_t > acc_dim_size;


  public:

    /**
     * @date 2012/Jun/21
     * @param none.
     * @return none.
     * @brief Basic Constructor.
     * @warning null matrix.
     */
    Matrix( );

    /**
     * @date 2012/Jun/21
     * @param size: Vector with matrix dimensions.
     * @return none.
     * @brief Basic Constructor.
     * @warning Uninitialized element values.
     */
    Matrix( const Vector< size_t > &size );

    /**
     * @date 2015/Set/10
     * @param new_data: Pointer to data allocated elsewhere.
     * @param new_dim: Matrix dimensions.
     * @return none.
     * @brief Basic Constructor. This constructor works as a wrapper for data already allocated. It will use
     * the same space pointed by new_data to manipulate the matrix. Use this for compatibility
     * with other data structures, in a easy convertion to Matrix class.
     * @warning Data is not deallocated automatcally, since it may be stored in the stack. Also, DO NOT USE this
     * constructure in Verbose or Debug compilation mode, as it will try to access invalid memory positions. Just
     * compile without any Verbosity or Debug flags.
     */
    Matrix( D *new_data, const Vector< size_t > &new_dim );

    /**
     * @date 2012/Jun/21
     * @param size_0: matrix dimension 0.
     * @param size_1: matrix dimension 1.
     * @return none.
     * @brief Basic Constructor for 2D matrix.
     * @warning Uninitialized element values.
     */
    Matrix( size_t size_0, size_t size_1 );

    /**
     * @date 2012/Jun/21
     * @param size_0: matrix dimension 0.
     * @param size_1: matrix dimension 1.
     * @param size_2: matrix dimension 2.
     * @return none.
     * @brief Basic Constructor for 3D matrix.
     * @warning Uninitialized element values.
     */
    Matrix( size_t size_0, size_t size_1, size_t size_2 );

    /**
     * @date 2014/Oct/12
     * @param mtx: Source matrix to be clonned.
     * @return none.
     * @brief Copy constructor.
     * @warning none.
     */
    Matrix( const Matrix< D > &mtx );

    /**
     * @date 2014/Oct/12
     * @param mtx: Source matrix to be clonned.
     * @return none.
     * @brief Move constructor.
     * @warning none.
     */
    Matrix( Matrix< D > && ) = default;

    /**
     * @date 2013/jun/27
     * @param mtx: Source matrix to be clonned.
     * @return none.
     * @brief Copy constructor.
     * @warning none.
     */
    template< class D2 > Matrix( const Matrix< D2 > &mtx );

    /**
     * @date 2014/Oct/10
     * @param other: matrix to be assigned.
     * @return none.
     * @brief Assigns 'other' matrix content to this matrix.
     * @warning none.
     */
    Matrix< D > &operator=( const Matrix< D > &other );

    /**
     * @date 2014/Oct/10
     * @param other: matrix to be assigned.
     * @return none.
     * @brief Assigns 'other' matrix content to this matrix, moving the content.
     * @warning none.
     */
    Matrix< D > &operator=( Matrix< D > && ) = default;

    /**
     * @date 2014/Oct/10
     * @param other: matrix to be assigned.
     * @return none.
     * @brief Assigns 'other' matrix content to this matrix.
     * @warning none.
     */
    template< class D2 >
    Matrix< D > &operator=( const Matrix< D2 > &other );

    /**
     * @date 2012/Jun/21
     * @param other: matrix to be compared.
     * @return true if matrices are equal.
     * @brief The matrices are considered equal if they have the same dimensions and their elements have the same
     * value.
     * @warning Equality may be tricky for floating point matrices. In these cases, use Equals method.
     */
    template< class D2 >
    bool operator==( const Matrix< D2 > &other ) const;

    /**
     * @date 2014/Nov/03
     * @param none.
     * @return Reference to this.
     * @brief Increments all elements of this matrix and returns it.
     * @warning none.
     */
    Matrix< D > &operator++( );

    /**
     * @date 2014/Nov/03
     * @param none.
     * @return A copy of this matrix before incrementing its element's intensities.
     * @brief Increments all elements of this matrix and returns it.
     * @warning none.
     */
    Matrix< D > operator++( int );

    /**
     * @date 2014/Nov/03
     * @param none.
     * @return Reference to this.
     * @brief Decrements all elements of this matrix and returns it.
     * @warning none.
     */
    Matrix< D > &operator--( );

    /**
     * @date 2014/Nov/03
     * @param none.
     * @return A copy of this matrix before decrementing its element's intensities.
     * @brief Decrements all elements of this matrix and returns it.
     * @warning none.
     */
    Matrix< D > operator--( int );

    /**
     * @date 2012/Jun/21
     * @param other: second other.
     * @return Sum of matrices 'this' and other.
     * @brief Computes the sum of the matrices 'this' and other and returns the resultant matrix.
     * @warning 'this' matrix must have the same dimensions of other matrix.
     */
    template< class D2 >
    Matrix< D > operator+( const Matrix< D2 > &other ) const;

    /**
     * @date 2012/Jun/21
     * @param other: second other.
     * @return Reference to 'this'.
     * @brief Computes the sum of the matrices 'this' and other and assigns the result to this.
     * @warning 'this' matrix must have the same dimensions of other matrix.
     */
    template< class D2 >
    Matrix< D > &operator+=( const Matrix< D2 > &other );

    /**
     * @date 2012/Jun/21
     * @param other: second other.
     * @return Negative matrix of 'this'.
     * @brief Computes the negative matrix of 'this' and returns the resultant matrix.
     * @warning none.
     */
    Matrix< D > operator-( ) const;

    /**
     * @date 2012/Jun/21
     * @param other: second other.
     * @return Sum of matrices 'this' and other.
     * @brief Computes the difference of the matrices 'this' and other and returns the resultant matrix.
     * @warning 'this' matrix must have the same dimensions of other matrix.
     */
    template< class D2 >
    Matrix< D > operator-( const Matrix< D2 > &other ) const;

    /**
     * @date 2012/Jun/21
     * @param other: second other.
     * @return Reference to 'this'.
     * @brief Computes the difference of the matrices 'this' and other and assigns the result to this.
     * @warning 'this' matrix must have the same dimensions of other matrix.
     */
    template< class D2 >
    Matrix< D > &operator-=( const Matrix< D2 > &other );

    /**
     * @date 2012/Jun/21
     * @param other: second other.
     * @return Product of matrices 'this' and other.
     * @brief Computes the product of the matrices 'this' and other and returns the resultant matrix.
     * @warning 'this' matrix must have the same dimensions of other matrix.
     */
    template< class D2 >
    Matrix< D > operator*( const Matrix< D2 > &other ) const;

    /**
     * @date 2012/Jun/21
     * @param other: second other.
     * @return Reference to 'this'.
     * @brief Computes the product of the matrices 'this' and other and assigns the result to this.
     * @warning 'this' matrix must have the same dimensions of other matrix.
     */
    template< class D2 >
    Matrix< D > &operator*=( const Matrix< D2 > &other );

    /**
     * @date 2012/jul/02
     * @param scalar: a scalar.
     * @return Sum of 'this' matrix by the scalar.
     * @brief Computes the sum of 'this' matrix by the scalar.
     * @warning none.
     */
    Matrix< D > operator+( double scalar ) const;

    /**
     * @date 2012/jul/02
     * @param scalar: a scalar.
     * @return reference to 'this'.
     * @brief Updates 'this' matrix with the sum of 'this' by the input scalar.
     * @warning none.
     */
    Matrix< D > &operator+=( double scalar );

    /**
     * @date 2012/jul/02
     * @param scalar: a scalar.
     * @return Difference between 'this' matrix and the scalar.
     * @brief Computes the difference between 'this' matrix and the scalar.
     * @warning none.
     */
    Matrix< D > operator-( double scalar ) const;

    /**
     * @date 2012/jul/02
     * @param scalar: a scalar.
     * @return reference to 'this'.
     * @brief Updates 'this' matrix with the difference between 'this' and the input scalar.
     * @warning none.
     */
    Matrix< D > &operator-=( double scalar );

    /**
     * @date 2012/Jun/21
     * @param scalar: a scalar.
     * @return Product of 'this' matrix by the scalar.
     * @brief Computes the product of 'this' matrix by the scalar.
     * @warning 'this' matrix must have the same dimensions of other matrix.
     */
    Matrix< D > operator*( double scalar ) const;

    /**
     * @date 2012/Jun/21
     * @param scalar: a scalar.
     * @return reference to 'this'.
     * @brief Updates 'this' matrix with the product of 'this' by the input scalar.
     * @warning 'this' matrix must have the same dimensions of other matrix.
     */
    Matrix< D > &operator*=( double scalar );

    /**
     * @date 2012/Jun/21
     * @param other: second other.
     * @return Division of matrices 'this' and other.
     * @brief Computes the division of the matrices 'this' and other and returns the resultant matrix.
     * @warning 'this' matrix must have the same dimensions of other matrix.
     */
    template< class D2 >
    Matrix< D > operator/( const Matrix< D2 > &other ) const;

    /**
     * @date 2012/Jun/21
     * @param other: second other.
     * @return Reference to 'this'.
     * @brief Computes the division of the matrices 'this' and other and assigns the result to this.
     * @warning 'this' matrix must have the same dimensions of other matrix.
     */
    template< class D2 >
    Matrix< D > &operator/=( const Matrix< D2 > &other );

    /**
     * @date 2012/Jun/21
     * @param scalar: a scalar.
     * @return Division of 'this' matrix by the scalar.
     * @brief Computes the division of 'this' matrix by the scalar.
     * @warning 'this' matrix must have the same dimensions of other matrix.
     */
    Matrix< D > operator/( double scalar ) const;

    /**
     * @date 2012/Jun/21
     * @param scalar: a scalar.
     * @return reference to 'this'.
     * @brief Updates 'this' matrix with the division of 'this' by the input scalar.
     * @warning 'this' matrix must have the same dimensions of other matrix.
     */
    Matrix< D > &operator/=( double scalar );

    /**
     * @date 2012/Jun/21
     * @param p: a global index to be acessed.
     * @return Constant reference of data pointed by index p.
     * @brief Returns a constant reference of data pointed by index p. It has the same effect as operator
     * data( p ).
     * @warning Matrix bounds are not verified.
     */
    const D &operator[]( size_t p ) const;

    /**
     * @date 2012/Jun/21
     * @param p: a global index to be acessed.
     * @return Reference of data pointed by index p.
     * @brief Returns the reference for data pointed by index p. It has the same effect as operator data( p ).
     * @warning Matrix bounds are not verified.
     */
    D &operator[]( size_t p );

    /**
     * @date 2012/Jun/21
     * @param p: a global index to be acessed.
     * @return Constant reference of data pointed by index p.
     * @brief Returns constant reference of data pointed by index p. It has the same effect as operator
     * data[ p ].
     * @warning Matrix bounds are not verified, as in operator[].
     */
    const D &operator()( size_t p ) const;

    /**
     * @date 2012/Jun/21
     * @param p: a global index to be acessed.
     * @return Reference of data pointed by index p.
     * @brief Returns the reference for data pointed by index p. It has the same effect as operator data[ p ].
     * @warning Matrix bounds are not verified, as in operator[].
     */
    D &operator()( size_t p );

    /**
     * @date 2012/Jun/21
     * @param p_0: first index to be acessed.
     * @param p_1: second index to be acessed.
     * @return Constant reference of data pointed by index ( p_0, p_1 ).
     * @brief Returns constant reference of data pointed by index ( p_0, p_1 ). It has the same effect as operator
     * data[ p_0 + p_1 * dim_size[ 0 ] ].
     * @warning Matrix dimensions and bounds are not verified, as in operator data[]. Designed for 2D matrices. The
     * behavior is undefined for other dimensions.
     */
    const D &operator()( size_t p_0, size_t p_1 ) const;

    /**
     * @date 2012/Jun/21
     * @param p_0: first index to be acessed.
     * @param p_1: second index to be acessed.
     * @return Reference of data pointed by index ( p_0, p_1 ).
     * @brief Returns the reference for data pointed by index ( p_0, p_1 ). It has the same effect as operator
     * data[ p_0 + p_1 * dim_size[ 0 ] ].
     * @warning Matrix dimensions and bounds are not verified, as in operator data[]. Designed for 2D matrices. The
     * behavior is undefined for other dimensions.
     */
    D &operator()( size_t p_0, size_t p_1 );

    /**
     * @date 2012/Jun/21
     * @param p_0: first index to be acessed.
     * @param p_1: second index to be acessed.
     * @param p_2: third index to be acessed.
     * @return Constant reference of data pointed by index ( p_0, p_1, p_2 ).
     * @brief Returns constant reference of data pointed by index ( p_0, p_1, p_2 ). It has the same effect as
     * operator data[ p_0 + p_1 * dim_size[ 0 ] + p_2 * acc_dim_size[ 1 ] ].
     * @warning Matrix dimensions and bounds are not verified, as in operator data[]. Designed for 3D matrices. The
     * behavior is undefined for other dimensions.
     */
    const D &operator()( size_t p_0, size_t p_1, size_t p_2 ) const;

    /**
     * @date 2012/Jun/21
     * @param p_0: first index to be acessed.
     * @param p_1: second index to be acessed.
     * @param p_2: third index to be acessed.
     * @return Reference of data pointed by index ( p_0, p_1, p_2 ).
     * @brief Returns the reference for data pointed by index ( p_0, p_1, p_2 ). It has the same effect as
     * operator
     * data[ p_0 + p_1 * dim_size[ 0 ] + p_2 * acc_dim_size[ 1 ] ].
     * @warning Matrix dimensions and bounds are not verified, as in operator data[]. Designed for 3D matrices. The
     * behavior is undefined for other dimensions.
     */
    D &operator()( size_t p_0, size_t p_1, size_t p_2 );

    /**
     * @date 2013/Aug/21
     * @param p_0: first index to be acessed.
     * @param p_1: second index to be acessed.
     * @param p_2: third index to be acessed.
     * @param p_3: fourth index to be acessed.
     * @return Constant reference of data pointed by index ( p_0, p_1, p_2, p_3 ).
     * @brief Returns constant reference of data pointed by index ( p_0, p_1, p_2, p_3 ). It has the same effect
     * as operator data[ p_0 + p_1 * dim_size[ 0 ] + p_2 * acc_dim_size[ 1 ] + p_3 * acc_dim_size[ 2 ]
     * ].
     * @warning Matrix dimensions and bounds are not verified, as in operator data[]. Designed for 4D matrices. The
     * behavior is undefined for other dimensions.
     */
    const D &operator()( size_t p_0, size_t p_1, size_t p_2, size_t p_3 ) const;

    /**
     * @date 2012/Jun/21
     * @param p_0: first index to be acessed.
     * @param p_1: second index to be acessed.
     * @param p_2: third index to be acessed.
     * @param p_3: fourth index to be acessed.
     * @return Reference of data pointed by index ( p_0, p_1, p_2, p_3 ).
     * @brief Returns the reference for data pointed by index ( p_0, p_1, p_2, p_3 ). It has the same effect as
     * operator data[ p_0 + p_1 * dim_size[ 0 ] + p_2 * acc_dim_size[ 1 ] + p_3 * acc_dim_size[ 2 ] ].
     * @warning Matrix dimensions and bounds are not verified, as in operator data[]. Designed for 4D matrices. The
     * behavior is undefined for other dimensions.
     */
    D &operator()( size_t p_0, size_t p_1, size_t p_2, size_t p_3 );

    /**
     * @date 2014/Apr/14
     * @param p_0: first index to be acessed.
     * @param p_1: second index to be acessed.
     * @param p_2: third index to be acessed.
     * @param p_3: fourth index to be acessed.
     * @param p_4: fifth index to be acessed.
     * @return Constant reference of data pointed by index ( p_0, p_1, p_2, p_3, p_4 ).
     * @brief Returns constant reference of data pointed by index ( p_0, p_1, p_2, p_3, p_4 ). It has the same
     * effect as operator data[ p_0 + p_1 * dim_size[ 0 ] + p_2 * acc_dim_size[ 1 ] + p_3 * acc_dim_size[ 2
     * ] + p_4 * acc_dim_size[ 3 ] ].
     * @warning Matrix dimensions and bounds are not verified, as in operator data[]. Designed for 5D matrices. The
     * behavior is undefined for other dimensions.
     */
    const D &operator()( size_t p_0, size_t p_1, size_t p_2, size_t p_3, size_t p_4 ) const;

    /**
     * @date 2014/Apr/14
     * @param p_0: first index to be acessed.
     * @param p_1: second index to be acessed.
     * @param p_2: third index to be acessed.
     * @param p_3: fourth index to be acessed.
     * @param p_4: fifth index to be acessed.
     * @return Reference of data pointed by index ( p_0, p_1, p_2, p_3, p_4 ).
     * @brief Returns the reference for data pointed by index ( p_0, p_1, p_2, p_3, p_4 ). It has the same effect
     * as operator data[ p_0 + p_1 * dim_size[ 0 ] + p_2 * acc_dim_size[ 1 ] + p_3 * acc_dim_size[ 2 ] +
     * p_4 * acc_dim_size[ 3 ] ].
     * @warning Matrix dimensions and bounds are not verified, as in operator data[]. Designed for 5D matrices. The
     * behavior is undefined for other dimensions.
     */
    D &operator()( size_t p_0, size_t p_1, size_t p_2, size_t p_3, size_t p_4 );

    /**
     * @date 2013/Jun/25
     * @param p: Vector of indexes to be acessed.
     * @return Constant reference of data pointed by index Vector p.
     * @brief Returns constant reference of data pointed by index ( p[ 0 ], p[ 1 ], p[ 2 ], ... ). It has the same
     * effect as operator data[ p_0 + p_1 * dim_size[ 0 ] + p_2 * acc_dim_size[ 1 ] ... ].
     * @warning Matrix dimensions and bounds are not verified, as in operator data[]. Designed for N-D matrices.
     */
    const D &operator()( const Vector< size_t > &p ) const;

    /**
     * @date 2013/Jun/25
     * @param p: Vector of indexes to be acessed.
     * @return Reference of data pointed by index Vector p.
     * @brief Returns the reference of data pointed by index ( p[ 0 ], p[ 1 ], p[ 2 ], ... ). It has the same
     * effect as operator data[ p_0 + p_1 * dim_size[ 0 ] + p_2 * acc_dim_size[ 1 ] ... ].
     * @warning Matrix dimensions and bounds are not verified, as in operator data[]. Designed for N-D matrices.
     */
    D &operator()( const Vector< size_t > &p );

    /**
     * @date 2013/Nov/06
     * @param none.
     * @return Nothing.
     * @brief Computes the absolute value of the matrix elements.
     * @warning none.
     */
    void Abs( );

    /**
     * @date 2013/Nov/06
     * @param none.
     * @return Matrix with absolute values of all elements from this matrix.
     * @brief Computes the absolute value of the matrix elements.
     * @warning none.
     */
    Matrix< D > Abs( ) const;

    /**
     * @date 2012/Jun/29
     * @param none.
     * @return iterator begin() of data.
     * @brief Returns iterator begin() of data.
     * @warning none.
     */
    typename std::vector< D >::iterator begin( ) noexcept;

    /**
     * @date 2012/Jun/29
     * @param none.
     * @return const_iterator begin() of data.
     * @brief Returns const_iterator begin() of data.
     * @warning none.
     */
    typename std::vector< D >::const_iterator begin( ) const noexcept;

    /**
     * @date 2012/Jun/29
     * @param none.
     * @return iterator end() of data.
     * @brief Returns iterator end() of data.
     * @warning none.
     */
    typename std::vector< D >::iterator end( ) noexcept;

    /**
     * @date 2012/Jun/29
     * @param none.
     * @return const_iterator end() of data.
     * @brief Returns const_iterator end() of data.
     * @warning none.
     */
    typename std::vector< D >::const_iterator end( ) const noexcept;

    /**
     * @date 2013/Aug/09
     * @param none.
     * @return reverse_iterator rbegin() of data Vector.
     * @brief Returns the reverse_iterator rbegin() of data Vector.
     * @warning none.
     */
    typename std::vector< D >::reverse_iterator rbegin( ) noexcept;

    /**
     * @date 2013/Aug/09
     * @param none.
     * @return const_reverse_iterator rbegin() of data Vector.
     * @brief Returns const_reverse_iterator rbegin() of data Vector.
     * @warning none.
     */
    typename std::vector< D >::const_reverse_iterator rbegin( ) const noexcept;

    /**
     * @date 2013/Aug/09
     * @param none.
     * @return reverse_iterator rend() of data Vector.
     * @brief Returns reverse_iterator rend() of data Vector.
     * @warning none.
     */
    typename std::vector< D >::reverse_iterator rend( ) noexcept;

    /**
     * @date 2013/Aug/09
     * @param none.
     * @return const_reverse_iterator rend() of data Vector.
     * @brief Returns const_reverse_iterator rend() of data Vector.
     * @warning none.
     */
    typename std::vector< D >::const_reverse_iterator rend( ) const noexcept;

    /**
     * @date 2015/Apr/06
     * @param none.
     * @return iterator cbegin() of data Vector.
     * @brief Returns the iterator cbegin() of data Vector.
     * @warning none.
     */
    typename std::vector< D >::const_iterator cbegin( ) const noexcept;

    /**
     * @date 2015/Apr/06
     * @param none.
     * @return iterator cend() of data Vector.
     * @brief Returns the iterator cend() of data Vector.
     * @warning none.
     */
    typename std::vector< D >::const_iterator cend( ) const noexcept;

    /**
     * @date 2015/Apr/06
     * @param none.
     * @return iterator crbegin() of data Vector.
     * @brief Returns the iterator crbegin() of data Vector.
     * @warning none.
     */
    typename std::vector< D >::const_reverse_iterator crbegin( ) const noexcept;

    /**
     * @date 2015/Apr/06
     * @param none.
     * @return iterator crend() of data Vector.
     * @brief Returns the iterator crend() of data Vector.
     * @warning none.
     */
    typename std::vector< D >::const_reverse_iterator crend( ) const noexcept;

    /**
     * @date 2012/Sep/11
     * @param none.
     * @return A reference to data.
     * @brief Returns a reference to data.
     * @warning none.
     */
    const D* Data( ) const;

    /**
     * @date 2015/Feb/25
     * @param none.
     * @return A reference to data.
     * @brief Returns a reference to data.
     * @warning none.
     */
    D* Data( );

    /**
     * @date 2015/Feb/25
     * @param none.
     * @return A reference to data.
     * @brief Returns a reference to data.
     * @warning none.
     */
    const D* data( ) const;

    /**
     * @date 2015/Feb/25
     * @param none.
     * @return A reference to data.
     * @brief Returns a reference to data.
     * @warning none.
     */
    D* data( );

    /**
     * @date 2012/Jun/21
     * @param none.
     * @return the number of elements of data Vector.
     * @brief Returns size of data Vector.
     * @warning none.
     */
    size_t Size( ) const;

    /**
     * @date 2012/Jun/21
     * @param none.
     * @return the number of elements of data Vector.
     * @brief Returns size of data Vector. Same as Size( ), just for compatibility with std containers.
     * @warning none.
     */
    size_t size( ) const;

    /**
     * @date 2012/Jul/17
     * @param none.
     * @return The number of elements of the matrix.
     * @brief Returns the number of elements of the matrix. Same as Size( ) method.
     * @warning none.
     */
    size_t Elements( ) const;

    /**
     * @date 2012/Jul/17
     * @param min: Minimum value in interval.
     * @param max: Maximum value in interval.
     * @return the number of elements counted inside the interval limited by min and max.
     * @brief Returns the number of elements counted inside the interval limited by min and max.
     * @warning none.
     */
    size_t Elements( D min, D max ) const;

    /**
     * @date 2013/Aug/27
     * @param position: Dimension index.
     * @param dimension: A matrix dimension.
     * @return The coordinate of position in given dimension.
     * @brief Returns the coordinate of position in given dimension.
     * @warning No position or dimension validation are made.
     */
    size_t Coordinate( size_t position, size_t dimension ) const;

    /**
     * @date 2012/Oct/03
     * @param index: element position.
     * @return The coordinates of the input position in this matrix.
     * @brief Returns the coordinates of the input position in this matrix.
     * @warning none.
     */
    Vector< size_t > Coordinates( size_t position ) const;

    /**
     * @date 2013/Oct/30
     * @param p_0: dim 0 coordinate.
     * @param p_1: dim 1 coordinate.
     * @return The position corresponding to the input coordinates in this matrix.
     * @brief Returns the position corresponding to the input coordinates in this matrix.
     * @warning none.
     */
    size_t Position( size_t p_0, size_t p_1 ) const;

    /**
     * @date 2013/Oct/30
     * @param p_0: dim 0 coordinate.
     * @param p_1: dim 1 coordinate.
     * @param p_2: dim 2 coordinate.
     * @return The position corresponding to the input coordinates in this matrix.
     * @brief Returns the position corresponding to the input coordinates in this matrix.
     * @warning none.
     */
    size_t Position( size_t p_0, size_t p_1, size_t p_2 ) const;

    /**
     * @date 2013/Oct/30
     * @param p_0: dim 0 coordinate.
     * @param p_1: dim 1 coordinate.
     * @param p_2: dim 2 coordinate.
     * @param p_3: dim 3 coordinate.
     * @return The position corresponding to the input coordinates in this matrix.
     * @brief Returns the position corresponding to the input coordinates in this matrix.
     * @warning none.
     */
    size_t Position( size_t p_0, size_t p_1, size_t p_2, size_t p_3 ) const;

    /**
     * @date 2012/Oct/03
     * @param coordinates: element coordinates.
     * @return The position corresponding to the input coordinates in this matrix.
     * @brief Returns the position corresponding to the input coordinates in this matrix.
     * @warning none.
     */
    size_t Position( const Vector< size_t > &coordinates ) const;

    /**
     * @date 2015/Jul/07
     * @param coordinates: element coordinates.
     * @return The position corresponding to the input coordinates in this matrix.
     * @brief Returns the position corresponding to the input coordinates in this matrix.
     * @warning none.
     */
    size_t Position( const Vector< int > &coordinates ) const;

    /**
     * @date 2015/Jul/07
     * @param coordinates: element coordinates.
     * @return The position corresponding to the input coordinates in this matrix.
     * @brief Returns the position corresponding to the input coordinates in this matrix.
     * @warning none.
     */
    size_t Position( const Vector< float > &coordinates ) const;

    /**
     * @date 2012/Jun/29
     * @param val: value to be set to the entire matrix.
     * @return none.
     * @brief Sets val to each element of the matrix.
     * @warning none.
     */
    void Set( D val );

    /**
     * @date 2012/Jun/21
     * @param none.
     * @return A Vector with matrix dimensions.
     * @brief Returns a Vector with all matrix dimensions.
     * @warning It is a copy of the dimension Vector.
     */
    Vector< size_t > Dim( ) const;

    /**
     * @date 2014/Feb/07
     * @param none.
     * @return Minimum size among all matrix dimensions.
     * @brief Returns the minimum size among all matrix dimensions.
     * @warning none.
     */
    size_t MinDim( ) const;

    /**
     * @date 2014/Feb/07
     * @param none.
     * @return Maximum size among all matrix dimensions.
     * @brief Returns the maximum size among all matrix dimensions.
     * @warning none.
     */
    size_t MaxDim( ) const;

    /**
     * @date 2012/Jun/21
     * @param i: The required dimension.
     * @return Matrix required dimension.
     * @brief Returns matrix required dimension.
     * @warning Do not verify if dimension is valid.
     */
    size_t Size( size_t dimension ) const;

    /**
     * @date 2012/Jun/21
     * @param dimension: The required dimension.
     * @return Matrix required dimension.
     * @brief Returns matrix required dimension. Just for symmetry with size( ) and Size( ) functions.
     * @warning Do not verify if dimension is valid.
     */
    size_t size( size_t dimension ) const;

    /**
     * @date 2012/Jun/21
     * @param none.
     * @return The number of dimensions of 'this' matrix.
     * @brief Returns the number of dimensions of 'this' matrix.
     * @warning none.
     */
    size_t Dims( ) const;

    /**
     * @date 2013/Aug/30
     * @param dimension: The required dimension.
     * @return Position displacement of one unit in dimension i.
     * @brief Returns the position displacement of one unit in dimension i.
     * @warning Does not verify if dimension is valid.
     */
    size_t Displacement( size_t dimension ) const;

    /**
     * @date 2012/Jun/21
     * @param p: global index of the data being accessed.
     * @return Constant reverence of value contained in position p of data Vector.
     * @brief Reads the constant reverence of value contained at position p of data Vector.
     * @warning Works for matrices of any dimension. Index p is a global position.
     */
    const D &at( size_t p ) const;

    /**
     * @date 2012/Jun/21
     * @param p: global index of the data being accessed.
     * @return Reference to position p of data Vector.
     * @brief Reads the value contained at position p of data Vector and returns its reference.
     * @warning Works for matrices of any dimension. Index p is a global position.
     */
    D &at( size_t p );

    /**
     * @date 2012/Jun/21
     * @param p_0: first index of the data being accessed.
     * @param p_1: second index of the data being accessed.
     * @return Constant reverence of value contained in position p_0, p_1 of data Vector.
     * @brief Reads the constant reverence of value contained at position p_0, p_1 of data Vector.
     * @warning 2D matrix access.
     */
    const D &at( size_t p_0, size_t p_1 ) const;

    /**
     * @date 2012/Jun/21
     * @param p_0: first index of the data being accessed.
     * @param p_1: second index of the data being accessed.
     * @return Reference to position p_0, p_1 of data Vector.
     * @brief Reads the value contained at position p_0, p_1 of data Vector and returns its reference.
     * @warning 2D matrix access.
     */
    D &at( size_t p_0, size_t p_1 );

    /**
     * @date 2012/Jun/21
     * @param p_0: first index of the data being accessed.
     * @param p_1: second index of the data being accessed.
     * @param p_2: third index of the data being accessed.
     * @return Constant reverence of value contained in position p_0, p_1, p_2 of data Vector.
     * @brief Reads the constant reverence of value contained at position p_0, p_1, p_2 of data Vector.
     * @warning 3D matrix access.
     */
    const D &at( size_t p_0, size_t p_1, size_t p_2 ) const;

    /**
     * @date 2012/Jun/21
     * @param p_0: first index of the data being accessed.
     * @param p_1: second index of the data being accessed.
     * @param p_2: third index of the data being accessed.
     * @return Reference to position p_0, p_1, p_2 of data Vector.
     * @brief Reads the value contained at position p_0, p_1, p_2 of data Vector and returns its
     * reference.
     * @warning 3D matrix access.
     */
    D &at( size_t p_0, size_t p_1, size_t p_2 );

    /**
     * @date 2013/Aug/21
     * @param p_0: first index of the data being accessed.
     * @param p_1: second index of the data being accessed.
     * @param p_2: third index of the data being accessed.
     * @param p_3: fourth index of the data being accessed.
     * @return Constant reverence of value contained in position p_0, p_1, p_2, p_3 of data Vector.
     * @brief Reads the constant reverence of value contained at position p_0, p_1, p_2, p_3 of data
     * Vector.
     * @warning 4D matrix access.
     */
    const D &at( size_t p_0, size_t p_1, size_t p_2, size_t p_3 ) const;

    /**
     * @date 2013/Aug/21
     * @param p_0: first index of the data being accessed.
     * @param p_1: second index of the data being accessed.
     * @param p_2: third index of the data being accessed.
     * @param p_3: fourth index of the data being accessed.
     * @return Reference to position p_0, p_1, p_2, p_3 of data Vector.
     * @brief Reads the value contained at position p_0, p_1, p_2, p_3 of data Vector and returns its
     * reference.
     * @warning 4D matrix access.
     */
    D &at( size_t p_0, size_t p_1, size_t p_2, size_t p_3 );

    /**
     * @date 2014/Apr/14
     * @param p_0: first index of the data being accessed.
     * @param p_1: second index of the data being accessed.
     * @param p_2: third index of the data being accessed.
     * @param p_3: fourth index of the data being accessed.
     * @param p_4: fifth index of the data being accessed.
     * @return Constant reverence of value contained in position p_0, p_1, p_2, p_3, p_4 of data Vector.
     * @brief Reads the constant reverence of value contained at position p_0, p_1, p_2, p_3, p_4 of data
     * Vector.
     * @warning 5D matrix access.
     */
    const D &at( size_t p_0, size_t p_1, size_t p_2, size_t p_3, size_t p_4 ) const;

    /**
     * @date 2014/Apr/14
     * @param p_0: first index of the data being accessed.
     * @param p_1: second index of the data being accessed.
     * @param p_2: third index of the data being accessed.
     * @param p_3: fourth index of the data being accessed.
     * @param p_4: fifth index of the data being accessed.
     * @return Reference to position p_0, p_1, p_2, p_3, p_4 of data Vector.
     * @brief Reads the value contained at position p_0, p_1, p_2, p_3, p_4 of data Vector and returns its
     * reference.
     * @warning 5D matrix access.
     */
    D &at( size_t p_0, size_t p_1, size_t p_2, size_t p_3, size_t p_4 );

    /**
     * @date 2013/Aug/23
     * @param p: Vector of indexes to be acessed.
     * @return Constant reference of data pointed by index Vector p.
     * @brief Returns constant reference of data pointed by index ( p[ 0 ], p[ 1 ], p[ 2 ], ... ). It has the same
     * effect as operator data[ p_0 + p_1 * acc_dim_size[ 0 ] + p_2 * acc_dim_size[ 1 ] ... ].
     * @warning none.
     */
    const D &at( const Vector< size_t > &p ) const;

    /**
     * @date 2013/Aug/23
     * @param p: Vector of indexes to be acessed.
     * @return Constant reference of data pointed by index Vector p.
     * @brief Returns constant reference of data pointed by index ( p[ 0 ], p[ 1 ], p[ 2 ], ... ). It has the same
     * effect as operator data[ p_0 + p_1 * acc_dim_size[ 0 ] + p_2 * acc_dim_size[ 1 ] ... ].
     * @warning none.
     */
    D &at( const Vector< size_t > &p );

    /**
     * @date 2014/Mar/18
     * @param p: Vector of indexes to be acessed.
     * @return True if element is inside matrix domain.
     * @brief Verifies if element is in matrix domain.
     * @warning Does not verify if number of dimensions of p match matrix number of dimensions.
     */
    bool ValidElement( const Vector< size_t > &p ) const;

    /**
     * @date 2014/Apr/23
     * @param min: minimum value of the new amplitude scale
     * @param max: maximum value of the new amplitude scale
     * @return none
     * @brief Recalculate the amplitude scale of the matrix by the following formula:
     *              mat[ pxl ] = min + ( mat[ pxl ] - mat_min ) * ( max - min ) / ( mat_max - mat_min )
     * @warning The PNM format won't support negative values.
     */
    void SetRange( D min, D max );

    /**
     * @date 2012/Jun/21
     * @param none.
     * @return The maximum value among matrix elements.
     * @brief Compute and return the maximum value among matrix elements.
     * @warning none.
     */
    D Maximum( ) const;

    /**
     * @date 2012/Jun/21
     * @param none.
     * @return The minimum value among matrix elements.
     * @brief Compute and return the minimum value among matrix elements.
     * @warning none.
     */
    D Minimum( ) const;

    /**
     * @date 2012/Jun/21
     * @param os: an output stream.
     * @return The output stream.
     * @brief Prints matrix containts to output stream os.
     * @warning 'this' matrix must be 1D, 2D, or 3D.
     */
    template< class O >
    O &Print( O &os ) const;

    /**
     * @date 2012/Jun/21
     * @param none.
     * @return The dimensions of the matrix.
     * @brief Prints matrix dimensions to output stream os.
     * @warning none.
     */
    template< class O >
    O &PrintDimensions( O &os ) const;

    /**
     * @date 2012/Jun/27
     * @param src: Matrix to compare to.
     * @param delta: Maximum difference between any element of
     * @param 'this' and src.
     * @return Weather src is equal or not to 'this' matrix.
     * @brief Compares src matrix to 'this' matrix. They are considere equal only if they have the same dimensions
     * and data.
     * @warning none.
     */
    template< class D2 >
    bool Equals( const Matrix< D2 > &src, double delta ) const;

    /**
     * @date 2012/Jun/27
     * @param src: Matrix to compare to.
     * @return Weather src has the same dimensions as 'this' matrix.
     * @brief Compares the dimensions of src matrix to the dimensions of 'this' matrix.
     * @warning none.
     */
    template< class D2 >
    bool SameDimensions( const Matrix< D2 > &src ) const;

  };
  /**
   * @param os: output stream.
   * @param mat: a matrix.
   * @return Reference to os.
   * @brief Prints the matrix in a friendly way.
   * @warning none.
   */
  //template< typename D > O& operator<<( O& os, const Matrix< D >& mat );
  template< class D, class O >
  O &operator<<( O &os, const Matrix< D > &mat ) {
    mat.Print( os );
    return( os );
  }

}

/* Implementation  ------------------------------------------------------------------------------------------------------ */
#include "Adjacency.hpp"
#include "File.hpp"

namespace Bial {

  template< class D > Matrix< D >::Matrix( ) : _data( ), qk_data( nullptr ), _size( 0 ), dims( 0 ), dim_size( ),
                                               acc_dim_size( ) {
  }
  
  template< class D > Matrix< D >::Matrix( const Vector< size_t > &dim_size ) try
    : _data( ), qk_data( nullptr ), _size( dim_size[ 0 ] ), dims( dim_size.size( ) ), dim_size( dim_size ),
        acc_dim_size( dim_size ) {

    COMMENT( "Computing accumulated size of matrix dimensions and matrix size.", 2 );
    for( size_t dms = 1; dms < dims; ++dms ) {
      acc_dim_size[ dms ] *= acc_dim_size[ dms - 1 ];
      _size *= dim_size[ dms ];
    }
    COMMENT( "Checking for zero size dimension.", 2 );
    if( acc_dim_size[ dims - 1 ] == 0 ) {
      std::string msg( BIAL_ERROR( "Matrix with zero sized dimension(s)." ) );
      throw( std::logic_error( msg ) );
    }
    COMMENT( "Initializing data.", 2 );
    _data = Vector< D >( _size );
    COMMENT( "Assigning quick access pointers.", 2 );
    qk_data = _data.data( );
  }
  catch( std::bad_alloc &e ) {
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
    throw( std::runtime_error( msg ) );
  }
  catch( std::runtime_error &e ) {
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
    throw( std::runtime_error( msg ) );
  }
  catch( const std::out_of_range &e ) {
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
    throw( std::out_of_range( msg ) );
  }
  catch( const std::logic_error &e ) {
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
    throw( std::logic_error( msg ) );
  }

  template< class D > Matrix< D >::Matrix( D *new_data, const Vector< size_t > &new_dim ) try
    : _data( new_data, std::accumulate( new_dim.begin( ), new_dim.end( ), 0 ) ), qk_data( new_data ),
        _size( std::accumulate( new_dim.begin( ), new_dim.end( ), 0 ) ), dims( new_dim.size( ) ),
        dim_size( new_dim ), acc_dim_size( new_dim ) {

      COMMENT( "Computing dimension accumulated size.", 4 );
      for( size_t dms = 1; dms < acc_dim_size.size( ); ++dms )
        acc_dim_size[ dms ] *= acc_dim_size[ dms - 1 ];
    }
  catch( std::bad_alloc &e ) {
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
    throw( std::runtime_error( msg ) );
  }
  catch( std::runtime_error &e ) {
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
    throw( std::runtime_error( msg ) );
  }
  catch( const std::out_of_range &e ) {
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
    throw( std::out_of_range( msg ) );
  }
  catch( const std::logic_error &e ) {
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
    throw( std::logic_error( msg ) );
  }

  template< class D > Matrix< D >::Matrix( size_t size_0, size_t size_1 ) try :
    _data( Vector< D >( size_0 * size_1 ) ), qk_data( nullptr ), _size( size_0 * size_1 ), dims( 2 ), 
      dim_size( 2, size_0 ), acc_dim_size( 2, size_0 ) {

      COMMENT( "Assigning quick access pointers.", 2 );
      qk_data = _data.data( );

      COMMENT( "Setting size of matrix dimensions.", 2 );
      dim_size( 1 ) = size_1;

      COMMENT( "Computing accumulated size of matrix dimensions.", 2 );
      acc_dim_size( 1 ) *= acc_dim_size( 0 );

      COMMENT( "Checking for zero size dimension.", 2 );
      if( acc_dim_size( 1 ) == 0 ) {
        std::string msg( BIAL_ERROR( "Matrix with zero sized dimension(s)." ) );
        throw( std::logic_error( msg ) );
      }
    }
  catch( std::bad_alloc &e ) {
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
    throw( std::runtime_error( msg ) );
  }
  catch( std::runtime_error &e ) {
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
    throw( std::runtime_error( msg ) );
  }
  catch( const std::out_of_range &e ) {
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
    throw( std::out_of_range( msg ) );
  }
  catch( const std::logic_error &e ) {
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
    throw( std::logic_error( msg ) );
  }

  template< class D > Matrix< D >::Matrix( size_t size_0, size_t size_1, size_t size_2 ) try
    : _data( ), qk_data( nullptr ), _size( size_0 * size_1 * size_2 ), dims( 3 ), dim_size( 3, size_0 ),
        acc_dim_size( 3, size_0 ) {
      COMMENT( "Allocating data vector.", 2 );
      _data = Vector< D >( _size );
      COMMENT( "Assigning quick access pointers.", 2 );
      qk_data = _data.data( );
      COMMENT( "Setting size of matrix dimensions.", 2 );
      dim_size( 1 ) = size_1;
      dim_size( 2 ) = size_2;
      COMMENT( "Computing accumulated size of matrix dimensions.", 2 );
      acc_dim_size( 1 ) *= acc_dim_size( 0 );
      acc_dim_size( 2 ) *= acc_dim_size( 1 );
      COMMENT( "Checking for zero size dimension.", 2 );
      if( acc_dim_size( 2 ) == 0 ) {
        std::string msg( BIAL_ERROR( "Matrix with zero sized dimension(s)." ) );
        throw( std::logic_error( msg ) );
      }
    }
  catch( std::bad_alloc &e ) {
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
    throw( std::runtime_error( msg ) );
  }
  catch( std::runtime_error &e ) {
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
    throw( std::runtime_error( msg ) );
  }
  catch( const std::out_of_range &e ) {
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
    throw( std::out_of_range( msg ) );
  }
  catch( const std::logic_error &e ) {
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
    throw( std::logic_error( msg ) );
  }

  template< class D > Matrix< D >::Matrix( const Matrix< D > &mtx ) try
    : _data( mtx._data ), qk_data( nullptr ), _size( mtx._size ), dims( mtx.dims ), dim_size( mtx.dim_size ),
        acc_dim_size( mtx.acc_dim_size ) {

      COMMENT( "Assigning quick access pointers for the matrix.", 2 );
      qk_data = _data.data( );
    }
  catch( std::bad_alloc &e ) {
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
    throw( std::runtime_error( msg ) );
  }
  catch( std::runtime_error &e ) {
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
    throw( std::runtime_error( msg ) );
  }
  catch( const std::out_of_range &e ) {
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
    throw( std::out_of_range( msg ) );
  }
  catch( const std::logic_error &e ) {
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
    throw( std::logic_error( msg ) );
  }

  template< class D >
  template< class D2 > Matrix< D >::Matrix( const Matrix< D2 > &mtx ) try
    : _data( mtx._size ), qk_data( nullptr ), _size( mtx._size ), dims( mtx.dims ), dim_size( mtx.dim_size ),
        acc_dim_size( mtx.acc_dim_size ) {

      COMMENT( "Assigning quick access pointers.", 2 );
      qk_data = _data.data( );

      COMMENT( "Copying data.", 2 );
      for( size_t elm = 0; elm < _size; ++elm )
        QK_DATA( elm ) = static_cast< D >( mtx.QK_DATA( elm ) );
    }
  catch( std::bad_alloc &e ) {
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
    throw( std::runtime_error( msg ) );
  }
  catch( std::runtime_error &e ) {
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
    throw( std::runtime_error( msg ) );
  }
  catch( const std::out_of_range &e ) {
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
    throw( std::out_of_range( msg ) );
  }
  catch( const std::logic_error &e ) {
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
    throw( std::logic_error( msg ) );
  }

  template< class D >
  Matrix< D > &Matrix< D >::operator=( const Matrix< D > &other ) {
    try {
      COMMENT( "Setting data.", 2 );
      _data = other._data;
      _size = other._size;
      dims = other.dims;
      dim_size = other.dim_size;
      acc_dim_size = other.acc_dim_size;

      COMMENT( "Assigning quick access pointers.", 2 );
      qk_data = _data.data( );
      return( *this );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
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

  template< class D >
  template< class D2 >
  Matrix< D > &Matrix< D >::operator=( const Matrix< D2 > &other ) {
    try {
      COMMENT( "Allocating new data of size " << other._size, 2 );
      _data = Vector< D >( other._data.size( ) );
      qk_data = _data.data( );
      _size = other._size;
      dims = other.dims;
      dim_size = other.dim_size;
      acc_dim_size = other.acc_dim_size;

      COMMENT( "Assigning data.", 2 );
      for( size_t elm = 0; elm < _size; ++elm )
        QK_DATA( elm ) = static_cast< D >( other.QK_DATA( elm ) );
      return( *this );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
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

  template< class D >
  template< class D2 >
  bool Matrix< D >::operator==( const Matrix< D2 > &src ) const {
    try {
      if( src.dims == dims ) {
        if( equal( dim_size.begin( ), dim_size.end( ), src.dim_size.begin( ) ) ) {
          if( equal( _data, _data + _size, src._data ) ) {
            return( true );
          }
        }
      }
      return( false );
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

  template< class D >
  Matrix< D > &Matrix< D >::operator++( ) {
    for( size_t elm = 0; elm < _size; ++elm ) {
      ++QK_DATA( elm );
    }
    return( *this );
  }

  template< class D >
  Matrix< D > Matrix< D >::operator++( int ) {
    try {
      Matrix< D > tmp( *this );
      operator++( );
      return( tmp );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
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

  template< class D >
  Matrix< D > &Matrix< D >::operator--( ) {
    try {
      for( size_t elm = 0; elm < _size; ++elm ) {
        --QK_DATA( elm );
      }
      return( *this );
    }
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
  }

  template< class D >
  Matrix< D > Matrix< D >::operator--( int ) {
    try {
      Matrix< D > tmp( *this );
      operator--( );
      return( tmp );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
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

  template< class D >
  template< class D2 >
  Matrix< D > Matrix< D >::operator+( const Matrix< D2 > &other ) const {
    try {
      if( !SameDimensions( other ) ) {
        std::string msg( BIAL_ERROR( "Matrix dimensions do not match." ) );
        throw( std::logic_error( msg ) );
      }
      Matrix< D > res( *this );
      for( size_t elm = 0; elm < res._size; ++elm ) {
        res.QK_DATA( elm ) += other.QK_DATA( elm );
      }
      return( res );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
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

  template< class D >
  template< class D2 >
  Matrix< D > &Matrix< D >::operator+=( const Matrix< D2 > &other ) {
    try {
      *this = *this + other;
      return( *this );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
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

  template< class D >
  Matrix< D > Matrix< D >::operator-( ) const {
    try {
      Matrix< D > res( *this );
      for( size_t elm = 0; elm < res._size; ++elm ) {
        res.QK_DATA( elm ) = -res.QK_DATA( elm );
      }
      return( res );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
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

  template< class D >
  template< class D2 >
  Matrix< D > Matrix< D >::operator-( const Matrix< D2 > &other ) const {
    try {
      if( !SameDimensions( other ) ) {
        std::string msg( BIAL_ERROR( "Matrix dimensions do not match." ) );
        throw( std::logic_error( msg ) );
      }
      Matrix< D > res( *this );
      for( size_t elm = 0; elm < res._size; ++elm ) {
        res.QK_DATA( elm ) -= other.QK_DATA( elm );
      }
      return( res );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
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

  template< class D >
  template< class D2 >
  Matrix< D > &Matrix< D >::operator-=( const Matrix< D2 > &other ) {
    try {
      *this = *this - other;
      return( *this );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
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

  template< class D >
  template< class D2 >
  Matrix< D > Matrix< D >::operator*( const Matrix< D2 > &other ) const {
    try {
      if( ( dims != 2 ) || ( other.dims != 2 ) ) {
        std::string msg( BIAL_ERROR( "Input matrices must be two dimensional." ) );
        throw( std::logic_error( msg ) );
      }
      if( dim_size( 0 ) != other.dim_size( 1 ) ) {
        std::string msg( BIAL_ERROR(
            "Number of columns of left matrix must match the number of lines of right matrix'." ) );
        throw( std::logic_error( msg ) );
      }
      Matrix< D > res( other.dim_size( 0 ), dim_size( 1 ) );
      for( size_t j = 0; j < res.dim_size( 1 ); ++j ) {
        for( size_t i = 0; i < res.dim_size( 0 ); ++i ) {
          res.QK_DATA( j * res.dim_size( 0 ) + i ) = 0.0;
          for( size_t k = 0; k < dim_size( 0 ); ++k ) {
            res.QK_DATA( j * res.dim_size( 0 ) + i ) += QK_DATA( j * dim_size( 0 ) + k ) *
        other.QK_DATA( k * res.dim_size( 0 ) + i );
          }
        }
      }
      return( res );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
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

  template< class D >
  template< class D2 >
  Matrix< D > &Matrix< D >::operator*=( const Matrix< D2 > &other ) {
    try {
      *this = *this * other;
      return( *this );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
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

  template< class D >
  template< class D2 >
  Matrix< D > Matrix< D >::operator/( const Matrix< D2 > &other ) const {
    try {
      if( ( other.dims != 2 ) || ( dims != 2 ) ) {
        std::string msg( BIAL_ERROR( "Input matrices must be two dimensional." ) );
        throw( std::logic_error( msg ) );
      }
      if( other.dim_size( 0 ) != other.dim_size( 1 ) ) {
        std::string msg( BIAL_ERROR( "Right matrix must be square." ) );
        throw( std::logic_error( msg ) );
      }
      if( dim_size( 0 ) != other.dim_size( 1 ) ) {
        std::string msg( BIAL_ERROR(
            "Number of columns of left matrix must match the number of lines of right matrix'." ) );
        throw( std::logic_error( msg ) );
      }
      Matrix< D > res( dim_size( 0 ), dim_size( 1 ) );
      Matrix< D > inv = other.Inverse( );
      for( size_t i = 0; i < res.dim_size( 1 ); i++ ) {
        for( size_t j = 0; j < res.dim_size( 0 ); j++ ) {
          res.QK_DATA( i * dim_size( 0 ) + j ) = 0.0;
          for( size_t k = 0; k < dim_size( 0 ); k++ ) {
            res.QK_DATA( i * res.dim_size( 0 ) + j ) +=
              QK_DATA( i * dim_size( 0 ) + k ) * inv.QK_DATA( k * res.dim_size( 0 ) + j );
          }
        }
      }
      return( Matrix< D >( res ) );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
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

  template< class D >
  template< class D2 >
  Matrix< D > &Matrix< D >::operator/=( const Matrix< D2 > &other ) {
    try {
      *this = *this / other;
      return( *this );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
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

  template< class D >
  Matrix< D > Matrix< D >::operator+( double scalar ) const {
    try {
      Matrix< D > res( *this );
      for( size_t elm = 0; elm < _size; ++elm ) {
        res.QK_DATA( elm ) += scalar;
      }
      return( res );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
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

  template< class D >
  Matrix< D > &Matrix< D >::operator+=( double scalar ) {
    try {
      *this = *this + scalar;
      return( *this );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
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

  template< class D >
  Matrix< D > Matrix< D >::operator-( double scalar ) const {
    try {
      Matrix< D > res( *this );
      for( size_t elm = 0; elm < _size; ++elm ) {
        res.QK_DATA( elm ) -= scalar;
      }
      return( res );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
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

  template< class D >
  Matrix< D > &Matrix< D >::operator-=( double scalar ) {
    try {
      *this = *this - scalar;
      return( *this );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
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

  template< class D >
  Matrix< D > Matrix< D >::operator*( double scalar ) const {
    try {
      Matrix< D > res( *this );
      for( size_t elm = 0; elm < _size; ++elm ) {
        res.QK_DATA( elm ) *= scalar;
      }
      return( res );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
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

  template< class D >
  Matrix< D > &Matrix< D >::operator*=( double scalar ) {
    try {
      *this = *this * scalar;
      return( *this );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
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

  template< class D >
  Matrix< D > Matrix< D >::operator/( double scalar ) const {
    try {
      COMMENT( "Verifying if scalar is not zero.", 2 );
      if( scalar == 0.0 ) {
        std::string msg( BIAL_ERROR( "Division by zero." ) );
        throw( std::logic_error( msg ) );
      }
      Matrix< D > res( *this );
      for( size_t elm = 0; elm < _size; ++elm ) {
        res.QK_DATA( elm ) /= scalar;
      }
      return( res );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
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

  template< class D >
  Matrix< D > &Matrix< D >::operator/=( double scalar ) {
    try {
      *this = *this / scalar;
      return( *this );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
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


  /* Using macros here to speed up code in run mode and allow debug mode. */

  template< class D >
  const D &Matrix< D >::operator[]( size_t p ) const {
    return( QK_DATA( p ) );
  }


  template< class D >
  D &Matrix< D >::operator[]( size_t p ) {
    return( QK_DATA( p ) );
  }

  template< class D >
  const D &Matrix< D >::operator()( size_t p ) const {
    return( QK_DATA( p ) );
  }

  template< class D >
  D &Matrix< D >::operator()( size_t p ) {
    return( QK_DATA( p ) );
  }

  template< class D >
  const D &Matrix< D >::operator()( size_t p_0, size_t p_1 ) const {
    return( QK_DATA( p_0 + dim_size( 0 ) * p_1 ) );
  }

  template< class D >
  D &Matrix< D >::operator()( size_t p_0, size_t p_1 ) {
    return( QK_DATA( p_0 + dim_size( 0 ) * p_1 ) );
  }

  template< class D >
  const D &Matrix< D >::operator()( size_t p_0, size_t p_1, size_t p_2 ) const {
    return( QK_DATA( p_0 + dim_size( 0 ) * p_1 + acc_dim_size( 1 ) * p_2 ) );
  }

  template< class D >
  D &Matrix< D >::operator()( size_t p_0, size_t p_1, size_t p_2 ) {
    return( QK_DATA( p_0 + dim_size( 0 ) * p_1 + acc_dim_size( 1 ) * p_2 ) );
  }

  template< class D >
  const D &Matrix< D >::operator()( size_t p_0, size_t p_1, size_t p_2, size_t p_3 ) const {
    return( QK_DATA( p_0 + dim_size( 0 ) * p_1 + acc_dim_size( 1 ) * p_2 + acc_dim_size( 2 ) * p_3 ) );
  }

  template< class D >
  D &Matrix< D >::operator()( size_t p_0, size_t p_1, size_t p_2, size_t p_3 ) {
    return( QK_DATA( p_0 + dim_size( 0 ) * p_1 + acc_dim_size( 1 ) * p_2 + acc_dim_size( 2 ) * p_3 ) );
  }

  template< class D >
  const D &Matrix< D >::operator()( size_t p_0, size_t p_1, size_t p_2, size_t p_3, size_t p_4 ) const {
    return( QK_DATA( p_0 + dim_size( 0 ) * p_1 + acc_dim_size( 1 ) * p_2 + acc_dim_size( 2 ) * p_3
                     + acc_dim_size( 3 ) * p_4 ) );
  }

  template< class D >
  D &Matrix< D >::operator()( size_t p_0, size_t p_1, size_t p_2, size_t p_3, size_t p_4 ) {
    return( QK_DATA( p_0 + dim_size( 0 ) * p_1 + acc_dim_size( 1 ) * p_2 + acc_dim_size( 2 ) * p_3
                     + acc_dim_size( 3 ) * p_4 ) );
  }

  template< class D >
  const D &Matrix< D >::operator()( const Vector< size_t > &p ) const {
    try {
      size_t index = 0;

      COMMENT( "Verifying if p is non-empty vector.", 4 );
      if( p.size( ) > 0 ) {
        index += p( 0 );
      }
      COMMENT( "Adding dimensions greater than zero to global index.", 4 );
      for( size_t dms = 1; dms < p.size( ); ++dms ) {
        index += p( dms ) * acc_dim_size( dms - 1 );
      }
      return( QK_DATA( index ) );
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

  template< class D >
  D &Matrix< D >::operator()( const Vector< size_t > &p ) {
    try {
      size_t index = 0;

      COMMENT( "Verifying if p is non-empty vector.", 4 );
      if( p.size( ) > 0 ) {
        index += p( 0 );
      }
      COMMENT( "Adding dimensions greater than zero to global index.", 4 );
      for( size_t dms = 1; dms < p.size( ); ++dms ) {
        index += p( dms ) * acc_dim_size( dms - 1 );
      }
      return( QK_DATA( index ) );
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

  template< class D >
  void Matrix< D >::Abs( ) {
    try {
      for( size_t elm = 0; elm < _size; ++elm ) {
        QK_DATA( elm ) = std::abs( QK_DATA( elm ) );
      }
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

  template< class D >
  Matrix< D > Matrix< D >::Abs( ) const {
    try {
      return( Matrix< D >( *this ).Abs( ) );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
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

  template< class D >
  typename std::vector< D >::iterator Matrix< D >::begin( ) noexcept {
    COMMENT( "size_of D: " << sizeof( D ), 4 );
    return( _data.begin( ) );
  }

  template< class D >
  typename std::vector< D >::const_iterator Matrix< D >::begin( ) const noexcept {
    COMMENT( "size_of D: " << sizeof( D ), 4 );
    return( _data.begin( ) );
  }

  template< class D >
  typename std::vector< D >::iterator Matrix< D >::end( ) noexcept {
    COMMENT( "size_of D: " << sizeof( D ), 4 );
    return( _data.end( ) );
  }

  template< class D >
  typename std::vector< D >::const_iterator Matrix< D >::end( ) const noexcept {
    COMMENT( "size_of D: " << sizeof( D ), 4 );
    return( _data.end( ) );
  }

  template< class D >
  typename std::vector< D >::reverse_iterator Matrix< D >::rbegin( ) noexcept {
    return( _data.rbegin( ) );
  }

  template< class D >
  typename std::vector< D >::const_reverse_iterator Matrix< D >::rbegin( ) const noexcept {
    return( _data.rbegin( ) );
  }

  template< class D >
  typename std::vector< D >::reverse_iterator Matrix< D >::rend( ) noexcept {
    return( _data.rend( ) );
  }

  template< class D >
  typename std::vector< D >::const_reverse_iterator Matrix< D >::rend( ) const noexcept {
    return( _data.rend( ) );
  }

  template< class D >
  typename std::vector< D >::const_iterator Matrix< D >::cbegin( ) const noexcept {
    return( _data.cbegin( ) );
  }

  template< class D >
  typename std::vector< D >::const_iterator Matrix< D >::cend( ) const noexcept {
    return( _data.cend( ) );
  }

  template< class D >
  typename std::vector< D >::const_reverse_iterator Matrix< D >::crbegin( ) const noexcept {
    return( _data.crbegin( ) );
  }

  template< class D >
  typename std::vector< D >::const_reverse_iterator Matrix< D >::crend( ) const noexcept {
    return( _data.crend( ) );
  }

  template< class D >
  const D*Matrix< D >::Data( ) const {
    return( qk_data );
  }

  template< class D >
  D*Matrix< D >::Data( ) {
    return( qk_data );
  }

  template< class D >
  const D*Matrix< D >::data( ) const {
    return( qk_data );
  }

  template< class D >
  D*Matrix< D >::data( ) {
    return( qk_data );
  }

  template< class D >
  size_t Matrix< D >::size( ) const {
    return( _size );
  }

  template< class D >
  size_t Matrix< D >::Size( ) const {
    return( _size );
  }

  template< class D >
  size_t Matrix< D >::Elements( ) const {
    return( _size );
  }

  template< class D >
  size_t Matrix< D >::Elements( D min, D max ) const {
    COMMENT( "Counting the number of elements in matrix within interval [min, max].", 3 );
    size_t count = 0;
    for( size_t elm = 0; elm < _size; ++elm ) {
      if( ( QK_DATA( elm ) >= min ) && ( QK_DATA( elm ) <= max ) ) {
        ++count;
      }
    }
    return( count );
  }

  template< class D >
  size_t Matrix< D >::Coordinate( size_t position, size_t dimension ) const {
    for( size_t dms = dims - 1; dms > 0; --dms ) {
      if( dms == dimension ) {
        return( position / acc_dim_size( dms - 1 ) );
      }
      position %= acc_dim_size( dms - 1 );
    }
    return( position );
  }

  template< class D >
  Vector< size_t > Matrix< D >::Coordinates( size_t position ) const {
    Vector< size_t > index( dims, 0 );
    for( size_t dms = dims - 1; dms > 0; --dms ) {
      index[ dms ] = position / acc_dim_size( dms - 1 );
      position %= acc_dim_size( dms - 1 );
    }
    index[ 0 ] = position;
    return( index );
  }

  template< class D >
  size_t Matrix< D >::Position( size_t p_0, size_t p_1 ) const {
    return( p_0 + p_1 * dim_size( 0 ) );
  }

  template< class D >
  size_t Matrix< D >::Position( size_t p_0, size_t p_1, size_t p_2 ) const {
    return( p_0 + p_1 * dim_size( 0 ) + p_2 * acc_dim_size( 1 ) );
  }

  template< class D >
  size_t Matrix< D >::Position( size_t p_0, size_t p_1, size_t p_2, size_t p_3 ) const {
    return( p_0 + p_1 * dim_size( 0 ) + p_2 * acc_dim_size( 1 ) + p_3 * acc_dim_size( 2 ) );
  }

  template< class D >
  size_t Matrix< D >::Position( const Vector< size_t > &coordinates ) const {
    IF_DEBUG( coordinates.size( ) == 0 ) {
      std::string msg( BIAL_ERROR( "Matrix with zero sized dimension(s)." ) );
      throw( std::logic_error( msg ) );
    }
    size_t position = coordinates[ 0 ];
    for( size_t dms = 1; dms < dims; ++dms )
      position += acc_dim_size( dms - 1 ) * coordinates[ dms ];
    return( position );
  }

  template< class D >
  size_t Matrix< D >::Position( const Vector< int > &coordinates ) const {
    IF_DEBUG( coordinates.size( ) == 0 ) {
      std::string msg( BIAL_ERROR( "Matrix with zero sized dimension(s)." ) );
      throw( std::logic_error( msg ) );
    }
    size_t position = static_cast< size_t >( coordinates[ 0 ] );
    for( size_t dms = 1; dms < dims; ++dms )
      position += acc_dim_size( dms - 1 ) * static_cast< int >( coordinates[ dms ] );
    return( position );
  }

  template< class D >
  size_t Matrix< D >::Position( const Vector< float > &coordinates ) const {
    IF_DEBUG( coordinates.size( ) == 0 ) {
      std::string msg( BIAL_ERROR( "Matrix with zero sized dimension(s)." ) );
      throw( std::logic_error( msg ) );
    }
    size_t position = static_cast< size_t >( coordinates[ 0 ] );
    for( size_t dms = 1; dms < dims; ++dms )
      position += acc_dim_size( dms - 1 ) * static_cast< size_t >( coordinates[ dms ] );
    return( position );
  }

  template< class D >
  void Matrix< D >::Set( D val ) {
    try {
      size_t i;
      for( i = 0; i < _size; i++ ) {
        QK_DATA( i ) = val;
      }
    }
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
  }

  template< class D >
  Vector< size_t > Matrix< D >::Dim( ) const {
    return( dim_size );
  }

  template< class D >
  size_t Matrix< D >::MinDim( ) const {
    size_t min_dim = dim_size( 0 );
    for( size_t dms = 1; dms < dims; ++dms ) {
      if( min_dim > dim_size( dms ) ) {
        min_dim = dim_size( dms );
      }
    }
    return( min_dim );
  }

  template< class D >
  size_t Matrix< D >::MaxDim( ) const {
    size_t max_dim = dim_size( 0 );
    for( size_t dms = 1; dms < dims; ++dms ) {
      if( max_dim < dim_size( dms ) ) {
        max_dim = dim_size( dms );
      }
    }
    return( max_dim );
  }

  template< class D >
  size_t Matrix< D >::Size( size_t dimension ) const {
    return( dim_size( dimension ) );
  }

  template< class D >
  size_t Matrix< D >::size( size_t dimension ) const {
    return( dim_size( dimension ) );
  }

  template< class D >
  size_t Matrix< D >::Dims( ) const {
    return( dims );
  }

  template< class D >
  size_t Matrix< D >::Displacement( size_t dimension ) const {
    return( acc_dim_size( dimension ) );
  }

  template< class D >
  const D &Matrix< D >::at( size_t p ) const {
    if( p >= _size ) {
      std::string msg( BIAL_ERROR( "Index out of range. Given: " + std::to_string( p ) + ", expected: < " +
                                   std::to_string( _size ) ) );
      throw( std::out_of_range( msg ) );
    }
    return( qk_data[ p ] );
  }

  template< class D >
  D &Matrix< D >::at( size_t p ) {
    if( p >= _size ) {
      std::string msg( BIAL_ERROR( "Index out of range. Given: " + std::to_string( p ) + ", expected: < " +
                                   std::to_string( _size ) ) );
      throw( std::out_of_range( msg ) );
    }
    return( qk_data[ p ] );
  }

  template< class D >
  const D &Matrix< D >::at( size_t p_0, size_t p_1 ) const {
    if( dims != 2 ) {
      std::string msg( BIAL_ERROR( "Invalid number of dimensions. Expected 2." ) );
      throw( std::logic_error( msg ) );
    }
    if( ( p_0 >= dim_size( 0 ) ) || ( p_1 >= dim_size( 1 ) ) ) {
      std::string msg( BIAL_ERROR( "Index out of range." ) );
      throw( std::out_of_range( msg ) );
    }
    return( qk_data[ p_0 + p_1 * dim_size( 0 ) ] );
  }

  template< class D >
  D &Matrix< D >::at( size_t p_0, size_t p_1 ) {
    if( dims != 2 ) {
      std::string msg( BIAL_ERROR( "Invalid number of dimensions. Expected 2." ) );
      throw( std::logic_error( msg ) );
    }
    if( ( p_0 >= dim_size( 0 ) ) || ( p_1 >= dim_size( 1 ) ) ) {
      std::string msg( BIAL_ERROR( "Index out of range." ) );
      throw( std::out_of_range( msg ) );
    }
    return( qk_data[ p_0 + p_1 * dim_size( 0 ) ] );
  }

  template< class D >
  const D &Matrix< D >::at( size_t p_0, size_t p_1, size_t p_2 ) const {
    if( dims != 3 ) {
      std::string msg( BIAL_ERROR( "Invalid number of dimensions. Expected 3." ) );
      throw( std::logic_error( msg ) );
    }
    if( ( p_0 >= dim_size( 0 ) ) || ( p_1 >= dim_size( 1 ) ) || ( p_2 >= dim_size( 2 ) ) ) {
      std::string msg( BIAL_ERROR( "Index out of range." ) );
      throw( std::out_of_range( msg ) );
    }
    return( qk_data[ p_0 + p_1 * dim_size( 0 ) + p_2 * acc_dim_size( 1 ) ] );
  }

  template< class D >
  D &Matrix< D >::at( size_t p_0, size_t p_1, size_t p_2 ) {
    if( dims != 3 ) {
      std::string msg( BIAL_ERROR( "Invalid number of dimensions. Expected 3." ) );
      throw( std::logic_error( msg ) );
    }
    if( ( p_0 >= dim_size( 0 ) ) || ( p_1 >= dim_size( 1 ) ) || ( p_2 >= dim_size( 2 ) ) ) {
      std::string msg( BIAL_ERROR( "Index out of range." ) );
      throw( std::out_of_range( msg ) );
    }
    return( qk_data[ p_0 + p_1 * dim_size( 0 ) + p_2 * acc_dim_size( 1 ) ] );
  }

  template< class D >
  const D &Matrix< D >::at( size_t p_0, size_t p_1, size_t p_2, size_t p_3 ) const {
    if( dims != 4 ) {
      std::string msg( BIAL_ERROR( "Invalid number of dimensions. Expected 4." ) );
      throw( std::logic_error( msg ) );
    }
    if( ( p_0 >= dim_size( 0 ) ) || ( p_1 >= dim_size( 1 ) ) || ( p_2 >= dim_size( 2 ) ) ||
        ( p_3 >= dim_size( 3 ) ) ) {
      std::string msg( BIAL_ERROR( "Index out of range." ) );
      throw( std::out_of_range( msg ) );
    }
    return( qk_data[ p_0 + p_1 * dim_size( 0 ) + p_2 * acc_dim_size( 1 ) + p_3 * acc_dim_size( 2 ) ] );
  }

  template< class D >
  D &Matrix< D >::at( size_t p_0, size_t p_1, size_t p_2, size_t p_3 ) {
    if( dims != 4 ) {
      std::string msg( BIAL_ERROR( "Invalid number of dimensions. Expected 4." ) );
      throw( std::logic_error( msg ) );
    }
    if( ( p_0 >= dim_size( 0 ) ) || ( p_1 >= dim_size( 1 ) ) || ( p_2 >= dim_size( 2 ) ) ||
        ( p_3 >= dim_size( 3 ) ) ) {
      std::string msg( BIAL_ERROR( "Index out of range." ) );
      throw( std::out_of_range( msg ) );
    }
    return( qk_data[ p_0 + p_1 * dim_size( 0 ) + p_2 * acc_dim_size( 1 ) + p_3 * acc_dim_size( 2 ) ] );
  }

  template< class D >
  const D &Matrix< D >::at( size_t p_0, size_t p_1, size_t p_2, size_t p_3, size_t p_4 ) const {
    if( dims != 5 ) {
      std::string msg( BIAL_ERROR( "Invalid number of dimensions. Expected 5." ) );
      throw( std::logic_error( msg ) );
    }
    if( ( p_0 >= dim_size( 0 ) ) || ( p_1 >= dim_size( 1 ) ) || ( p_2 >= dim_size( 2 ) ) || ( p_3 >= dim_size( 3 ) ) ||
        ( p_4 >= dim_size( 4 ) ) ) {
      std::string msg( BIAL_ERROR( "Index out of range." ) );
      throw( std::out_of_range( msg ) );
    }
    return( qk_data[ p_0 + p_1 * dim_size( 0 ) + p_2 * acc_dim_size( 1 ) +
                     p_3 * acc_dim_size( 2 ) + p_4 * acc_dim_size( 3 ) ] );
  }

  template< class D >
  D &Matrix< D >::at( size_t p_0, size_t p_1, size_t p_2, size_t p_3, size_t p_4 ) {
    if( dims != 5 ) {
      std::string msg( BIAL_ERROR( "Invalid number of dimensions. Expected 5." ) );
      throw( std::logic_error( msg ) );
    }
    if( ( p_0 >= dim_size( 0 ) ) || ( p_1 >= dim_size( 1 ) ) || ( p_2 >= dim_size( 2 ) ) || ( p_3 >= dim_size( 3 ) ) ||
        ( p_4 >= dim_size( 4 ) ) ) {
      std::string msg( BIAL_ERROR( "Index out of range." ) );
      throw( std::out_of_range( msg ) );
    }
    return( qk_data[ p_0 + p_1 * dim_size( 0 ) + p_2 * acc_dim_size( 1 ) +
                     p_3 * acc_dim_size( 2 ) + p_4 * acc_dim_size( 3 ) ] );
  }


  template< class D >
  const D &Matrix< D >::at( const Vector< size_t > &p ) const {
    if( dims != p.size( ) ) {
      std::string msg( BIAL_ERROR( "Invalid number of dimensions. Expected " + std::to_string( p.size( ) ) + "." ) );
      throw( std::logic_error( msg ) );
    }
    for( size_t dms = 0; dms < p.size( ); ++dms ) {
      if( p[ dms ] >= dim_size( dms ) ) {
        std::string msg( BIAL_ERROR( "Index out of range." ) );
        throw( std::out_of_range( msg ) );
      }
    }
    return( *this( p ) );
  }

  template< class D >
  D &Matrix< D >::at( const Vector< size_t > &p ) {
    if( dims != p.size( ) ) {
      std::string msg( BIAL_ERROR( "Invalid number of dimensions. Expected " + std::to_string( p.size( ) ) + "." ) );
      throw( std::logic_error( msg ) );
    }
    for( size_t dms = 0; dms < p.size( ); ++dms ) {
      if( p[ dms ] >= dim_size( dms ) ) {
        std::string msg( BIAL_ERROR( "Index out of range." ) );
        throw( std::out_of_range( msg ) );
      }
    }
    return( *this( p ) );
  }

  template< class D >
  bool Matrix< D >::ValidElement( const Vector< size_t > &p ) const {
    for( size_t dms = 0; dms < p.size( ); ++dms ) {
      if( p[ dms ] >= dim_size( dms ) ) {
        return( false );
      }
    }
    return( true );
  }

  template< class D >
  void Matrix< D >::SetRange( D min, D max ) {
    try {
      COMMENT( "Verifying if minimum is lower or equal to maximum.", 0 );
      if( min >= max ) {
        std::string msg( BIAL_ERROR( "max >= min. Given min: " + std::to_string( min ) + ", max: " + std::to_string(
                                max ) ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Getting matrix minimum and maximum intensities.", 2 );
      D mat_max = Maximum( );
      D mat_min = Minimum( );
      if( ( mat_max == max ) && ( mat_min == min ) ) {
        COMMENT( "If matrix is already in given range, do not change!", 2 );
        return;
      }
      if( mat_max - mat_min == static_cast< D >( 0.0 ) ) {
        COMMENT( "If matrix is flat, set it to min.", 2 );
        Set( min );
        return;
      }
      if( static_cast< D >( 1.1 ) == static_cast< D >( 1.0 ) ) {
        COMMENT( "Setting new range for integer matrices.", 2 );
        for( size_t elm = 0; elm < _size; ++elm ) {
          ( *this )[ elm ] = static_cast< D >( std::round( min + ( static_cast< double >( ( *this )[ elm ] - mat_min ) )
                                                           * ( max - min ) / ( mat_max - mat_min ) ) );
        }
      }
      else {
        COMMENT( "Setting new range for float point matrices.", 2 );
        for( size_t elm = 0; elm < _size; ++elm ) {
          ( *this )[ elm ] = static_cast< D >( min + ( static_cast< double >( ( *this )[ elm ] - mat_min ) )
                                               * ( max - min ) / ( mat_max - mat_min ) );
        }
      }
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

  template< class D >
  D Matrix< D >::Maximum( ) const {
    return( *std::max_element( qk_data, qk_data + _size ) );
  }

  template< class D >
  D Matrix< D >::Minimum( ) const {
    return( *std::min_element( qk_data, qk_data + _size ) );
  }

  template< class D >
  template< class O >
  O &Matrix< D >::Print( O &os ) const {
    try {
      if( ( dims < 1 ) || ( dims > 3 ) ) {
        std::string msg( BIAL_ERROR( "Invalid number of dimensions. Expected 1, 2, or 3." ) );
        throw( std::logic_error( msg ) );
      }
      if( dims == 1 ) {
        for( size_t i = 0; i < dim_size( 0 ); ++i ) {
          os << QK_DATA( i );
          if( i != dim_size( 0 ) - 1 ) {
            os << " ";
          }
        }
      }
      else if( dims == 2 ) {
        for( size_t i = 0; i < dim_size( 1 ); ++i ) {
          for( size_t j = 0; j < dim_size( 0 ); ++j ) {
            os << QK_DATA( j + i * dim_size( 0 ) );
            if( j != dim_size( 0 ) - 1 ) {
              os << " ";
            }
          }
          if( i != dim_size( 1 ) - 1 ) {
            os << std::endl;
          }
        }
      }
      else { /* dims == 3 */
        for( size_t i = 0; i < dim_size( 2 ); ++i ) {
          if( i > 0 ) {
            os << std::endl;
          }
          os << "( , , " << i << "):" << std::endl;
          for( size_t j = 0; j < dim_size( 1 ); ++j ) {
            for( size_t k = 0; k < dim_size( 0 ); ++k ) {
              os << QK_DATA( k + j * dim_size( 0 ) + i * dim_size( 1 ) * dim_size( 0 ) );
              if( k != dim_size( 0 ) - 1 ) {
                os << " ";
              }
            }
            if( j != dim_size( 1 ) - 1 ) {
              os << std::endl;
            }
          }
        }
      }
      return( os );
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

  template< class D >
  template< class O >
  O &Matrix< D >::PrintDimensions( O &os ) const {
    try {
      for( size_t dms = 0; dms < dims; ++dms ) {
        os << dim_size( dms );
        if( dms != dims - 1 ) {
          os << " ";
        }
      }
      return( os );
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

  template< class D >
  template< class D2 >
  bool Matrix< D >::Equals( const Matrix< D2 > &src, double delta ) const {
    try {
      if( src.dim_size.size( ) == dim_size.size( ) ) {
        if( equal( dim_size.begin( ), dim_size.end( ), src.dim_size.begin( ) ) ) {
          for( size_t i = 0; i < _size; i++ ) {
            if( ( QK_DATA( i ) + delta < src.QK_DATA( i ) ) || ( QK_DATA( i ) - delta > src.QK_DATA( i ) ) ) {
              return( false );
            }
          }
          return( true );
        }
      }
      return( false );
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

  template< class D >
  template< class D2 >
  bool Matrix< D >::SameDimensions( const Matrix< D2 > &src ) const {
    if( src.dims == dims ) {
      return( equal( dim_size.begin( ), dim_size.end( ), src.dim_size.begin( ) ) );
    }
    return( false );
  }

}

#endif
