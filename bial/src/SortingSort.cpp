/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2013/Sep/04
 * @brief Sorting algorithm that outputs the order of the original indexes after sorting.
 */

#ifndef BIALSORTINGSORT_C
#define BIALSORTINGSORT_C

#include "SortingSort.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_SortingSort )
#define BIAL_EXPLICIT_SortingSort
#endif
#if defined ( BIAL_EXPLICIT_SortingSort ) || ( BIAL_IMPLICIT_BIN )

#include "Image.hpp"

namespace Bial {

  template< template< class D > class C, class D >
  void Sorting::Sort( C< D > &data, Vector< size_t > &order, size_t min_idx, size_t max_idx, bool increasing ) {
    try {
      COMMENT( "min_idx: " << min_idx << ", max_idx: " << max_idx, 2 );
      if( min_idx < max_idx ) {
        COMMENT( "mid index for iqual elements.", 2 );
        size_t min = min_idx;
        size_t max = max_idx;
        size_t mid = min_idx;
        COMMENT( "random index to avoid bad pivots.", 2 );
        D pivot = data( ( std::rand( ) % ( max - min + 1 ) ) + min );
        if( increasing == true ) {
          COMMENT( "Sorting in increasing order.", 2 );
          COMMENT( "increasing. pivot:" << pivot, 2 );
          while( mid <= max ) {
            COMMENT( "min:" << min << " mid:" << mid << " max:" << max << " pivot:" << pivot << " vec:" << data, 4 );
            if( data( mid ) < pivot ) {
              std::swap( data( min ), data( mid ) );
              std::swap( order( min ), order( mid ) );
              ++min;
              ++mid;
            }
            else if( data( mid ) > pivot ) {
              std::swap( data( mid ), data( max ) );
              std::swap( order( mid ), order( max ) );
              --max;
            }
            else {
              ++mid;
            }
          }
        }
        else {
          COMMENT( "Sorting in decreasing order.", 2 );
          COMMENT( "decreasing. pivot:" << pivot, 2 );
          while( mid <= max ) {
            COMMENT( "min: " << min << "(" << data( min ) << "), mid: " << mid << "(" << data( mid ) << "), max: " <<
                     max << "(" << data( max ) << ")", 4 );
            if( data( mid ) > pivot ) {
              std::swap( data( min ), data( mid ) );
              std::swap( order( min ), order( mid ) );
              ++min;
              ++mid;
            }
            else if( data( mid ) < pivot ) {
              std::swap( data( mid ), data( max ) );
              std::swap( order( mid ), order( max ) );
              --max;
            }
            else {
              ++mid;
            }
          }
        }
        COMMENT( "Sorting recursively the parts of the container.", 2 );
        if( min > min_idx + 1 ) {
          Sort( data, order, min_idx, min - 1, increasing );
        }
        if( max < max_idx - 1 ) {
          Sort( data, order, max + 1, max_idx, increasing );
        }
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

  template< template< class D > class C, class D >
  Vector< size_t > Sorting::Sort( C< D > &data, bool increasing ) {
    try {
      COMMENT( "Creating order vector.", 2 );
      Vector< size_t > order( data.size( ) );
      for( size_t idx = 0; idx < order.size( ); ++idx ) {
        order( idx ) = idx;
      }
      COMMENT( "Sorting.", 2 );
      Sorting::Sort( data, order, 0, data.size( ) - 1, increasing );
      return( order );
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
  void Sorting::Sort( C< D > &data, const Vector< size_t > &order ) {
    try {
      if( order.size( ) != data.size( ) ) {
        std::string msg( BIAL_ERROR( "Data and order sizes do not match." ) );
        throw( std::logic_error( msg ) );
      }
      C< D > temporary( data );
      for( size_t elm = 0; elm < data.size( ); ++elm ) {
        data( elm ) = temporary( order( elm ) );
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

#ifdef BIAL_EXPLICIT_SortingSort

  template void Sorting::Sort( Vector< char > &data, Vector< size_t > &order, size_t min_idx, size_t max_idx,
                               bool increasing );
  template Vector< size_t > Sorting::Sort( Vector< char > &data, bool increasing );
  template void Sorting::Sort( Vector< char > &data, const Vector< size_t > &order );
  template void Sorting::Sort( Vector< uchar > &data, Vector< size_t > &order, size_t min_idx, size_t max_idx,
                               bool increasing );
  template Vector< size_t > Sorting::Sort( Vector< uchar > &data, bool increasing );
  template void Sorting::Sort( Vector< uchar > &data, const Vector< size_t > &order );
  template void Sorting::Sort( Vector< int > &data, Vector< size_t > &order, size_t min_idx, size_t max_idx,
                               bool increasing );
  template Vector< size_t > Sorting::Sort( Vector< int > &data, bool increasing );
  template void Sorting::Sort( Vector< int > &data, const Vector< size_t > &order );
  template void Sorting::Sort( Vector< uint > &data, Vector< size_t > &order, size_t min_idx, size_t max_idx,
                               bool increasing );
  template Vector< size_t > Sorting::Sort( Vector< uint > &data, bool increasing );
  template void Sorting::Sort( Vector< uint > &data, const Vector< size_t > &order );
  template void Sorting::Sort( Vector< ulint > &data, Vector< size_t > &order, size_t min_idx, size_t max_idx,
                               bool increasing );
  template Vector< size_t > Sorting::Sort( Vector< ulint > &data, bool increasing );
  template void Sorting::Sort( Vector< ulint > &data, const Vector< size_t > &order );
  template void Sorting::Sort( Vector< llint > &data, Vector< size_t > &order, size_t min_idx, size_t max_idx,
                               bool increasing );
  template Vector< size_t > Sorting::Sort( Vector< llint > &data, bool increasing );
  template void Sorting::Sort( Vector< llint > &data, const Vector< size_t > &order );
  template void Sorting::Sort( Vector< float > &data, Vector< size_t > &order, size_t min_idx, size_t max_idx,
                               bool increasing );
  template Vector< size_t > Sorting::Sort( Vector< float > &data, bool increasing );
  template void Sorting::Sort( Vector< float > &data, const Vector< size_t > &order );
  template void Sorting::Sort( Vector< double > &data, Vector< size_t > &order, size_t min_idx, size_t max_idx,
                               bool increasing );
  template Vector< size_t > Sorting::Sort( Vector< double > &data, bool increasing );
  template void Sorting::Sort( Vector< double > &data, const Vector< size_t > &order );

  template void Sorting::Sort( Image< int > &data, Vector< size_t > &order, size_t min_idx, size_t max_idx, 
                               bool increasing );
  template Vector< size_t > Sorting::Sort( Image< int > &data, bool increasing );
  template void Sorting::Sort( Image< int > &data, const Vector< size_t > &order );
  template void Sorting::Sort( Image< llint > &data, Vector< size_t > &order, size_t min_idx, size_t max_idx, 
                               bool increasing );
  template Vector< size_t > Sorting::Sort( Image< llint > &data, bool increasing );
  template void Sorting::Sort( Image< llint > &data, const Vector< size_t > &order );
  template void Sorting::Sort( Image< float > &data, Vector< size_t > &order, size_t min_idx, size_t max_idx, 
                               bool increasing );
  template Vector< size_t > Sorting::Sort( Image< float > &data, bool increasing );
  template void Sorting::Sort( Image< float > &data, const Vector< size_t > &order );
  template void Sorting::Sort( Image< double > &data, Vector< size_t > &order, size_t min_idx, size_t max_idx, 
                               bool increasing );
  template Vector< size_t > Sorting::Sort( Image< double > &data, bool increasing );
  template void Sorting::Sort( Image< double > &data, const Vector< size_t > &order );

#endif

}

#endif

#endif

