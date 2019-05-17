/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @file Vector.hpp
 * @author Fábio Augusto Menocci Cappabianco
 * @date 2015/Apr/01
 * @version 1.0.00
 * @brief
 * Content: Vector class
 * <br> Description: A vector that calls almost all std::vector functions (C++11 standard).
 * <br> Future add-on's: Things that speedup processing.
 */

#include "Common.hpp"
#ifndef BIALVECTOR_H
#define BIALVECTOR_H

namespace Bial {

  template< class D >
  class Vector {

    template< class D2 >
    friend class Vector;

  protected:

    /**
     * @brief  data vector
     */
    std::vector< D, std::allocator< D > > _data;

    /**
     * @brief  Quick access reference for data.
     */
    D *qk_data;

    /**
     * @brief Size of data. Required here because _data may not exist in case of pointer constructor.
     */
    size_t _size;

  public:

    /**
     * @date 2015/Apr/01
     * @param many.
     * @return none.
     * @brief Basic Constructors.
     * @warning none.
     */
    explicit Vector( );
    explicit Vector( size_t n );
    Vector( size_t n, const D &val );
    template< class InputIterator > Vector( InputIterator first, InputIterator last );
    Vector( const Vector< D > &x );
    Vector( Vector< D > && x );
    Vector( const std::vector< D > &x );
    Vector( std::vector< D > && x );
    Vector( std::initializer_list< D > il );
    template< class D2 > Vector( const Vector< D2 > &src );
    template< class D2 > Vector( const std::vector< D2 > &src );

    /**
     * @date 2015/Set/10
     * @param new_data: Pointer to data allocated elsewhere.
     * @param new_size: Size of data.
     * @return none.
     * @brief Basic Constructor. This constructor works as a wrapper for data already allocated. It will use the 
     * same space pointed by new_data to manipulate the vector. Use this for compatibility with other data structures,
     * in a easy convertion to Vector class.
     * @warning Data is not deallocated automatcally, since it may be stored in the stack. Also, DO NOT USE this 
     * constructure in Verbose or Debug compilation mode, as it will try to access invalid memory positions. Just
     * compile without any Verbosity or Debug flags.
     */
    Vector( D *new_data, size_t new_size );

    /**
     * @date 2015/Apr/01
     * @param many.
     * @return none.
     * @brief Assign operator.
     * @warning none.
     */
    Vector< D > &operator=( const Vector< D > &x );
    Vector< D > &operator=( Vector< D > && x );
    Vector< D > &operator=( std::initializer_list< D > il );
    Vector< D > &operator=( double val );

    /**
     * @date 2015/Apr/01
     * @param many.
     * @return none.
     * @brief Arithmetic operators.
     * @warning none.
     */
    template< class D2 >
    Vector< D > &operator+=( const Vector< D2 > &other );
    template< class D2 >
    Vector< D > &operator-=( const Vector< D2 > &other );
    template< class D2 >
    Vector< D > &operator*=( const Vector< D2 > &other );
    template< class D2 >
    Vector< D > &operator/=( const Vector< D2 > &other );
    template< class D2 >
    Vector< D > operator+( const Vector< D2 > &other ) const;
    template< class D2 >
    Vector< D > operator-( const Vector< D2 > &other ) const;
    template< class D2 >
    Vector< D > operator*( const Vector< D2 > &other ) const;
    template< class D2 >
    Vector< D > operator/( const Vector< D2 > &other ) const;
    Vector< D > &operator+=( const double scalar );
    Vector< D > &operator-=( const double scalar );
    Vector< D > &operator*=( const double scalar );
    Vector< D > &operator/=( const double scalar );
    Vector< D > operator+( const double scalar ) const;
    Vector< D > operator-( const double scalar ) const;
    Vector< D > operator*( const double scalar ) const;
    Vector< D > operator/( const double scalar ) const;

    /**
     * @date 2015/Apr/01
     * @param none.
     * @return Nothing.
     * @brief Computes the absolute value of vector.
     * @warning none.
     */
    void Abs( );

    /**
     * @date 2015/Apr/01
     * @param none.
     * @return Image with absolute values of vector.
     * @brief Computes the absolute value of vector.
     * @warning none.
     */
    Vector< D > Abs( ) const;

    /**
     * @date 2015/Apr/01
     * @param many.
     * @return none.
     * @brief Iterators.
     * @warning none.
     */
    typename std::vector< D >::iterator begin( );
    typename std::vector< D >::const_iterator begin( ) const;
    typename std::vector< D >::iterator end( );
    typename std::vector< D >::const_iterator end( ) const;
    typename std::vector< D >::reverse_iterator rbegin( );
    typename std::vector< D >::const_reverse_iterator rbegin( ) const;
    typename std::vector< D >::reverse_iterator rend( );
    typename std::vector< D >::const_reverse_iterator rend( ) const;
    typename std::vector< D >::const_iterator cbegin( ) const;
    typename std::vector< D >::const_iterator cend( ) const;
    typename std::vector< D >::const_reverse_iterator crbegin( ) const;
    typename std::vector< D >::const_reverse_iterator crend( ) const;

    /**
     * @date 2015/Apr/01
     * @param none.
     * @return Vector size.
     * @brief Returns vector size.
     * @warning none.
     */
    size_t size( ) const noexcept;

    /**
     * @date 2015/Apr/01
     * @param none.
     * @return Vector size.
     * @brief Returns vector size.
     * @warning none.
     */
    size_t Size( ) const noexcept;

    /**
     * @date 2015/Jun/25
     * @param none.
     * @return Vector dimensions.
     * @brief Returns 1. For compatibility with Matrix and Image.
     * @warning none.
     */
    size_t Dims( ) const noexcept;

    /**
     * @date 2015/Apr/01
     * @param none.
     * @return Vector maximum size.
     * @brief Returns maximum vector size.
     * @warning none.
     */
    size_t max_size( ) const noexcept;

    /**
     * @date 2015/Apr/01
     * @param n: new size.
     * @param val: value assigned to new elements.
     * @return none.
     * @brief Resizes vector.
     * @warning none.
     */
    void resize( size_t n );
    void resize( size_t n, const D &val );

    /**
     * @date 2015/Apr/01
     * @param none.
     * @return Vector capacity.
     * @brief Returns vector capacity.
     * @warning none.
     */
    size_t capacity( ) const;

    /**
     * @date 2015/Apr/01
     * @param none.
     * @return true if vector is empty.
     * @brief Tests if vector is empty.
     * @warning none.
     */
    bool empty( ) const noexcept;

    /**
     * @date 2015/Apr/01
     * @param n: new vector capacity.
     * @return none.
     * @brief Changes vector capacity.
     * @warning none.
     */
    void reserve( size_t n );

    /**
     * @date 2015/Apr/01
     * @param none.
     * @return none.
     * @brief Reduces vector capacity to size.
     * @warning none.
     */
    void shrink_to_fit( );

    /**
     * @date 2015/Jun/25
     * @param filename: Name of the file conatining vector.
     * @return The read vector.
     * @brief Reads vector from file.
     * @warning none.
     */
    static Vector< D > Read( const std::string &filename );

    /**
     * @date 2015/Jun/25
     * @param filename: Name of the file conatining vector.
     * @return none.
     * @brief Writes vector into file.
     * @warning none.
     */
    void Write( const std::string &filename ) const;

    /**
     * @date 2015/Apr/01
     * @param n: a vector index.
     * @return Reference to data at position n.
     * @brief Returns reference to data at position n.
     * @warning none.
     */
    D &operator[]( size_t n );
    const D &operator[]( size_t n ) const;

    /**
     * @date 2015/Apr/01
     * @param n: a vector index.
     * @return Reference to data at position n.
     * @brief Returns reference to data at position n.
     * @warning none.
     */
    D &operator()( size_t n );
    const D &operator()( size_t n ) const;

    /**
     * @date 2015/Apr/01
     * @param n: a vector reference.
     * @return Reference to data at position n.
     * @brief Returns reference to data at position n, checking if it exists.
     * @warning none.
     */
    D &at( size_t n );
    const D &at( size_t n ) const;

    /**
     * @date 2015/Apr/01
     * @param none.
     * @return Vector's first element.
     * @brief Returns vector's first element.
     * @warning none.
     */
    D &front( );
    const D &front( ) const;

    /**
     * @date 2015/Apr/01
     * @param none.
     * @return Vector's last element.
     * @brief Returns vector's last element.
     * @warning none.
     */
    D &back( );
    const D &back( ) const;

    /**
     * @date 2015/Apr/01
     * @param none.
     * @return Pointer to _data vector.
     * @brief Returns pointer to _data vector.
     * @warning none.
     */
    D* data( ) noexcept;
    const D* data( ) const noexcept;

    /**
     * @date 2015/Apr/01
     * @param none.
     * @return none.
     * @brief Assigns data to all elements.
     * @warning none.
     */
    template< class InputIterator >
    void assign( InputIterator first, InputIterator last );
    void assign( size_t n, const D &val );
    void assign( std::initializer_list< D > il );

    /**
     * @date 2015/Apr/01
     * @param none.
     * @return none.
     * @brief Assigns data to all elements.
     * @warning none.
     */
    void Set( double val );

    /**
     * @date 2015/Apr/01
     * @param val: The element to be inserted.
     * @return none.
     * @brief Inserts element in the end of the vector.
     * @warning none.
     */
    void push_back( const D &val );

    /**
     * @date 2015/Apr/01
     * @param val: The element to be inserted.
     * @return none.
     * @brief Inserts element in the end of the vector.
     * @warning none.
     */
    void push_back( D && val );

