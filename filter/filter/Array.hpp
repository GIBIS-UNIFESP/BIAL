/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @file Array.hpp
 * @author Fábio Augusto Menocci Cappabianco 
 * @date 2015/Apr/01 
 * @version 1.0.00 
 * @brief
 * Content: Array class 
 * <br> Description: A array that calls almost all std::array functions (C++11 standard). 
 * <br> Future add-on's: Things that speedup processing. 
 */

#include "Common.hpp"
#ifndef BIALARRAY_H
#define BIALARRAY_H

namespace Bial {
  /**
   * @brief  A array that calls almost all std::array functions (C++11 standard). 
   */
  template< class D, size_t S >
  class Array {

    template< class D2, size_t S2 >
    friend class Array;

  protected:

    /** @brief data array */
    std::array< D, S > _data;

  public:

    /**
     * @date 2015/Apr/22 
     * @param many. 
     * @return none. 
     * @brief Basic Constructors. 
     * @warning none. 
     */
    Array( );
    Array( const D &val );
    Array( const Array< D, S > &x );
    template< class D2 > Array( const Array< D2, S > &src );
    template< class D2 > Array( const std::array< D2, S > &src );

    /**
     * @date 2015/Apr/22 
     * @param many. 
     * @return none. 
     * @brief Fill operator. 
     * @warning none. 
     */
    Array< D, S > &operator=( const Array< D, S > &x );
    template< class D2 >
    Array< D, S > &operator=( const Array< D2, S > &x );
    Array< D, S > &operator=( double val );

    /**
     * @date 2015/Apr/22 
     * @param many. 
     * @return none. 
     * @brief Arithmetic operators. 
     * @warning none. 
     */
    template< class D2 >
    Array< D, S > &operator+=( const Array< D2, S > &other );
    template< class D2 >
    Array< D, S > &operator-=( const Array< D2, S > &other );
    template< class D2 >
    Array< D, S > &operator*=( const Array< D2, S > &other );
    template< class D2 >
    Array< D, S > &operator/=( const Array< D2, S > &other );
    template< class D2 >
    Array< D, S > operator+( const Array< D2, S > &other ) const;
    template< class D2 >
    Array< D, S > operator-( const Array< D2, S > &other ) const;
    template< class D2 >
    Array< D, S > operator*( const Array< D2, S > &other ) const;
    template< class D2 >
    Array< D, S > operator/( const Array< D2, S > &other ) const;
    Array< D, S > &operator+=( const double scalar );
    Array< D, S > &operator-=( const double scalar );
    Array< D, S > &operator*=( const double scalar );
    Array< D, S > &operator/=( const double scalar );
    Array< D, S > operator+( const double scalar ) const;
    Array< D, S > operator-( const double scalar ) const;
    Array< D, S > operator*( const double scalar ) const;
    Array< D, S > operator/( const double scalar ) const;

    /**
     * @date 2015/Apr/22 
     * @param none. 
     * @return Nothing. 
     * @brief Computes the absolute value of array. 
     * @warning none. 
     */
    void Abs( );

    /**
     * @date 2015/Apr/22 
     * @param none. 
     * @return Image with absolute values of array. 
     * @brief Computes the absolute value of array. 
     * @warning none. 
     */
    Array< D, S > Abs( ) const;

    /**
     * @date 2015/Apr/22 
     * @param many. 
     * @return none. 
     * @brief Iterators. 
     * @warning none. 
     */
    typename std::array< D, S >::iterator begin( ) noexcept;
    typename std::array< D, S >::const_iterator begin( ) const noexcept;
    typename std::array< D, S >::iterator end( ) noexcept;
    typename std::array< D, S >::const_iterator end( ) const noexcept;
    typename std::array< D, S >::reverse_iterator rbegin( ) noexcept;
    typename std::array< D, S >::const_reverse_iterator rbegin( ) const noexcept;
    typename std::array< D, S >::reverse_iterator rend( ) noexcept;
    typename std::array< D, S >::const_reverse_iterator rend( ) const noexcept;
    typename std::array< D, S >::const_iterator cbegin( ) const noexcept;
    typename std::array< D, S >::const_iterator cend( ) const noexcept;
    typename std::array< D, S >::const_reverse_iterator crbegin( ) const noexcept;
    typename std::array< D, S >::const_reverse_iterator crend( ) const noexcept;

