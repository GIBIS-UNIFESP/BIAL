/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Sep/04
 * @brief binary search in sorted vector.
 */

#ifndef BIALSORTINGBINARYSEARCH_C
#define BIALSORTINGBINARYSEARCH_C

#include "SortingBinarySearch.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_SortingBinarySearch )
#define BIAL_EXPLICIT_SortingBinarySearch
#endif
#if defined ( BIAL_EXPLICIT_SortingBinarySearch ) || ( BIAL_IMPLICIT_BIN )

#include "Image.hpp"

namespace Bial {

  template< template< class D > class C, class D >
  size_t Sorting::BinarySearch( const C< D > &data, D val, size_t min ) {
    try {
      return( Sorting::BinarySearch( data, val, min, data.size( ) - 1 ) );
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

  template< template< class D > class C, class D >
  size_t Sorting::BinarySearch( const C< D > &data, D val, size_t min, size_t max ) {
    try {
      COMMENT( "Adjust incorrect inputs.", 2 );
      if( min >= data.size( ) ) {
        std::string msg( BIAL_ERROR( "Minimum index out of data range." ) );
        throw( std::logic_error( msg ) );
      }
      if( max >= data.size( ) ) {
        max = data.size( ) - 1;
      }
      COMMENT( "Searching.", 2 );
      int i_max = static_cast< int >( max );
      int i_min = static_cast< int >( min );
      while( i_min <= i_max ) {
        int pos = ( i_max + i_min ) / 2;
        if( data( pos ) == val ) {
          return( pos );
        }
        else if( val > data( pos ) ) {
          i_min = pos + 1;
        }
        else {
          i_max = pos - 1;
        }
      }
      COMMENT( "Not found.", 2 );
      return( std::min( static_cast< size_t >( std::max( i_min, i_max ) ), max ) );
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

#ifdef BIAL_EXPLICIT_SortingBinarySearch

  template size_t Sorting::BinarySearch( const Vector< char > &data, char val, size_t min );
  template size_t Sorting::BinarySearch( const Vector< char > &data, char val, size_t min, size_t max );
  template size_t Sorting::BinarySearch( const Vector< uchar > &data, uchar val, size_t min );
  template size_t Sorting::BinarySearch( const Vector< uchar > &data, uchar val, size_t min, size_t max );
  template size_t Sorting::BinarySearch( const Vector< int > &data, int val, size_t min );
  template size_t Sorting::BinarySearch( const Vector< int > &data, int val, size_t min, size_t max );
  template size_t Sorting::BinarySearch( const Vector< uint > &data, uint val, size_t min );
  template size_t Sorting::BinarySearch( const Vector< uint > &data, uint val, size_t min, size_t max );
  template size_t Sorting::BinarySearch( const Vector< ulint > &data, ulint val, size_t min );
  template size_t Sorting::BinarySearch( const Vector< ulint > &data, ulint val, size_t min, size_t max );
  template size_t Sorting::BinarySearch( const Vector< llint > &data, llint val, size_t min );
  template size_t Sorting::BinarySearch( const Vector< llint > &data, llint val, size_t min, size_t max );
  template size_t Sorting::BinarySearch( const Vector< float > &data, float val, size_t min );
  template size_t Sorting::BinarySearch( const Vector< float > &data, float val, size_t min, size_t max );
  template size_t Sorting::BinarySearch( const Vector< double > &data, double val, size_t min );
  template size_t Sorting::BinarySearch( const Vector< double > &data, double val, size_t min, size_t max );

  template size_t Sorting::BinarySearch( const Image< int > &data, int val, size_t min );
  template size_t Sorting::BinarySearch( const Image< int > &data, int val, size_t min, size_t max );
  template size_t Sorting::BinarySearch( const Image< llint > &data, llint val, size_t min );
  template size_t Sorting::BinarySearch( const Image< llint > &data, llint val, size_t min, size_t max );
  template size_t Sorting::BinarySearch( const Image< float > &data, float val, size_t min );
  template size_t Sorting::BinarySearch( const Image< float > &data, float val, size_t min, size_t max );
  template size_t Sorting::BinarySearch( const Image< double > &data, double val, size_t min );
  template size_t Sorting::BinarySearch( const Image< double > &data, double val, size_t min, size_t max );

#endif

}

#endif

#endif

