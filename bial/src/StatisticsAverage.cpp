/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Dec/04
 * @brief Average image intensity.
 */

#ifndef BIALSTATISTICSAVERAGE_C
#define BIALSTATISTICSAVERAGE_C

#include "StatisticsAverage.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_StatisticsAverage )
#define BIAL_EXPLICIT_StatisticsAverage
#endif
#if defined ( BIAL_EXPLICIT_StatisticsAverage ) || ( BIAL_IMPLICIT_BIN )

#include "Image.hpp"

namespace Bial {

  template< template< class D > class C, class D >
  inline float Statistics::Average( C< D > data ) {
    try {
      return( Statistics::Average( data, 0, data.size( ) ) );
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
  float Statistics::Average( C< D > data, size_t first, size_t size ) {
    try {
      if( first + size > data.size( ) ) {
        std::string msg( BIAL_ERROR( "Maximum element index is greater than data size. Given data size: " +
                                     std::to_string( data.size( ) ) + ", max element index: " +
                                     std::to_string( first + size ) ) );
        throw( std::logic_error( msg ) );
      }
      float sum = 0.0;
      size_t efec_size = 0;
      for( size_t elm = 0; elm < size; ++elm ) {
        if( !std::isnan( data( first + elm ) ) ) {
          sum += data( first + elm );
          ++efec_size;
        }
      }
      return( sum / efec_size );
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

#ifdef BIAL_EXPLICIT_StatisticsAverage

  template float Statistics::Average( Image< int > data );
  template float Statistics::Average( Image< int > data, size_t first, size_t size );
  template float Statistics::Average( Image< llint > data );
  template float Statistics::Average( Image< llint > data, size_t first, size_t size );
  template float Statistics::Average( Image< float > data );
  template float Statistics::Average( Image< float > data, size_t first, size_t size );
  template float Statistics::Average( Image< double > data );
  template float Statistics::Average( Image< double > data, size_t first, size_t size );

  template float Statistics::Average( Vector< int > data );
  template float Statistics::Average( Vector< int > data, size_t first, size_t size );
  template float Statistics::Average( Vector< llint > data );
  template float Statistics::Average( Vector< llint > data, size_t first, size_t size );
  template float Statistics::Average( Vector< float > data );
  template float Statistics::Average( Vector< float > data, size_t first, size_t size );
  template float Statistics::Average( Vector< double > data );
  template float Statistics::Average( Vector< double > data, size_t first, size_t size );

#endif

}

#endif

#endif