    /**
     * @date 2015/Apr/22 
     * @param none. 
     * @return Array size. 
     * @brief Returns array size. 
     * @warning none. 
     */
    size_t size( ) const noexcept;
    size_t Size( ) const noexcept;

    /**
     * @date 2015/Apr/22 
     * @param none. 
     * @return true if array is empty. 
     * @brief Tests if array is empty. 
     * @warning none. 
     */
    bool empty( ) const noexcept;

    /**
     * @date 2015/Apr/22 
     * @param n: a array index. 
     * @return Reference to data at position n. 
     * @brief Returns reference to data at position n. 
     * @warning none. 
     */
    D &operator[]( size_t n );
    const D &operator[]( size_t n ) const;

    /**
     * @date 2015/Apr/22 
     * @param n: a array index. 
     * @return Reference to data at position n. 
     * @brief Returns reference to data at position n. 
     * @warning none. 
     */
    D &operator()( size_t n );
    const D &operator()( size_t n ) const;

    /**
     * @date 2015/Apr/22 
     * @param n: a array reference. 
     * @return Reference to data at position n. 
     * @brief Returns reference to data at position n, checking if it exists. 
     * @warning none. 
     */
    D &at( size_t n );
    const D &at( size_t n ) const;

    /**
     * @date 2015/Apr/22 
     * @param none. 
     * @return Array's first element. 
     * @brief Returns array's first element. 
     * @warning none. 
     */
    D &front( );
    const D &front( ) const;

    /**
     * @date 2015/Apr/22 
     * @param none. 
     * @return Array's last element. 
     * @brief Returns array's last element. 
     * @warning none. 
     */
    D &back( );
    const D &back( ) const;

    /**
     * @date 2015/Apr/22 
     * @param none. 
     * @return Pointer to _data array. 
     * @brief Returns pointer to _data array. 
     * @warning none. 
     */
    D* data( ) noexcept;
    const D* data( ) const noexcept;

    /**
     * @date 2015/Apr/22 
     * @param none. 
     * @return none. 
     * @brief Fills data to all elements. 
     * @warning none. 
     */
    void fill( const D &val );

    /**
     * @date 2015/Apr/22 
     * @param none. 
     * @return none. 
     * @brief Fills data to all elements. 
     * @warning none. 
     */
    void Set( double val );

    /**
     * @date 2015/Apr/22 
     * @param x: a array. 
     * @return none. 
     * @brief Exchanges content of this array by x. 
     * @warning none. 
     */
    void swap( Array &x );

    /**
     * @date 2015/Apr/22 
     * @param none. 
     * @return The maximum value among array elements. 
     * @brief Compute and return the maximum value among array elements. 
     * @warning none. 
     */
    D Maximum( ) const;

    /**
     * @date 2015/Apr/22 
     * @param none. 
     * @return The minimum value among array elements. 
     * @brief Compute and return the minimum value among array elements. 
     * @warning none. 
     */
    D Minimum( ) const;

    /**
     * @date 2015/Apr/22 
     * @param os: an output stream. 
     * @return The output stream. 
     * @brief Prints array containts to output stream os. 
     * @warning 'this' array must be 1D, 2D, or 3D. 
     */
    template< class O >
    O &Print( O &os ) const;

    /**
     * @date 2015/Apr/22 
     * @param none. 
     * @return The dimensions of the array. 
     * @brief Prints array dimensions to output stream os. 
     * @warning none. 
     */
    template< class O >
    O &PrintDimensions( O &os ) const;
  };