    /**
     * @date 2015/Aug/31
     * @param val: The element to be searched.
     * @return A boolean value.
     * @brief Searchs if the vector contains that value.
     * @warning This operation can be slow.
     */
    bool contains( const D &val );

    /**
     * @date 2015/Aug/31
     * @param val: The element to be searched.
     * @return The found element position, -1 for failed searchs.
     * @brief Searchs the fisrt instance of the given value in the vector.
     * @warning This operation can be slow.
     */
    int first_of( const D &val );

    /**
     * @date 2015/Apr/01
     * @param none.
     * @return none.
     * @brief Removes last element from vector.
     * @warning none.
     */
    void pop_back( );

    /**
     * @date 2015/Apr/01
     * @param many.
     * @return Pointer to the first inserted element.
     * @brief Inserts elements into the vector.
     * @warning none.
     */
    typename std::vector< D >::iterator insert( typename std::vector< D >::iterator position, const D &val );
    typename std::vector< D >::iterator insert( typename std::vector< D >::iterator position, size_t n,
                                                const D &val );
    template< class InputIterator >
    typename std::vector< D >::iterator insert( typename std::vector< D >::iterator position, InputIterator first,
                                                InputIterator last );
    typename std::vector< D >::iterator insert( typename std::vector< D >::iterator position, D && val );
    typename std::vector< D >::iterator insert( typename std::vector< D >::iterator position, 
                                                std::initializer_list< D > il );

    /**
     * @date 2015/Apr/01
     * @param many.
     * @return Pointer to element after the lastest removed.
     * @brief Removes elements from the vector.
     * @warning none.
     */
    typename std::vector< D >::iterator erase( typename std::vector< D >::iterator position );
    typename std::vector< D >::iterator erase( typename std::vector< D >::iterator first, 
                                               typename std::vector< D >::iterator last );

    /**
     * @date 2015/Apr/01
     * @param x: a vector.
     * @return none.
     * @brief Exchanges content of this vector by x.
     * @warning none.
     */
    void swap( Vector &x );

    /**
     * @date 2015/Apr/01
     * @param none.
     * @return none.
     * @brief Removes all elements from vector.
     * @warning none.
     */
    void clear( );

    /**
     * @date 2015/Apr/01
     * @param many.
     * @return Pointer to inserted element.
     * @brief Inserts new element into vector increasing its size.
     * @warning none.
     */
    template< class ... Args >
    typename std::vector< D >::iterator emplace( typename std::vector< D >::const_iterator position, Args && ... args );

    /**
     * @date 2015/Apr/01
     * @param many.
     * @return none.
     * @brief Inserts element into the end of the vector.
     * @warning none.
     */
    template< class ... Args >
    void emplace_back( Args && ... args );

    /**
     * @date 2012/Jun/21
     * @param none.
     * @return The maximum value among vector elements.
     * @brief Compute and return the maximum value among vector elements.
     * @warning none.
     */
    D Maximum( ) const;

    /**
     * @date 2012/Jun/21
     * @param none.
     * @return The minimum value among vector elements.
     * @brief Compute and return the minimum value among vector elements.
     * @warning none.
     */
    D Minimum( ) const;

    /**
     * @date 2012/Jun/27
     * @param src: Vector to compare to.
     * @return Weather src has the same dimensions as 'this' vector.
     * @brief Compares the dimensions of src vector to the dimensions of 'this' vector. For compatibility with Matrix 
     * and Image classes.
     * @warning none.
     */
    template< class D2 >
    bool SameDimensions( const Vector< D2 > &src ) const;

    /**
     * @date 2012/Jun/21
     * @param os: an output stream.
     * @return The output stream.
     * @brief Prints vector containts to output stream os.
     * @warning 'this' vector must be 1D, 2D, or 3D.
     */
    template< class O >
    O &Print( O &os ) const;

    /**
     * @date 2012/Jun/21
     * @param none.
     * @return The dimensions of the vector.
     * @brief Prints vector dimensions to output stream os.
     * @warning none.
     */
    template< class O >
    O &PrintDimensions( O &os ) const;

    /**
     * @date 2019/May/15
     * @param element: An element to be found in the vector.
     * @return True if the element is in the vector.
     * @brief Searches for an element inside the vector.
     * @warning none.
     */
    bool Has( D element ) const;
  };

  template< >
  class Vector< bool > {
  protected:

    std::vector< bool > _data; /* data vector */
    bool qk_data; /* foo variable just to avoid compilation errors. */
    size_t _size; /* Size of data. */

  public:

    /**
     * @date 2015/Apr/02
     * @param many.
     * @return none.
     * @brief Basic Constructors.
     * @warning none.
     */
    explicit Vector( );
    explicit Vector( size_t n );
    Vector( size_t n, const bool &val );
    template< class InputIterator > Vector( InputIterator first, InputIterator last );
    Vector( const Vector< bool > &x );
    Vector( Vector< bool > && x );
    Vector( const std::vector< bool > &x );
    Vector( std::vector< bool > && x );
    Vector( std::initializer_list< bool > il );

    /**
     * @date 2015/Apr/02
     * @param many.
     * @return none.
     * @brief Assign operator.
     * @warning none.
     */
    Vector< bool > &operator=( const Vector< bool > &x );
    Vector< bool > &operator=( Vector< bool > && x );
    Vector< bool > &operator=( std::initializer_list< bool > il );

    /**
     * @date 2015/Apr/01
     * @param many.
     * @return none.
     * @brief Iterators.
     * @warning none.
     */
    typename std::vector< bool >::iterator begin( ) noexcept;
    typename std::vector< bool >::const_iterator begin( ) const noexcept;
    typename std::vector< bool >::iterator end( ) noexcept;
    typename std::vector< bool >::const_iterator end( ) const noexcept;
    typename std::vector< bool >::reverse_iterator rbegin( ) noexcept;
    typename std::vector< bool >::const_reverse_iterator rbegin( ) const noexcept;
    typename std::vector< bool >::reverse_iterator rend( ) noexcept;
    typename std::vector< bool >::const_reverse_iterator rend( ) const noexcept;
    typename std::vector< bool >::const_iterator cbegin( ) const noexcept;
    typename std::vector< bool >::const_iterator cend( ) const noexcept;
    typename std::vector< bool >::const_reverse_iterator crbegin( ) const noexcept;
    typename std::vector< bool >::const_reverse_iterator crend( ) const noexcept;

    /**
     * @date 2015/Apr/01
     * @param none.
     * @return Vector size.
     * @brief Returns vector size.
     * @warning none.
     */
    size_t size( ) const noexcept;
    size_t Size( ) const noexcept;

    /**
     * @date 2015/Jun/25
     * @param none.
     * @return Vector dimensions.
     * @brief Returns 1. For compatibility with Matrix and Image.
     * @warning none.
     */
    size_t Dims( ) const noexcept;

    /**
     * @date 2015/Apr/01
     * @param none.
     * @return Vector maximum size.
     * @brief Returns maximum vector size.
     * @warning none.
     */
    size_t max_size( ) const noexcept;

    /**
     * @date 2015/Apr/01
     * @param n: new size.
     * @return none.
     * @brief Resizes vector.
     * @warning none.
     */
    void resize( size_t n );

    /**
     * @date 2015/Apr/01
     * @param n: new size.
     * @param val: value assigned to new elements.
     * @return none.
     * @brief Resizes vector.
     * @warning none.
     */
    void resize( size_t n, const bool &val );

    /**
     * @date 2015/Apr/01
     * @param none.
     * @return Vector capacity.
     * @brief Returns vector capacity.
     * @warning none.
     */
    size_t capacity( ) const noexcept;

    /**
     * @date 2015/Apr/01
     * @param none.
     * @return true if vector is empty.
     * @brief Tests if vector is empty.
     * @warning none.
     */
    bool empty( ) const noexcept;

    /**
     * @date 2015/Apr/01
     * @param n: new vector capacity.
     * @return none.
     * @brief Changes vector capacity.
     * @warning none.
     */
    void reserve( size_t n );

    /**
     * @date 2015/Apr/01
     * @param none.
     * @return none.
     * @brief Reduces vector capacity to size.
     * @warning none.
     */
    void shrink_to_fit( );

    /**
     * @date 2015/Jun/25
     * @param filename: Name of the file conatining vector.
     * @return The read vector.
     * @brief Reads vector from file.
     * @warning none.
     */
    static Vector< bool > Read( const std::string &filename );

    /**
     * @date 2015/Jun/25
     * @param filename: Name of the file conatining vector.
     * @return none.
     * @brief Writes vector into file.
     * @warning none.
     */
    void Write( const std::string &filename ) const;

    /**
     * @date 2015/Apr/01
     * @param n: a vector index.
     * @return Reference to data at position n.
     * @brief Returns reference to data at position n.
     * @warning none.
     */
    typename std::vector< bool >::reference operator[]( size_t n );
    bool operator[]( size_t n ) const;

    /**
     * @date 2015/Apr/01
     * @param n: a vector index.
     * @return Reference to data at position n.
     * @brief Returns reference to data at position n.
     * @warning none.
     */
    typename std::vector< bool >::reference operator()( size_t n );
    bool operator()( size_t n ) const;

    /**
     * @date 2015/Apr/01
     * @param n: a vector reference.
     * @return Reference to data at position n.
     * @brief Returns reference to data at position n, checking if it exists.
     * @warning none.
     */
    typename std::vector< bool >::reference at( size_t n );
    bool at( size_t n ) const;

