/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Dec/04
 * @brief Median absolute deviation computation.
 */

#ifndef BIALSTATISTICSMAD_C
#define BIALSTATISTICSMAD_C

#include "StatisticsMAD.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_StatisticsMAD )
#define BIAL_EXPLICIT_StatisticsMAD
#endif
#if defined ( BIAL_EXPLICIT_StatisticsMAD ) || ( BIAL_IMPLICIT_BIN )

#include "Image.hpp"

namespace Bial {

  template< template< class D > class C, class D >
  D Statistics::MedianAbsoluteDeviation( const C< D > &data ) {
    try {
      C< D > a_copy( data );
      COMMENT( "Sorting copy of data of size: " << a_copy.size( ), 0 );
      COMMENT( "data: " << a_copy, 0 );
      std::sort( a_copy.begin( ), a_copy.end( ) );
      D median = a_copy( a_copy.Size( ) / 2 );
      COMMENT( "Computing difference from median.", 0 );
      for( size_t it = 0; it < a_copy.Size( ) / 2; ++it ) {
        a_copy( it ) = median - a_copy( it );
      }
      for( size_t it = a_copy.Size( ) / 2; it < a_copy.Size( ); ++it ) {
        a_copy( it ) -= median;
      }
      std::sort( a_copy.begin( ), a_copy.end( ) );
      COMMENT( "Returning MAD.", 3 );
      return( a_copy( a_copy.Size( ) / 2 ) );
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

#ifdef BIAL_EXPLICIT_StatisticsMAD

  template int Statistics::MedianAbsoluteDeviation( const Image< int > &data );
  template llint Statistics::MedianAbsoluteDeviation( const Image< llint > &data );
  template float Statistics::MedianAbsoluteDeviation( const Image< float > &data );
  template double Statistics::MedianAbsoluteDeviation( const Image< double > &data );

  template int Statistics::MedianAbsoluteDeviation( const Vector< int > &data );
  template llint Statistics::MedianAbsoluteDeviation( const Vector< llint > &data );
  template float Statistics::MedianAbsoluteDeviation( const Vector< float > &data );
  template double Statistics::MedianAbsoluteDeviation( const Vector< double > &data );

#endif

}

#endif

#endif