  /**
   * @date 2015/Apr/22 
   * @param lhs: left array. 
   * @param rhs: right array. 
   * @return Result of operator. 
   * @brief Operators. 
   * @warning none. 
   */
  template< class D, size_t S >
  bool operator==( const Array< D, S > &lhs, const Array< D, S > &rhs );
  template< class D, size_t S >
  bool operator!=( const Array< D, S > &lhs, const Array< D, S > &rhs );
  template< class D, size_t S >
  bool operator<( const Array< D, S > &lhs, const Array< D, S > &rhs );
  template< class D, size_t S >
  bool operator<=( const Array< D, S > &lhs, const Array< D, S > &rhs );
  template< class D, size_t S >
  bool operator>( const Array< D, S > &lhs, const Array< D, S > &rhs );
  template< class D, size_t S >
  bool operator>=( const Array< D, S > &lhs, const Array< D, S > &rhs );

  template< class D, size_t S >
  void swap( Array< D, S > &x, Array< D, S > &y );

  /**
   * @date 2015/Apr/22 
   * @param arr: an array. 
   * @return Reference to I'th element of the array. 
   * @brief Returns a reference to I'th element of the array. 
   * @warning none. 
   */
  template< size_t I, class D, size_t S >
  D &get( Array< D, S > &arr ) noexcept;
  template< size_t I, class D, size_t S >
  D && get( Array< D, S > && arr ) noexcept;
  template< size_t I, class D, size_t S >
  const D &get( const Array< D, S > &arr ) noexcept;

  /* @param os: output stream. 
   * @param mat: a array. 
   * @return Reference to os. 
   * @brief Prints the array in a friendly way. 
   * @warning none. 
   * template< typename D > O& operator<<( O& os, const Array< D, S >& mat ); 
   */
  template< class D, size_t S, class O >
  O &operator<<( O &os, const Array< D, S > &vec ) {
    vec.Print( os );
    return( os );
  }

}

/* Implementation ---------------------------------------------------------------------------------------------------- */

namespace Bial {

  /* Array ********************************************************************************************************** */