    /**
     * @date 2015/Apr/01
     * @param none.
     * @return Vector's first element.
     * @brief Returns vector's first element.
     * @warning none.
     */
    typename std::vector< bool >::reference front( );
    bool front( ) const;

    /**
     * @date 2015/Apr/01
     * @param none.
     * @return Vector's last element.
     * @brief Returns vector's last element.
     * @warning none.
     */
    typename std::vector< bool >::reference back( );
    bool back( ) const;

    /**
     * @date 2015/Apr/01
     * @param none.
     * @return none.
     * @brief Assigns data to all elements.
     * @warning none.
     */
    template< class InputIterator >
    void assign( InputIterator first, InputIterator last );
    void assign( size_t n, const bool &val );
    void assign( std::initializer_list< bool > il );

    /**
     * @date 2015/Apr/01
     * @param none.
     * @return none.
     * @brief Assigns data to all elements.
     * @warning none.
     */
    void Set( bool val );

    /**
     * @date 2015/Apr/01
     * @param val: The element to be inserted.
     * @return none.
     * @brief Inserts element in the end of the vector.
     * @warning none.
     */
    void push_back( const bool &val );

    /**
     * @date 2015/Apr/01
     * @param val: The element to be inserted.
     * @return none.
     * @brief Inserts element in the end of the vector.
     * @warning none.
     */
    void push_back( bool &&val );

    /**
     * @date 2015/Apr/01
     * @param none.
     * @return none.
     * @brief Removes last element from vector.
     * @warning none.
     */
    void pop_back( );

    /**
     * @date 2015/Apr/01
     * @param many.
     * @return Pointer to the first inserted element.
     * @brief Inserts elements into the vector.
     * @warning none.
     */
    typename std::vector< bool >::iterator insert( typename std::vector< bool >::iterator position, const bool &val );
    typename std::vector< bool >::iterator insert( typename std::vector< bool >::iterator position, size_t n, 
                                                   const bool &val );
    template< class InputIterator >
    typename std::vector< bool >::iterator insert( typename std::vector< bool >::iterator position, InputIterator first,
                                                   InputIterator last );
    typename std::vector< bool >::iterator insert( typename std::vector< bool >::iterator position, bool &&val );
    typename std::vector< bool >::iterator insert( typename std::vector< bool >::iterator position,
                                                   std::initializer_list< bool > il );

    /**
     * @date 2015/Apr/01
     * @param many.
     * @return Pointer to element after the lastest removed.
     * @brief Removes elements from the vector.
     * @warning none.
     */
    typename std::vector< bool >::iterator erase( typename std::vector< bool >::iterator position );
    typename std::vector< bool >::iterator erase( typename std::vector< bool >::iterator first, 
                                                  typename std::vector< bool >::iterator last );

    /**
     * @date 2015/Apr/01
     * @param x: a vector.
     * @return none.
     * @brief Exchanges content of this vector by x.
     * @warning none.
     */
    void swap( Vector &x );

    /**
     * @date 2015/Apr/01
     * @param none.
     * @return none.
     * @brief Removes all elements from vector.
     * @warning none.
     */
    void clear( ) noexcept;

    /**
     * @date 2012/Jun/21
     * @param os: an output stream.
     * @return The output stream.
     * @brief Prints vector containts to output stream os.
     * @warning 'this' vector must be 1D, 2D, or 3D.
     */
    template< class O >
    O &Print( O &os ) const;

    /**
     * @date 2012/Jun/21
     * @param none.
     * @return The dimensions of the vector.
     * @brief Prints vector dimensions to output stream os.
     * @warning none.
     */
    template< class O >
    O &PrintDimensions( O &os ) const;

    /**
     * @date 2019/May/15
     * @param element: An element to be found in the vector.
     * @return True if the element is in the vector.
     * @brief Searches for an element inside the vector.
     * @warning none.
     */
    bool Has( bool element ) const;
  };

  /**
   * @date 2015/Apr/01
   * @param lhs: left vector.
   * @param rhs: right vector.
   * @return Result of operator.
   * @brief Operators.
   * @warning none.
   */
  template< class D >
  bool operator==( const Vector< D > &lhs, const Vector< D > &rhs );
  template< class D >
  bool operator!=( const Vector< D > &lhs, const Vector< D > &rhs );
  template< class D >
  bool operator<( const Vector< D > &lhs, const Vector< D > &rhs );
  template< class D >
  bool operator<=( const Vector< D > &lhs, const Vector< D > &rhs );
  template< class D >
  bool operator>( const Vector< D > &lhs, const Vector< D > &rhs );
  template< class D >
  bool operator>=( const Vector< D > &lhs, const Vector< D > &rhs );

  template< class D >
  void swap( Vector< D > &x, Vector< D > &y );

  /* @param os: output stream.
   *             mat: a vector.
   * @return Reference to os.
   * @brief Prints the vector in a friendly way.
   * @warning none.
   * template< typename D, class O > O& operator<<( O& os, const Vector< D >& mat );
   */
  template< class D, class O >
  O &operator<<( O &os, const Vector< D > &vec ) {
    vec.Print( os );
    return( os );
  }

}

#include "File.hpp"

/* Implementation ------------------------------------------------------------------------------------------------------
 *
 */

namespace Bial {

  /* Vector ************************************************************************************************************
   *
   */

  template< class D > Vector< D >::Vector( ) try : _data( ), qk_data( nullptr ), _size( 0 ) {
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

  template< class D > Vector< D >::Vector( size_t n ) try : _data( n ), qk_data( nullptr ), _size( n ) {
    qk_data = &_data[ 0 ];
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

  template< class D > Vector< D >::Vector( size_t n, const D &val ) try 
    : _data( n, val ), qk_data( nullptr ), _size( n ) {
      qk_data = &_data[ 0 ];
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
  template< class InputIterator > Vector< D >::Vector( InputIterator first, InputIterator last ) try 
    : _data( first, last ), qk_data( nullptr ), _size( 0 ) {
    qk_data = &_data[ 0 ];
    _size = _data.size( );
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

  template< class D > Vector< D >::Vector( const Vector< D > &x ) try 
    : _data( x._size ), qk_data( nullptr ), _size( x._size ) {
      qk_data = &_data[ 0 ];
      for( size_t elm = 0; elm < _size; ++elm )
        qk_data[ elm ] = x.qk_data[ elm ];
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

  template< class D > Vector< D >::Vector( Vector< D > &&x ) try 
    : _data( std::move( x._data ) ), qk_data( x.qk_data ), _size( x._size ) {
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

  template< class D > Vector< D >::Vector( const std::vector< D > &x ) try 
    : _data( x ), qk_data( nullptr ), _size( x.size( ) ) {
      qk_data = &_data[ 0 ];
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

  template< class D > Vector< D >::Vector( std::vector< D > &&x ) try 
    : _data( std::move( x ) ), qk_data( nullptr ), _size( 0 ) {
      qk_data = &_data[ 0 ];
      _size = _data.size( );
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

  template< class D > Vector< D >::Vector( D *new_data, size_t new_size ) try 
    : _data( ), qk_data( new_data ), _size( new_size ) {
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

  template< class D > Vector< D >::Vector( std::initializer_list< D > il ) try 
    : _data( il ), qk_data( nullptr ), _size( 0 ) {
      qk_data = &_data[ 0 ];
      _size = _data.size( );
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
  template< class D2 > Vector< D >::Vector( const Vector< D2 > &src ) try 
    : _data( src._size ), qk_data( nullptr ), _size( src._size ) {
      qk_data = &_data[ 0 ];

    COMMENT( "Assigning data.", 3 );
    for( size_t elm = 0; elm < _data.size( ); ++elm ) {
      QK_DATA( elm ) = static_cast< D >( src.QK_DATA( elm ) );
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

  template< class D >
  template< class D2 > Vector< D >::Vector( const std::vector< D2 > &src ) try 
    : _data( src._size ), qk_data( nullptr ), _size( src._size ) {
      qk_data = &_data[ 0 ];

      COMMENT( "Assigning data.", 3 );
      for( size_t elm = 0; elm < _data.size( ); ++elm )
        QK_DATA( elm ) = static_cast< D >( src[ elm ] );
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
  Vector< D > &Vector< D >::operator=( const Vector< D > &x ) {
    try {
      _data = std::vector< D >( x._size );
      qk_data = &_data[ 0 ];
      _size = x._size;

      for( size_t elm = 0; elm < _size; ++elm )
        QK_DATA( elm ) = x.QK_DATA( elm );
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
  Vector< D > &Vector< D >::operator=( Vector< D > &&x ) {
    try {
      _data = std::move( x._data );
      qk_data = x.qk_data;
      _size = x._size;
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
  Vector< D > &Vector< D >::operator=( std::initializer_list< D > il ) {
    try {
      _data = il;
      qk_data = &_data[ 0 ];
      _size = _data.size( );
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
  Vector< D > &Vector< D >::operator=( double val ) {
    try {
      for( size_t elm = 0; elm < _size; ++elm )
        QK_DATA( elm ) = static_cast< D >( val );
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
  inline Vector< D > &Vector< D >::operator+=( const Vector< D2 > &other ) {
    try {
      for( size_t elm = 0; elm < _size; ++elm )
        QK_DATA( elm ) += static_cast< D >( other.QK_DATA( elm ) );
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
  inline Vector< D > &Vector< D >::operator-=( const Vector< D2 > &other ) {
    try {
      for( size_t elm = 0; elm < _size; ++elm )
        QK_DATA( elm ) -= static_cast< D >( other.QK_DATA( elm ) );
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
  inline Vector< D > &Vector< D >::operator*=( const Vector< D2 > &other ) {
    try {
      for( size_t elm = 0; elm < _size; ++elm )
        QK_DATA( elm ) *= static_cast< D >( other.QK_DATA( elm ) );
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
  inline Vector< D > &Vector< D >::operator/=( const Vector< D2 > &other ) {
    try {
#ifdef BIAL_DEBUG
      COMMENT( "Performing division.", 2 );
      for( size_t elm = 0; elm < _size; ++elm ) {
        if( other.QK_DATA( elm ) == 0 ) {
          std::string msg( BIAL_ERROR( "Division by zero at element " + std::to_string( elm ) + "." ) );
          throw( std::logic_error( msg ) );
        }
        QK_DATA( elm ) /= static_cast< D >( other.QK_DATA( elm ) );
      }
#else
      COMMENT( "Performing division.", 2 );
      for( size_t elm = 0; elm < _size; ++elm )
        QK_DATA( elm ) /= static_cast< D >( other.QK_DATA( elm ) );
#endif
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
  inline Vector< D > Vector< D >::operator+( const Vector< D2 > &other ) const {
    try {
      Vector< D > tgt( *this );
      return( tgt += other );
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
  inline Vector< D > Vector< D >::operator-( const Vector< D2 > &other ) const {
    try {
      Vector< D > tgt( *this );
      return( tgt -= other );
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
  inline Vector< D > Vector< D >::operator*( const Vector< D2 > &other ) const {
    try {
      Vector< D > tgt( *this );
      return( tgt *= other );
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
  inline Vector< D > Vector< D >::operator/( const Vector< D2 > &other ) const {
    try {
      Vector< D > tgt( *this );
      return( tgt /= other );
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
  inline Vector< D > &Vector< D >::operator+=( const double scalar ) {
    try {
      for( size_t elm = 0; elm < _size; ++elm )
        QK_DATA( elm ) += static_cast< D >( scalar );
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
  inline Vector< D > &Vector< D >::operator-=( const double scalar ) {
    try {
      for( size_t elm = 0; elm < _size; ++elm )
        QK_DATA( elm ) -= static_cast< D >( scalar );
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
  inline Vector< D > &Vector< D >::operator*=( const double scalar ) {
    try {
      for( size_t elm = 0; elm < _size; ++elm )
        QK_DATA( elm ) *= static_cast< D >( scalar );
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
  inline Vector< D > &Vector< D >::operator/=( const double scalar ) {
    try {
      COMMENT( "Verifying if scalar is not zero.", 2 );
      if( scalar == 0.0 ) {
        std::string msg( BIAL_ERROR( "Division by zero." ) );
        throw( std::logic_error( msg ) );
      }
      for( size_t elm = 0; elm < _size; ++elm )
        QK_DATA( elm ) /= static_cast< D >( scalar );
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
  inline Vector< D > Vector< D >::operator+( const double scalar ) const {
    try {
      Vector< D > tgt( *this );
      return( tgt += scalar );
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
  inline Vector< D > Vector< D >::operator-( const double scalar ) const {
    try {
      Vector< D > tgt( *this );
      return( tgt -= scalar );
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
  inline Vector< D > Vector< D >::operator*( const double scalar ) const {
    try {
      Vector< D > tgt( *this );
      return( tgt *= scalar );
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
  inline Vector< D > Vector< D >::operator/( const double scalar ) const {
    try {
      Vector< D > tgt( *this );
      return( tgt /= scalar );
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
  void Vector< D >::Abs( ) {
    COMMENT( "Computing absolute value.", 2 );
    for( size_t elm = 0; elm < _size; ++elm )
      QK_DATA( elm ) = std::abs( QK_DATA( elm ) );
  }

  template< class D >
  Vector< D > Vector< D >::Abs( ) const {
    try {
      Vector< D > res( *this );
      res.Abs( );
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
    ;
  }

  template< class D >
  typename std::vector< D >::iterator Vector< D >::begin( ) {
    COMMENT( "Checking if this is a true Vector or just used as a Wrapper.", 4 );
    if( _data.size( ) != _size ) {
      std::string msg( BIAL_ERROR( "Vector wrapper cannot use this function." ) );
      throw( std::runtime_error( msg ) );
    }
    return( _data.begin( ) );
  }

  template< class D >
  typename std::vector< D >::const_iterator Vector< D >::begin( ) const {
    if( _data.size( ) != _size ) {
      std::string msg( BIAL_ERROR( "Vector wrapper cannot use this function." ) );
      throw( std::runtime_error( msg ) );
    }
    return( _data.begin( ) );
  }

  template< class D >
  typename std::vector< D >::iterator Vector< D >::end( ) {
    if( _data.size( ) != _size ) {
      std::string msg( BIAL_ERROR( "Vector wrapper cannot use this function." ) );
      throw( std::runtime_error( msg ) );
    }
    return( _data.end( ) );
  }

  template< class D >
  typename std::vector< D >::const_iterator Vector< D >::end( ) const {
    if( _data.size( ) != _size ) {
      std::string msg( BIAL_ERROR( "Vector wrapper cannot use this function." ) );
      throw( std::runtime_error( msg ) );
    }
    return( _data.end( ) );
  }

  template< class D >
  typename std::vector< D >::reverse_iterator Vector< D >::rbegin( ) {
    if( _data.size( ) != _size ) {
      std::string msg( BIAL_ERROR( "Vector wrapper cannot use this function." ) );
      throw( std::runtime_error( msg ) );
    }
    return( _data.rbegin( ) );
  }

  template< class D >
  typename std::vector< D >::const_reverse_iterator Vector< D >::rbegin( ) const {
    if( _data.size( ) != _size ) {
      std::string msg( BIAL_ERROR( "Vector wrapper cannot use this function." ) );
      throw( std::runtime_error( msg ) );
    }
    return( _data.rbegin( ) );
  }

  template< class D >
  typename std::vector< D >::reverse_iterator Vector< D >::rend( ) {
    if( _data.size( ) != _size ) {
      std::string msg( BIAL_ERROR( "Vector wrapper cannot use this function." ) );
      throw( std::runtime_error( msg ) );
    }
    return( _data.rend( ) );
  }

  template< class D >
  typename std::vector< D >::const_reverse_iterator Vector< D >::rend( ) const {
    if( _data.size( ) != _size ) {
      std::string msg( BIAL_ERROR( "Vector wrapper cannot use this function." ) );
      throw( std::runtime_error( msg ) );
    }
    return( _data.rend( ) );
  }

  template< class D >
  typename std::vector< D >::const_iterator Vector< D >::cbegin( ) const {
    if( _data.size( ) != _size ) {
      std::string msg( BIAL_ERROR( "Vector wrapper cannot use this function." ) );
      throw( std::runtime_error( msg ) );
    }
    return( _data.cbegin( ) );
  }

  template< class D >
  typename std::vector< D >::const_iterator Vector< D >::cend( ) const {
    if( _data.size( ) != _size ) {
      std::string msg( BIAL_ERROR( "Vector wrapper cannot use this function." ) );
      throw( std::runtime_error( msg ) );
    }
    return( _data.cend( ) );
  }

  template< class D >
  typename std::vector< D >::const_reverse_iterator Vector< D >::crbegin( ) const {
    if( _data.size( ) != _size ) {
      std::string msg( BIAL_ERROR( "Vector wrapper cannot use this function." ) );
      throw( std::runtime_error( msg ) );
    }
    return( _data.crbegin( ) );
  }

  template< class D >
  typename std::vector< D >::const_reverse_iterator Vector< D >::crend( ) const {
    if( _data.size( ) != _size ) {
      std::string msg( BIAL_ERROR( "Vector wrapper cannot use this function." ) );
      throw( std::runtime_error( msg ) );
    }
    return( _data.crend( ) );
  }

  template< class D >
  size_t Vector< D >::size( ) const noexcept {
    return( _size );
  }

  template< class D >
  size_t Vector< D >::Size( ) const noexcept {
    return( _size );
  }

  template< class D >
  size_t Vector< D >::Dims( ) const noexcept {
    return( 1 );
  }

  template< class D >
  size_t Vector< D >::max_size( ) const noexcept {
    return( _data.max_size( ) );
  }

  template< class D >
  void Vector< D >::resize( size_t n ) {
    if( _data.size( ) != _size ) {
      std::string msg( BIAL_ERROR( "Vector wrapper cannot use this function." ) );
      throw( std::runtime_error( msg ) );
    }
    _data.resize( n );
    qk_data = &_data[ 0 ];
    _size = n;
  }

  template< class D >
  void Vector< D >::resize( size_t n, const D &val ) {
    if( _data.size( ) != _size ) {
      std::string msg( BIAL_ERROR( "Vector wrapper cannot use this function." ) );
      throw( std::runtime_error( msg ) );
    }
    _data.resize( n, val );
    qk_data = &_data[ 0 ];
    _size = n;
  }

  template< class D >
  size_t Vector< D >::capacity( ) const {
    if( _data.size( ) != _size ) {
      std::string msg( BIAL_ERROR( "Vector wrapper cannot use this function." ) );
      throw( std::runtime_error( msg ) );
    }
    return( _data.capacity( ) );
  }

  template< class D >
  bool Vector< D >::empty( ) const noexcept {
    return( _size == 0 );
  }

  template< class D >
  void Vector< D >::reserve( size_t n ) {
    if( _data.size( ) != _size ) {
      std::string msg( BIAL_ERROR( "Vector wrapper cannot use this function." ) );
      throw( std::runtime_error( msg ) );
    }
    try {
      _data.reserve( n );
      qk_data = &_data[ 0 ];
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
  void Vector< D >::shrink_to_fit( ) {
    if( _data.size( ) != _size ) {
      std::string msg( BIAL_ERROR( "Vector wrapper cannot use this function." ) );
      throw( std::runtime_error( msg ) );
    }
    try {
      _data.shrink_to_fit( );
      qk_data = &_data[ 0 ];
      _size = _data.size( );
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
  Vector< D > Vector< D >::Read( const std::string &filename ) {
    try {
      COMMENT( "Opening bial vector file.", 2 );
      IFile file;
      file.exceptions( std::fstream::failbit | std::fstream::badbit );
      file.open( filename );
      COMMENT( "Checking magic number.", 2 );
      int mgc_nbr;
      file.read( reinterpret_cast< char* >( &mgc_nbr ), sizeof( int ) );
      if( mgc_nbr == 5023 ) {
        COMMENT( "Reading size.", 2 );
        int size;
        file.read( reinterpret_cast< char* >( &size ), sizeof( int ) );
        COMMENT( "Read size: " << size, 2 );
        COMMENT( "Creating vector.", 2 );
        Vector< D > res( size );
        COMMENT( "Getting data type.", 2 );
        int type;
        file.read( reinterpret_cast< char* >( &type ), sizeof( int ) );
        COMMENT( "Read type: " << static_cast< int >( type ), 2 );
        COMMENT( "Getting number of elements.", 2 );
        size_t vet_size = res._size;
        if( type == 8 ) {
          COMMENT( "Reading 8 bit integer data.", 2 );
          unsigned char *data8;
          data8 = new unsigned char[ vet_size ];
          file.read( reinterpret_cast< char* >( data8 ), sizeof( unsigned char ) * vet_size );
          for( size_t pxl = 0; pxl < vet_size; ++pxl ) {
            res.QK_DATA( pxl ) = static_cast< D >( data8[ pxl ] );
          }
          delete[] data8;
        }
        else if( type == 16 ) {
          COMMENT( "Reading 16 bit integer data.", 2 );
          unsigned short *data16;
          data16 = new unsigned short[ vet_size ];
          file.read( reinterpret_cast< char* >( data16 ), sizeof( unsigned short ) * vet_size );
          for( size_t pxl = 0; pxl < vet_size; ++pxl ) {
            res.QK_DATA( pxl ) = static_cast< D >( data16[ pxl ] );
          }
          delete[] data16;
        }
        else if( type == 32 ) {
          COMMENT( "Reading 32 bit integer data.", 2 );
          int *data32;
          data32 = new int[ vet_size ];
          file.read( reinterpret_cast< char* >( data32 ), sizeof( int ) * vet_size );
          for( size_t pxl = 0; pxl < vet_size; ++pxl ) {
            res.QK_DATA( pxl ) = static_cast< D >( data32[ pxl ] );
          }
          delete[] data32;
        }
        else if( type == 64 ) {
          COMMENT( "Reading 64 bit integer data.", 2 );
          long long *data64;
          data64 = new long long[ vet_size ];
          file.read( reinterpret_cast< char* >( data64 ), sizeof( long long ) * vet_size );
          for( size_t pxl = 0; pxl < vet_size; ++pxl ) {
            res.QK_DATA( pxl ) = static_cast< D >( data64[ pxl ] );
          }
          delete[] data64;
        }
        else if( type == -32 ) {
          COMMENT( "Reading 32 bit float data.", 2 );
          float *fdata32;
          fdata32 = new float[ vet_size ];
          file.read( reinterpret_cast< char* >( fdata32 ), sizeof( float ) * vet_size );
          for( size_t pxl = 0; pxl < vet_size; ++pxl ) {
            res.QK_DATA( pxl ) = static_cast< D >( fdata32[ pxl ] );
          }
          delete[] fdata32;
        }
        else { /* if( type == -64 ) */
          COMMENT( "Reading 64 bit float data.", 2 );
          double *fdata64;
          fdata64 = new double[ vet_size ];
          file.read( reinterpret_cast< char* >( fdata64 ), sizeof( double ) * vet_size );
          for( size_t pxl = 0; pxl < vet_size; ++pxl ) {
            res.QK_DATA( pxl ) = static_cast< D >( fdata64[ pxl ] );
          }
          delete[] fdata64;
        }
        file.close( );
        return( res );
      }
      COMMENT( "Not a bial vector type.", 2 );
      std::string msg( BIAL_ERROR( "Not a bial vector type. Wrong magic string." ) );
      throw( std::logic_error( msg ) );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "I/O error while reading file." ) );
      throw( std::ios_base::failure( msg ) );
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
  void Vector< D >::Write( const std::string &filename ) const {
    try {
      COMMENT( "Opening file.", 2 );
      OFile file;
      file.exceptions( std::fstream::failbit | std::fstream::badbit );
      file.open( filename );
      COMMENT( "Writing magic number and vector dimensions.", 2 );
      int mgc_nbr = 5023;
      int i_size = _size;
      file.write( reinterpret_cast< const char* >( &mgc_nbr ), sizeof( int ) );
      file.write( reinterpret_cast< const char* >( &i_size ), sizeof( int ) );
      COMMENT( "Getting maximum intensity.", 2 );
      D Imax = Maximum( );
      COMMENT( "Writing data.", 2 );
      if( static_cast< D >( 1.1f ) != static_cast< D >( 1.0f ) ) {
        COMMENT( "floating point data.", 2 );
        if( sizeof( D ) == 4 ) {
          COMMENT( "32 bit float point vector.", 2 );
          int code = -32;
          file.write( reinterpret_cast< char* >( &code ), sizeof( int ) );
          file.write( reinterpret_cast< const char* >( &QK_DATA( 0 ) ), sizeof( float ) * i_size );
        }
        else { /* double data -> 64 bit. */
          COMMENT( "64 bit float point vector.", 2 );
          int code = -64;
          file.write( reinterpret_cast< char* >( &code ), sizeof( int ) );
          file.write( reinterpret_cast< const char* >( &QK_DATA( 0 ) ), sizeof( double ) * i_size );
        }
      }
      else if( Imax < 256 ) {
        COMMENT( "8 bit integer point vector.", 2 );
        int code = 8;
        file.write( reinterpret_cast< char* >( &code ), sizeof( int ) );
        Vector< unsigned char > data8( i_size );
        for( size_t elm = 0; elm < i_size; ++elm ) {
          data8[ elm ] = static_cast< unsigned char >( QK_DATA( elm ) );
        }
        file.write( reinterpret_cast< const char* >( &data8[ 0 ] ), sizeof( unsigned char ) * i_size );
      }
      else if( Imax < 65536 ) {
        COMMENT( "16 bit integer point vector.", 2 );
        int code = 16;
        file.write( reinterpret_cast< char* >( &code ), sizeof( int ) );
        Vector< unsigned short > data16( i_size );
        for( size_t elm = 0; elm < i_size; ++elm ) {
          data16[ elm ] = static_cast< unsigned short >( QK_DATA( elm ) );
        }
        file.write( reinterpret_cast< const char* >( &data16[ 0 ] ), sizeof( unsigned short ) * i_size );
      }
      else if( Imax < 2147483648 ) {
        COMMENT( "32 bit integer point vector.", 2 );
        int code = 32;
        file.write( reinterpret_cast< char* >( &code ), sizeof( int ) );
        Vector< int > data32( i_size );
        for( size_t elm = 0; elm < i_size; ++elm ) {
          data32[ elm ] = static_cast< int >( QK_DATA( elm ) );
        }
        file.write( reinterpret_cast< const char* >( &data32[ 0 ] ), sizeof( int ) * i_size );
      }
      else { /* 64 bit integer. */
        COMMENT( "64 bit integer point vector.", 2 );
        int code = 64;
        file.write( reinterpret_cast< char* >( &code ), sizeof( int ) );
        file.write( reinterpret_cast< const char* >( &QK_DATA( 0 ) ), sizeof( int ) * i_size );
      }
      file.close( );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "I/O error while writing to file." ) );
      throw( std::ios_base::failure( msg ) );
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
  D &Vector< D >::operator[]( size_t n ) {
#ifdef BIAL_DEBUG
    if( n >= _size ) {
      std::string msg( BIAL_ERROR( "Index out of range. Expected: < " + std::to_string( _size ) + ", given: " +
                                   std::to_string( n ) + "." ) );
      throw( std::out_of_range( msg ) );
    }
#endif
    return( qk_data[ n ] );
  }

  template< class D >
  const D &Vector< D >::operator[]( size_t n ) const {
#ifdef BIAL_DEBUG
    if( n >= _size ) {
      std::string msg( BIAL_ERROR( "Index out of range. Expected: < " + std::to_string( _size ) + ", given: " +
                                   std::to_string( n ) + "." ) );
      throw( std::out_of_range( msg ) );
    }
#endif
    return( qk_data[ n ] );
  }

  template< class D >
  D &Vector< D >::operator()( size_t n ) {
#ifdef BIAL_DEBUG
    if( n >= _size ) {
      std::string msg( BIAL_ERROR( "Index out of range. Expected: < " + std::to_string( _size ) + ", given: " +
                                   std::to_string( n ) + "." ) );
      throw( std::out_of_range( msg ) );
    }
#endif
    return( qk_data[ n ] );
  }

  template< class D >
  const D &Vector< D >::operator()( size_t n ) const {
#ifdef BIAL_DEBUG
    if( n >= _size ) {
      std::string msg( BIAL_ERROR( "Index out of range. Expected: < " + std::to_string( _size ) + ", given: " +
                                   std::to_string( n ) + "." ) );
      throw( std::out_of_range( msg ) );
    }
#endif
    return( qk_data[ n ] );
  }

  template< class D >
  D &Vector< D >::at( size_t n ) {
    if( n >= _size ) {
      std::string msg( BIAL_ERROR( "Index out of range. Expected: < " + std::to_string( _size ) + ", given: " +
                                   std::to_string( n ) + "." ) );
      throw( std::out_of_range( msg ) );
    }
    return( qk_data[ n ] );
  }

  template< class D >
  const D &Vector< D >::at( size_t n ) const {
    if( n >= _size ) {
      std::string msg( BIAL_ERROR( "Index out of range. Expected: < " + std::to_string( _size ) + ", given: " +
                                   std::to_string( n ) + "." ) );
      throw( std::out_of_range( msg ) );
    }
    return( qk_data[ n ] );
  }

  template< class D >
  D &Vector< D >::front( ) {
    try {
      if( _size == 0 ) {
        std::string msg( BIAL_ERROR( "Empty vector access." ) );
        throw( std::out_of_range( msg ) );
      }
      return( QK_DATA( 0 ) );
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
  const D &Vector< D >::front( ) const {
    try {
      if( _size == 0 ) {
        std::string msg( BIAL_ERROR( "Empty vector access." ) );
        throw( std::out_of_range( msg ) );
      }
      return( QK_DATA( 0 ) );
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
  D &Vector< D >::back( ) {
    try {
      if( _size == 0 ) {
        std::string msg( BIAL_ERROR( "Empty vector access." ) );
        throw( std::out_of_range( msg ) );
      }
      return( QK_DATA( _size - 1 ) );
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
  const D &Vector< D >::back( ) const {
    try {
      if( _size == 0 ) {
        std::string msg( BIAL_ERROR( "Empty vector access." ) );
        throw( std::out_of_range( msg ) );
      }
      return( QK_DATA( _size - 1 ) );
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
  D*Vector< D >::data( ) noexcept {
    return( qk_data );
  }

  template< class D >
  const D*Vector< D >::data( ) const noexcept {
    return( qk_data );
  }

  template< class D >
  template< class InputIterator >
  void Vector< D >::assign( InputIterator first, InputIterator last ) {
    if( _data.size( ) != _size ) {
      std::string msg( BIAL_ERROR( "Vector wrapper cannot use this function." ) );
      throw( std::runtime_error( msg ) );
    }
    try {
      _data.assign( first, last );
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
  void Vector< D >::assign( size_t n, const D &val ) {
    if( _data.size( ) != _size ) {
      std::string msg( BIAL_ERROR( "Vector wrapper cannot use this function." ) );
      throw( std::runtime_error( msg ) );
    }
    try {
      _data.assign( n, val );
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
  void Vector< D >::assign( std::initializer_list< D > il ) {
    if( _data.size( ) != _size ) {
      std::string msg( BIAL_ERROR( "Vector wrapper cannot use this function." ) );
      throw( std::runtime_error( msg ) );
    }
    try {
      _data.assign( il );
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
  void Vector< D >::Set( double val ) {
    try {
      for( size_t elm = 0; elm < _size; ++elm )
        QK_DATA( elm ) = static_cast< D >( val );
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
  void Vector< D >::push_back( const D &val ) {
    if( _data.size( ) != _size ) {
      std::string msg( BIAL_ERROR( "Vector wrapper cannot use this function." ) );
      throw( std::runtime_error( msg ) );
    }
    try {
      _data.push_back( val );
      qk_data = &_data[ 0 ];
      ++_size;
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
  bool Vector< D >::contains( const D &val ) {
    try {
      for( size_t pos = 0; pos < _size; ++pos ) {
        if( QK_DATA( pos ) == val )
          return( true );
      }
      return( false );
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
  int Vector< D >::first_of( const D &val ) {
    try {
      for( size_t pos = 0; pos < _size; ++pos ) {
        if( QK_DATA( pos ) == val )
          return( static_cast< int >( pos ) );
      }
      return( -1 );
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
  void Vector< D >::push_back( D &&val ) {
    if( _data.size( ) != _size ) {
      std::string msg( BIAL_ERROR( "Vector wrapper cannot use this function." ) );
      throw( std::runtime_error( msg ) );
    }
    try {
      _data.push_back( std::move( val ) );
      qk_data = &_data[ 0 ];
      ++_size;
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
  void Vector< D >::pop_back( ) {
    if( _data.size( ) != _size ) {
      std::string msg( BIAL_ERROR( "Vector wrapper cannot use this function." ) );
      throw( std::runtime_error( msg ) );
    }
    try {
      _data.pop_back( );
      qk_data = &_data[ 0 ];
      --_size;
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
  typename std::vector< D >::iterator Vector< D >::insert( typename std::vector< D >::iterator position,
                                                           const D &val ) {
    if( _data.size( ) != _size ) {
      std::string msg( BIAL_ERROR( "Vector wrapper cannot use this function." ) );
      throw( std::runtime_error( msg ) );
    }
    try {
      typename std::vector< D >::iterator res = _data.insert( position, val );
      qk_data = &_data[ 0 ];
      _size = _data.size( );
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
  typename std::vector< D >::iterator Vector< D >::insert( typename std::vector< D >::iterator position,
                                                           size_t n, const D &val ) {
    if( _data.size( ) != _size ) {
      std::string msg( BIAL_ERROR( "Vector wrapper cannot use this function." ) );
      throw( std::runtime_error( msg ) );
    }
    try {
      _data.insert( position, n, val );
      qk_data = &_data[ 0 ];
      _size = _data.size( );
      return( position );
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
  template< class InputIterator >
  typename std::vector< D >::iterator Vector< D >::insert( typename std::vector< D >::iterator position,
                                                           InputIterator first, InputIterator last ) {
    if( _data.size( ) != _size ) {
      std::string msg( BIAL_ERROR( "Vector wrapper cannot use this function." ) );
      throw( std::runtime_error( msg ) );
    }
    try {
      _data.insert( position, first, last );
      qk_data = &_data[ 0 ];
      _size = _data.size( );
      return( position );
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
  typename std::vector< D >::iterator Vector< D >::insert( typename std::vector< D >::iterator position, D && val ) {
    if( _data.size( ) != _size ) {
      std::string msg( BIAL_ERROR( "Vector wrapper cannot use this function." ) );
      throw( std::runtime_error( msg ) );
    }
    try {
      _data.insert( position, std::move( val ) );
      qk_data = &_data[ 0 ];
      _size = _data.size( );
      return( position );
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
  typename std::vector< D >::iterator Vector< D >::insert( typename std::vector< D >::iterator position,
                                                           std::initializer_list< D > il ) {
    if( _data.size( ) != _size ) {
      std::string msg( BIAL_ERROR( "Vector wrapper cannot use this function." ) );
      throw( std::runtime_error( msg ) );
    }
    try {
      _data.insert( position, il );
      qk_data = &_data[ 0 ];
      _size = _data.size( );
      return( position );
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
  typename std::vector< D >::iterator Vector< D >::erase( typename std::vector< D >::iterator position ) {
    if( _data.size( ) != _size ) {
      std::string msg( BIAL_ERROR( "Vector wrapper cannot use this function." ) );
      throw( std::runtime_error( msg ) );
    }
    if( ( position < begin( ) ) || ( position >= end( ) ) ) {
      std::string msg( BIAL_ERROR( "Trying to access invalid element." ) );
      throw( std::runtime_error( msg ) );
    }
    try {
      _data.erase( position );
      qk_data = &_data[ 0 ];
      _size = _data.size( );
      return( position );
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
  typename std::vector< D >::iterator Vector< D >::erase( typename std::vector< D >::iterator first,
                                                          typename std::vector< D >::iterator last ) {
    if( _data.size( ) != _size ) {
      std::string msg( BIAL_ERROR( "Vector wrapper cannot use this function." ) );
      throw( std::runtime_error( msg ) );
    }
    if( ( first < begin( ) ) || ( last > end( ) ) ) {
      std::string msg( BIAL_ERROR( "Trying to access invalid element." ) );
      throw( std::runtime_error( msg ) );
    }
    try {
      _data.erase( first, last );
      qk_data = &_data[ 0 ];
      _size = _data.size( );
      return( first );
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
  void Vector< D >::swap( Vector &x ) {
    try {
      _data.swap( x._data );
      std::swap( qk_data, x.qk_data );
      std::swap( _size, x._size );
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
  void Vector< D >::clear( ) {
    if( _data.size( ) != _size ) {
      std::string msg( BIAL_ERROR( "Vector wrapper cannot use this function." ) );
      throw( std::runtime_error( msg ) );
    }
    _data.clear( );
    qk_data = nullptr;
    _size = _data.size( );
  }

  template< class D >
  template< class ... Args >
  typename std::vector< D >::iterator 
  Vector< D >::emplace( typename std::vector< D >::const_iterator position, Args && ... args ) {
    if( _data.size( ) != _size ) {
      std::string msg( BIAL_ERROR( "Vector wrapper cannot use this function." ) );
      throw( std::runtime_error( msg ) );
    }
    try {
      typename std::vector< D >::iterator res = _data.emplace( position, std::forward< Args >( args ) ... );
      qk_data = &_data[ 0 ];
      _size = _data.size( );
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
  template< class ... Args >
  void Vector< D >::emplace_back( Args && ... args ) {
    if( _data.size( ) != _size ) {
      std::string msg( BIAL_ERROR( "Vector wrapper cannot use this function." ) );
      throw( std::runtime_error( msg ) );
    }
    try {
      _data.emplace_back( std::forward< Args >( args ) ... );
      qk_data = &_data[ 0 ];
      _size = _data.size( );
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
  D Vector< D >::Maximum( ) const {
    try {
      return( *std::max_element( qk_data, qk_data + _size ) );
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
  D Vector< D >::Minimum( ) const {
    try {
      return( *std::min_element( qk_data, qk_data + _size ) );
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

  template< class D > template< class D2 >
  bool Vector< D >::SameDimensions( const Vector< D2 > &src ) const {
    return( _size == src._size );
  }

  template< class D >
  template< class O >
  O &Vector< D >::Print( O &os ) const {
    try {
      for( size_t i = 0; i < _size; ++i ) {
        os << QK_DATA( i );
        if( i != _size - 1 ) {
          os << " ";
        }
      }
      return( os );
    }
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
  }

  template< class D >
  template< class O >
  O &Vector< D >::PrintDimensions( O &os ) const {
    try {
      os << _size;
      return( os );
    }
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
  }

  template< class D >
  bool Vector< D >::Has( D elm ) const {
    try {
      for( size_t idx = 0; idx < _size; ++idx ) {
        if( QK_DATA( idx ) == elm )
          return( true );
      }
      return( false );
    }
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
  }

  /* Vector< bool > ****************************************************************************************************
   *
   */

  inline Vector< bool >::Vector( ) try : _data( ), qk_data( ), _size( 0 ) {
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

  inline Vector< bool >::Vector( size_t n ) try : _data( n ), qk_data( ), _size( n ) {
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

  inline Vector< bool >::Vector( size_t n, const bool &val ) try : _data( n, val ), qk_data( ), _size( n ) {
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

  template< class InputIterator >
  inline Vector< bool >::Vector( InputIterator first, InputIterator last ) try 
    : _data( first, last ), qk_data( ), _size( 0 ) {
      _size = _data.size( );
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

  inline Vector< bool >::Vector( const Vector< bool > &x ) try 
    : _data( x._data ), qk_data( ), _size( x._size ) {
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

  inline Vector< bool >::Vector( Vector< bool > &&x ) try 
    : _data( std::move( x._data ) ), qk_data( ), _size( 0 ) {
      _size = _data.size( );
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

  inline Vector< bool >::Vector( const std::vector< bool > &x ) try 
    : _data( x ), qk_data( ), _size( x.size( ) ) {
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

  inline Vector< bool >::Vector( std::vector< bool > &&x ) try 
    : _data( std::move( x ) ), qk_data( ), _size( 0 ) {
      _size = _data.size( );
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

  inline Vector< bool >::Vector( std::initializer_list< bool > il ) try 
    : _data( il ), qk_data( ), _size( 0 ) {
      _size = _data.size( );
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

  inline Vector< bool >
  &Vector< bool >::operator=( const Vector< bool > &x ) {
    try {
      _data = x._data;
      _size = x._size;
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

  inline Vector< bool >
  &Vector< bool >::operator=( Vector< bool > &&x ) {
    try {
      _data = std::move( x._data );
      _size = _data.size( );
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

  inline Vector< bool >
  &Vector< bool >::operator=( std::initializer_list< bool > il ) {
    try {
      _data = il;
      _size = _data.size( );
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

  inline typename std::vector< bool >::iterator Vector< bool >::begin( ) noexcept {
    return( _data.begin( ) );
  }

  inline typename std::vector< bool >::const_iterator Vector< bool >::begin( ) const noexcept {
    return( _data.begin( ) );
  }

  inline typename std::vector< bool >::iterator Vector< bool >::end( ) noexcept {
    return( _data.end( ) );
  }

  inline typename std::vector< bool >::const_iterator Vector< bool >::end( ) const noexcept {
    return( _data.end( ) );
  }

  inline typename std::vector< bool >::reverse_iterator Vector< bool >::rbegin( ) noexcept {
    return( _data.rbegin( ) );
  }

  inline typename std::vector< bool >::const_reverse_iterator Vector< bool >::rbegin( ) const noexcept {
    return( _data.rbegin( ) );
  }

  inline typename std::vector< bool >::reverse_iterator Vector< bool >::rend( ) noexcept {
    return( _data.rend( ) );
  }

  inline typename std::vector< bool >::const_reverse_iterator Vector< bool >::rend( ) const noexcept {
    return( _data.rend( ) );
  }

  inline typename std::vector< bool >::const_iterator Vector< bool >::cbegin( ) const noexcept {
    return( _data.cbegin( ) );
  }

  inline typename std::vector< bool >::const_iterator Vector< bool >::cend( ) const noexcept {
    return( _data.cend( ) );
  }

  inline typename std::vector< bool >::const_reverse_iterator Vector< bool >::crbegin( ) const noexcept {
    return( _data.crbegin( ) );
  }

  inline typename std::vector< bool >::const_reverse_iterator Vector< bool >::crend( ) const noexcept {
    return( _data.crend( ) );
  }

  inline size_t Vector< bool >::size( ) const noexcept {
    return( _size );
  }

  inline size_t Vector< bool >::Size( ) const noexcept {
    return( _size );
  }

  inline size_t Vector< bool >::Dims( ) const noexcept {
    return( 1 );
  }

  inline size_t Vector< bool >::max_size( ) const noexcept {
    return( _data.max_size( ) );
  }

  inline void Vector< bool >::resize( size_t n ) {
    _data.resize( n );
    _size = n;
  }

  inline void Vector< bool >::resize( size_t n, const bool &val ) {
    _data.resize( n, val );
    _size = n;
  }

  inline size_t Vector< bool >::capacity( ) const noexcept {
    return( _data.capacity( ) );
  }

  inline bool Vector< bool >::empty( ) const noexcept {
    return( _data.empty( ) );
  }

  inline void Vector< bool >::reserve( size_t n ) {
    try {
      _data.reserve( n );
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

  inline void Vector< bool >::shrink_to_fit( ) {
    try {
      _data.shrink_to_fit( );
      _size = _data.size( );
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

  inline Vector< bool > Vector< bool >::Read( const std::string &filename ) {
    try {
      COMMENT( "Opening bial vector file.", 2 );
      IFile file;
      file.exceptions( std::fstream::failbit | std::fstream::badbit );
      file.open( filename );
      COMMENT( "Checking magic number.", 2 );
      int mgc_nbr;
      file.read( reinterpret_cast< char* >( &mgc_nbr ), sizeof( int ) );
      if( mgc_nbr == 5023 ) {
        COMMENT( "Reading size.", 2 );
        int size;
        file.read( reinterpret_cast< char* >( &size ), sizeof( int ) );
        COMMENT( "Read size: " << size, 2 );
        COMMENT( "Creating vector.", 2 );
        Vector< bool > res( size );
        COMMENT( "Getting data type.", 2 );
        int type;
        file.read( reinterpret_cast< char* >( &type ), sizeof( int ) );
        COMMENT( "Read type: " << static_cast< int >( type ), 2 );
        COMMENT( "Getting number of elements.", 2 );
        size_t vet_size = res._size;
        if( type == 8 ) {
          COMMENT( "Reading 8 bit integer data.", 2 );
          unsigned char *data8;
          data8 = new unsigned char[ vet_size ];
          file.read( reinterpret_cast< char* >( data8 ), sizeof( unsigned char ) * vet_size );
          for( size_t pxl = 0; pxl < vet_size; ++pxl ) {
            res._data[ pxl ] = static_cast< bool >( data8[ pxl ] );
          }
          delete[] data8;
        }
        else if( type == 16 ) {
          COMMENT( "Reading 16 bit integer data.", 2 );
          unsigned short *data16;
          data16 = new unsigned short[ vet_size ];
          file.read( reinterpret_cast< char* >( data16 ), sizeof( unsigned short ) * vet_size );
          for( size_t pxl = 0; pxl < vet_size; ++pxl ) {
            res._data[ pxl ] = static_cast< bool >( data16[ pxl ] );
          }
          delete[] data16;
        }
        else if( type == 32 ) {
          COMMENT( "Reading 32 bit integer data.", 2 );
          int *data32;
          data32 = new int[ vet_size ];
          file.read( reinterpret_cast< char* >( data32 ), sizeof( int ) * vet_size );
          for( size_t pxl = 0; pxl < vet_size; ++pxl ) {
            res._data[ pxl ] = static_cast< bool >( data32[ pxl ] );
          }
          delete[] data32;
        }
        else if( type == 64 ) {
          COMMENT( "Reading 64 bit integer data.", 2 );
          long long *data64;
          data64 = new long long[ vet_size ];
          file.read( reinterpret_cast< char* >( data64 ), sizeof( long long ) * vet_size );
          for( size_t pxl = 0; pxl < vet_size; ++pxl ) {
            res._data[ pxl ] = static_cast< bool >( data64[ pxl ] );
          }
          delete[] data64;
        }
        else if( type == -32 ) {
          COMMENT( "Reading 32 bit float data.", 2 );
          float *fdata32;
          fdata32 = new float[ vet_size ];
          file.read( reinterpret_cast< char* >( fdata32 ), sizeof( float ) * vet_size );
          for( size_t pxl = 0; pxl < vet_size; ++pxl ) {
            res._data[ pxl ] = static_cast< bool >( fdata32[ pxl ] );
          }
          delete[] fdata32;
        }
        else { /* if( type == -64 ) */
          COMMENT( "Reading 64 bit float data.", 2 );
          double *fdata64;
          fdata64 = new double[ vet_size ];
          file.read( reinterpret_cast< char* >( fdata64 ), sizeof( double ) * vet_size );
          for( size_t pxl = 0; pxl < vet_size; ++pxl ) {
            res._data[ pxl ] = static_cast< bool >( fdata64[ pxl ] );
          }
          delete[] fdata64;
        }
        file.close( );
        return( res );
      }
      COMMENT( "Not a bial vector type.", 2 );
      std::string msg( BIAL_ERROR( "Not a bial vector type. Wrong magic string." ) );
      throw( std::logic_error( msg ) );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "I/O error while reading file." ) );
      throw( std::ios_base::failure( msg ) );
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

  inline void Vector< bool >::Write( const std::string &filename ) const {
    try {
      COMMENT( "Opening file.", 2 );
      OFile file;
      file.exceptions( std::fstream::failbit | std::fstream::badbit );
      file.open( filename );
      COMMENT( "Writing magic number and vector dimensions.", 2 );
      int mgc_nbr = 5023;
      int i_size = _size;
      file.write( reinterpret_cast< const char* >( &mgc_nbr ), sizeof( int ) );
      file.write( reinterpret_cast< const char* >( &i_size ), sizeof( int ) );
      COMMENT( "Writing 8 bit integer point vector.", 2 );
      int code = 8;
      file.write( reinterpret_cast< char* >( &code ), sizeof( int ) );
      Vector< unsigned char > data8( i_size );
      for( size_t elm = 0; elm < static_cast< size_t >( i_size ); ++elm ) {
        data8[ elm ] = static_cast< unsigned char >( _data[ elm ] );
      }
      file.write( reinterpret_cast< const char* >( &data8[ 0 ] ), sizeof( unsigned char ) * i_size );
      file.close( );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "I/O error while writing to file." ) );
      throw( std::ios_base::failure( msg ) );
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

  inline typename std::vector< bool >::reference Vector< bool >::operator[]( size_t n ) {
#ifdef BIAL_DEBUG
    return( _data.at( n ) );
#else
    return( _data[ n ] );
#endif
  }

  inline bool Vector< bool >::operator[]( size_t n ) const {
#ifdef BIAL_DEBUG
    return( _data.at( n ) );
#else
    return( _data[ n ] );
#endif
  }

  inline typename std::vector< bool >::reference Vector< bool >::operator()( size_t n ) {
#ifdef BIAL_DEBUG
    return( _data.at( n ) );
#else
    return( _data[ n ] );
#endif
  }

  inline bool Vector< bool >::operator()( size_t n ) const {
#ifdef BIAL_DEBUG
    return( _data.at( n ) );
#else
    return( _data[ n ] );
#endif
  }

  inline typename std::vector< bool >::reference Vector< bool >::at( size_t n ) {
    return( _data.at( n ) );
  }

  inline bool Vector< bool >::at( size_t n ) const {
    return( _data.at( n ) );
  }

  inline typename std::vector< bool >::reference Vector< bool >::front( ) {
    try {
      return( _data.front( ) );
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

  inline bool Vector< bool >::front( ) const {
    try {
      return( _data.front( ) );
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

  inline typename std::vector< bool >::reference Vector< bool >::back( ) {
    try {
      return( _data.back( ) );
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

  inline bool Vector< bool >::back( ) const {
    try {
      return( _data.back( ) );
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

  template< class InputIterator >
  inline void Vector< bool >::assign( InputIterator first, InputIterator last ) {
    try {
      _data.assign( first, last );
      _size = _data.size( );
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

  inline void Vector< bool >::assign( size_t n, const bool &val ) {
    try {
      _data.assign( n, val );
      _size = n;
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

  inline void Vector< bool >::assign( std::initializer_list< bool > il ) {
    try {
      _data.assign( il );
      _size = _data.size( );
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

  inline void Vector< bool >::Set( bool val ) {
    try {
      for( size_t elm = 0; elm < _size; ++elm )
        _data[ elm ] = val;
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

  inline void Vector< bool >::push_back( const bool &val ) {
    try {
      _data.push_back( val );
      ++_size;
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

  inline void Vector< bool >::push_back( bool &&val ) {
    try {
      _data.push_back( std::move( val ) );
      ++_size;
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

  inline void Vector< bool >::pop_back( ) {
    try {
      _data.pop_back( );
      --_size;
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

  inline typename std::vector< bool >::iterator Vector< bool >::insert( typename std::vector< bool >::iterator position,
                                                                        const bool &val ) {
    try {
      ++_size;
      return( _data.insert( position, val ) );
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

  inline typename std::vector< bool >::iterator Vector< bool >::insert( typename std::vector< bool >::iterator position,
                                                                        size_t n, const bool &val ) {
    try {
      _data.insert( position, n, val );
      _size = _data.size( );
      return( position );
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

  template< class InputIterator >
  inline typename std::vector< bool >::iterator Vector< bool >::insert( typename std::vector< bool >::iterator position,
                                                                        InputIterator first, InputIterator last ) {
    try {
      _data.insert( position, first, last );
      _size = _data.size( );
      return( position );
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

  inline typename std::vector< bool >::iterator Vector< bool >::insert( typename std::vector< bool >::iterator position,
                                                                        bool &&val ) {
    try {
      _data.insert( position, std::move( val ) );
      _size = _data.size( );
      return( position );
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

  inline typename std::vector< bool >::iterator Vector< bool >::insert( typename std::vector< bool >::iterator position,
                                                                        std::initializer_list< bool > il ) {
    try {
      _data.insert( position, il );
      _size = _data.size( );
      return( position );
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

  inline typename std::vector< bool >::iterator Vector< bool >::erase( typename std::vector< bool >::iterator position )
  {
    try {
      _data.erase( position );
      _size = _data.size( );
      return( position );
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

  inline typename std::vector< bool >::iterator Vector< bool >::erase( typename std::vector< bool >::iterator first,
                                                                       typename std::vector< bool >::iterator last ) {
    try {
      _data.erase( first, last );
      _size = _data.size( );
      return( first );
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

  inline void Vector< bool >::swap( Vector &x ) {
    try {
      _data.swap( x._data );
      std::swap( x._size, _size );
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

  inline void Vector< bool >::clear( ) noexcept {
    _data.clear( );
    _size = _data.size( );
  }

  template< class O >
  inline O &Vector< bool >::Print( O &os ) const {
    try {
      for( size_t i = 0; i < _size; ++i ) {
        os << _data[ i ];
        if( i != _size - 1 ) {
          os << " ";
        }
      }
      return( os );
    }
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
  }

  template< class O >
  inline O &Vector< bool >::PrintDimensions( O &os ) const {
    try {
      os << _size;
      return( os );
    }
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
  }

  inline bool Vector< bool >::Has( bool elm ) const {
    try {
      for( size_t idx = 0; idx < _size; ++idx ) {
        if( _data[ idx ] == elm )
          return( true );
      }
      return( false );
    }
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
  }

  /* Functions *********************************************************************************************************/

  template< class D >
  bool operator==( const Vector< D > &lhs, const Vector< D > &rhs ) {
    try {
      if( lhs._size != rhs._size ) {
        return( false );
      }
      for( size_t elm = 0; elm < lhs._size; ++elm ) {
        if( lhs[ elm ] != rhs[ elm ] ) {
          return( false );
        }
      }
      return( true );
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
  bool operator!=( const Vector< D > &lhs, const Vector< D > &rhs ) {
    try {
      if( lhs.size( ) != rhs.size( ) ) {
        return( true );
      }
      for( size_t elm = 0; elm < lhs.size( ); ++elm ) {
        if( lhs[ elm ] != rhs[ elm ] ) {
          return( true );
        }
      }
      return( false );
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
  bool operator<( const Vector< D > &lhs, const Vector< D > &rhs ) {
    try {
      if( lhs.size( ) != rhs.size( ) ) {
        return( false );
      }
      for( size_t elm = 0; elm < lhs.size( ); ++elm ) {
        if( lhs[ elm ] >= rhs[ elm ] ) {
          return( false );
        }
      }
      return( true );
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
  bool operator<=( const Vector< D > &lhs, const Vector< D > &rhs ) {
    try {
      if( lhs.size( ) != rhs.size( ) ) {
        return( false );
      }
      for( size_t elm = 0; elm < lhs.size( ); ++elm ) {
        if( lhs[ elm ] > rhs[ elm ] ) {
          return( false );
        }
      }
      return( true );
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
  bool operator>( const Vector< D > &lhs, const Vector< D > &rhs ) {
    try {
      if( lhs.size( ) != rhs.size( ) ) {
        return( false );
      }
      for( size_t elm = 0; elm < lhs.size( ); ++elm ) {
        if( lhs[ elm ] <= rhs[ elm ] ) {
          return( false );
        }
      }
      return( true );
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
  bool operator>=( const Vector< D > &lhs, const Vector< D > &rhs ) {
    try {
      if( lhs.size( ) != rhs.size( ) ) {
        return( false );
      }
      for( size_t elm = 0; elm < lhs.size( ); ++elm ) {
        if( lhs[ elm ] < rhs[ elm ] ) {
          return( false );
        }
      }
      return( true );
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
  void swap( Vector< D > &x, Vector< D > &y ) {
    try {
      std::swap( x._data, y._data );
      std::swap( x.qk_data, y.qk_data );
      std::swap( x._size, y._size );
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

}
#endif