  template< class D, size_t S > Array< D, S >::Array( ) try : Array( static_cast< D >( 0 ) ) {
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

  template< class D, size_t S > Array< D, S >::Array( const D &val ) try : _data( ) {
    for( size_t elm = 0; elm < S; ++elm ) {
      _data[ elm ] = val;
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

  template< class D, size_t S > Array< D, S >::Array( const Array< D, S > &x ) try : _data( ) {
    for( size_t elm = 0; elm < S; ++elm ) {
      _data[ elm ] = x._data[ elm ];
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

  template< class D, size_t S >
  template< class D2 > Array< D, S >::Array( const std::array< D2, S > &src ) try : _data( ) {
    for( size_t elm = 0; elm < S; ++elm ) {
      _data[ elm ] = static_cast< D >( src[ elm ] );
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

  template< class D, size_t S >
  template< class D2 > Array< D, S >::Array( const Array< D2, S > &src ) try : Array( src._data ) {
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

  template< class D, size_t S >
  Array< D, S > &Array< D, S >::operator=( const Array< D, S > &x ) {
    try {
      _data = x._data;
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

  template< class D, size_t S >
  template< class D2 >
  Array< D, S > &Array< D, S >::operator=( const Array< D2, S > &x ) {
    try {
      for( size_t elm = 0; elm < S; ++elm ) {
        _data[ elm ] = static_cast< D >( x._data[ elm ] );
      }
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

  template< class D, size_t S >
  Array< D, S > &Array< D, S >::operator=( double val ) {
    try {
      for( size_t elm = 0; elm < S; ++elm ) {
        _data[ elm ] = static_cast< D >( val );
      }
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

  template< class D, size_t S >
  template< class D2 >
  inline Array< D, S > &Array< D, S >::operator+=( const Array< D2, S > &other ) {
    try {
      for( size_t elm = 0; elm < S; ++elm ) {
        _data[ elm ] += static_cast< D >( other._data[ elm ] );
      }
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

  template< class D, size_t S >
  template< class D2 >
  inline Array< D, S > &Array< D, S >::operator-=( const Array< D2, S > &other ) {
    try {
      for( size_t elm = 0; elm < S; ++elm ) {
        _data[ elm ] -= static_cast< D >( other._data[ elm ] );
      }
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

  template< class D, size_t S >
  template< class D2 >
  inline Array< D, S > &Array< D, S >::operator*=( const Array< D2, S > &other ) {
    try {
      for( size_t elm = 0; elm < S; ++elm ) {
        _data[ elm ] *= static_cast< D >( other._data[ elm ] );
      }
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

  template< class D, size_t S >
  template< class D2 >
  inline Array< D, S > &Array< D, S >::operator/=( const Array< D2, S > &other ) {
    try {
#ifdef BIAL_DEBUG
      COMMENT( "Performing division.", 2 );
      for( size_t elm = 0; elm < S; ++elm ) {
        if( other._data[ elm ] == 0 ) {
          std::string msg( BIAL_ERROR( "Division by zero at element " + std::to_string( elm ) + "." ) );
          throw( std::logic_error( msg ) );
        }
        _data[ elm ] /= static_cast< D >( other._data[ elm ] );
      }
#else
      COMMENT( "Performing division.", 2 );
      for( size_t elm = 0; elm < S; ++elm ) {
        _data[ elm ] /= static_cast< D >( other._data[ elm ] );
      }
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

  template< class D, size_t S >
  template< class D2 >
  inline Array< D, S > Array< D, S >::operator+( const Array< D2, S > &other ) const {
    try {
      Array< D, S > tgt( *this );
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

  template< class D, size_t S >
  template< class D2 >
  inline Array< D, S > Array< D, S >::operator-( const Array< D2, S > &other ) const {
    try {
      Array< D, S > tgt( *this );
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

  template< class D, size_t S >
  template< class D2 >
  inline Array< D, S > Array< D, S >::operator*( const Array< D2, S > &other ) const {
    try {
      Array< D, S > tgt( *this );
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

  template< class D, size_t S >
  template< class D2 >
  inline Array< D, S > Array< D, S >::operator/( const Array< D2, S > &other ) const {
    try {
      Array< D, S > tgt( *this );
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

  template< class D, size_t S >
  inline Array< D, S > &Array< D, S >::operator+=( const double scalar ) {
    try {
      for( size_t elm = 0; elm < S; ++elm ) {
        _data[ elm ] += static_cast< D >( scalar );
      }
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

  template< class D, size_t S >
  inline Array< D, S > &Array< D, S >::operator-=( const double scalar ) {
    try {
      for( size_t elm = 0; elm < S; ++elm ) {
        _data[ elm ] -= static_cast< D >( scalar );
      }
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

  template< class D, size_t S >
  inline Array< D, S > &Array< D, S >::operator*=( const double scalar ) {
    try {
      for( size_t elm = 0; elm < S; ++elm ) {
        _data[ elm ] *= static_cast< D >( scalar );
      }
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

  template< class D, size_t S >
  inline Array< D, S > &Array< D, S >::operator/=( const double scalar ) {
    try {
      COMMENT( "Verifying if scalar is not zero.", 2 );
      if( scalar == 0.0 ) {
        std::string msg( BIAL_ERROR( "Division by zero." ) );
        throw( std::logic_error( msg ) );
      }
      for( size_t elm = 0; elm < S; ++elm ) {
        _data[ elm ] /= static_cast< D >( scalar );
      }
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

  template< class D, size_t S >
  inline Array< D, S > Array< D, S >::operator+( const double scalar ) const {
    try {
      Array< D, S > tgt( *this );
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

  template< class D, size_t S >
  inline Array< D, S > Array< D, S >::operator-( const double scalar ) const {
    try {
      Array< D, S > tgt( *this );
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

  template< class D, size_t S >
  inline Array< D, S > Array< D, S >::operator*( const double scalar ) const {
    try {
      Array< D, S > tgt( *this );
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

  template< class D, size_t S >
  inline Array< D, S > Array< D, S >::operator/( const double scalar ) const {
    try {
      Array< D, S > tgt( *this );
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

  template< class D, size_t S >
  void Array< D, S >::Abs( ) {
    COMMENT( "Computing absolute value.", 2 );
    for( size_t elm = 0; elm < S; ++elm ) {
      _data[ elm ] = std::abs( _data[ elm ] );
    }
  }

  template< class D, size_t S >
  Array< D, S > Array< D, S >::Abs( ) const {
    try {
      Array< D, S > res( *this );
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

  template< class D, size_t S >
  typename std::array< D, S >::iterator Array< D, S >::begin( ) noexcept {
    return( _data.begin( ) );
  }

  template< class D, size_t S >
  typename std::array< D, S >::const_iterator Array< D, S >::begin( ) const noexcept {
    return( _data.begin( ) );
  }

  template< class D, size_t S >
  typename std::array< D, S >::iterator Array< D, S >::end( ) noexcept {
    return( _data.end( ) );
  }

  template< class D, size_t S >
  typename std::array< D, S >::const_iterator Array< D, S >::end( ) const noexcept {
    return( _data.end( ) );
  }

  template< class D, size_t S >
  typename std::array< D, S >::reverse_iterator Array< D, S >::rbegin( ) noexcept {
    return( _data.rbegin( ) );
  }

  template< class D, size_t S >
  typename std::array< D, S >::const_reverse_iterator Array< D, S >::rbegin( ) const noexcept {
    return( _data.rbegin( ) );
  }

  template< class D, size_t S >
  typename std::array< D, S >::reverse_iterator Array< D, S >::rend( ) noexcept {
    return( _data.rend( ) );
  }

  template< class D, size_t S >
  typename std::array< D, S >::const_reverse_iterator Array< D, S >::rend( ) const noexcept {
    return( _data.rend( ) );
  }

  template< class D, size_t S >
  typename std::array< D, S >::const_iterator Array< D, S >::cbegin( ) const noexcept {
    return( _data.cbegin( ) );
  }

  template< class D, size_t S >
  typename std::array< D, S >::const_iterator Array< D, S >::cend( ) const noexcept {
    return( _data.cend( ) );
  }

  template< class D, size_t S >
  typename std::array< D, S >::const_reverse_iterator Array< D, S >::crbegin( ) const noexcept {
    return( _data.crbegin( ) );
  }

  template< class D, size_t S >
  typename std::array< D, S >::const_reverse_iterator Array< D, S >::crend( ) const noexcept {
    return( _data.crend( ) );
  }

  template< class D, size_t S >
  size_t Array< D, S >::size( ) const noexcept {
    return( S );
  }

  template< class D, size_t S >
  size_t Array< D, S >::Size( ) const noexcept {
    return( S );
  }

  template< class D, size_t S >
  bool Array< D, S >::empty( ) const noexcept {
    return( _data.empty( ) );
  }

  template< class D, size_t S >
  D &Array< D, S >::operator[]( size_t n ) {
#ifdef BIAL_DEBUG
    if( n >= S ) {
      std::string msg( BIAL_ERROR( "Element out of range. Given: " + std::to_string( n ) + ", expected: < " + 
                                   std::to_string( S ) ) );
      throw( std::out_of_range( msg ) );
    }
#endif
    return( _data[ n ] );

  }

  template< class D, size_t S >
  const D &Array< D, S >::operator[]( size_t n ) const {
#ifdef BIAL_DEBUG
    if( n >= S ) {
      std::string msg( BIAL_ERROR( "Element out of range. Given: " + std::to_string( n ) + ", expected: < " + 
                                   std::to_string( S ) ) );
      throw( std::out_of_range( msg ) );
    }
#endif
    return( _data[ n ] );
  }

  template< class D, size_t S >
  D &Array< D, S >::operator()( size_t n ) {
#ifdef BIAL_DEBUG
    if( n >= S ) {
      std::string msg( BIAL_ERROR( "Element out of range. Given: " + std::to_string( n ) + ", expected: < " + 
                                   std::to_string( S ) ) );
      throw( std::out_of_range( msg ) );
    }
#endif
    return( _data[ n ] );
  }

  template< class D, size_t S >
  const D &Array< D, S >::operator()( size_t n ) const {
#ifdef BIAL_DEBUG
    if( n >= S ) {
      std::string msg( BIAL_ERROR( "Element out of range. Given: " + std::to_string( n ) + ", expected: < " + 
                                   std::to_string( S ) ) );
      throw( std::out_of_range( msg ) );
    }
#endif
    return( _data[ n ] );
  }

  template< class D, size_t S >
  D &Array< D, S >::at( size_t n ) {
    return( _data.at( n ) );
  }

  template< class D, size_t S >
  const D &Array< D, S >::at( size_t n ) const {
    return( _data.at( n ) );
  }

  template< class D, size_t S >
  D &Array< D, S >::front( ) {
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

  template< class D, size_t S >
  const D &Array< D, S >::front( ) const {
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

  template< class D, size_t S >
  D &Array< D, S >::back( ) {
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

  template< class D, size_t S >
  const D &Array< D, S >::back( ) const {
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

  template< class D, size_t S >
  D* Array< D, S >::data( ) noexcept {
    return( _data.data( ) );
  }

  template< class D, size_t S >
  const D* Array< D, S >::data( ) const noexcept {
    return( _data.data( ) );
  }

  template< class D, size_t S >
  void Array< D, S >::fill( const D &val ) {
    try {
      _data.fill( val );
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

  template< class D, size_t S >
  void Array< D, S >::Set( double val ) {
    try {
      for( size_t elm = 0; elm < S; ++elm ) {
        _data[ elm ] = static_cast< D >( val );
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
  }

  template< class D, size_t S >
  void Array< D, S >::swap( Array &x ) {
    try {
      _data.swap( x._data );
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

  template< class D, size_t S >
  bool operator==( const Array< D, S > &lhs, const Array< D, S > &rhs ) {
    try {
      return( lhs._data == rhs._data );
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

  template< class D, size_t S >
  bool operator!=( const Array< D, S > &lhs, const Array< D, S > &rhs ) {
    try {
      return( lhs._data != rhs._data );
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

  template< class D, size_t S >
  bool operator<( const Array< D, S > &lhs, const Array< D, S > &rhs ) {
    try {
      return( lhs._data <= rhs._data );
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

  template< class D, size_t S >
  bool operator<=( const Array< D, S > &lhs, const Array< D, S > &rhs ) {
    try {
      return( lhs._data <= rhs._data );
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

  template< class D, size_t S >
  bool operator>( const Array< D, S > &lhs, const Array< D, S > &rhs ) {
    try {
      return( lhs._data > rhs._data );
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

  template< class D, size_t S >
  bool operator>=( const Array< D, S > &lhs, const Array< D, S > &rhs ) {
    try {
      return( lhs._data >= rhs._data );
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

  template< class D, size_t S >
  D Array< D, S >::Maximum( ) const {
    try {
      return( *std::max_element( _data.begin( ), _data.end( ) ) );
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

  template< class D, size_t S >
  D Array< D, S >::Minimum( ) const {
    try {
      return( *std::min_element( _data.begin( ), _data.end( ) ) );
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

  template< class D, size_t S >
  template< class O >
  O &Array< D, S >::Print( O &os ) const {
    try {
      for( size_t i = 0; i < S; ++i ) {
        os << _data[ i ];
        if( i != _data.size( ) - 1 ) {
          os << " ";
        }
      }
      return( os );
    }
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
    ;
  }

  template< class D, size_t S >
  template< class O >
  O &Array< D, S >::PrintDimensions( O &os ) const {
    try {
      os << S;
      return( os );
    }
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
    ;
  }

  /* Functions *********************************************************************************************************
   *
   */

  template< size_t I, class D, size_t S >
  D &get( Array< D, S > &arr ) noexcept {
    return( get< I, D, S >( arr._data ) );
  }

  template< size_t I, class D, size_t S >
  D && get( Array< D, S > && arr ) noexcept {
    return( get< I, D, S >( arr._data ) );
  }

  template< size_t I, class D, size_t S >
  const D &get( const Array< D, S > &arr ) noexcept {
    return( get< I, D, S >( arr._data ) );
  }

  template< class D, size_t S >
  void swap( Array< D, S > &x, Array< D, S > &y ) {
    try {
      std::swap( x._data, y._data );
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
