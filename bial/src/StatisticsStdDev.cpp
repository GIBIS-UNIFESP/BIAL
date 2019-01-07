/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Dec/04
 * @brief Intensity standard deviation
 */

#ifndef BIALSTATISTICSSTDDEV_C
#define BIALSTATISTICSSTDDEV_C

#include "StatisticsStdDev.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_StatisticsStdDev )
#define BIAL_EXPLICIT_StatisticsStdDev
#endif
#if defined ( BIAL_EXPLICIT_StatisticsStdDev ) || ( BIAL_IMPLICIT_BIN )

#include "Image.hpp"
#include "StatisticsAverage.hpp"

namespace Bial {

  template< template< class D > class C, class D >
  inline float Statistics::StandardDeviation( const C< D > &data ) {
    try {
      float average = Statistics::Average( data, 0, data.size( ) );
      return( Statistics::StandardDeviation( data, 0, data.size( ), average ) );
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
  inline float Statistics::StandardDeviation( const C< D > &data, size_t first, size_t size ) {
    try {
      if( first + size > data.size( ) ) {
        std::string msg( BIAL_ERROR( "Maximum element index is greater than data size. Given data size: " +
                                     std::to_string( data.size( ) ) + ", max element index: " +
                                     std::to_string( first + size ) ) );
        throw( std::logic_error( msg ) );
      }
      float average = Statistics::Average( data, first, size );
      return( Statistics::StandardDeviation( data, first, size, average ) );
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
  inline float Statistics::StandardDeviation( const C< D > &data, float average ) {
    try {
      return( Statistics::StandardDeviation( data, average, 0, data.size( ) ) );
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
  float Statistics::StandardDeviation( const C< D > &data, size_t first, size_t size, float average ) {
    try {
      if( first + size > data.size( ) ) {
        std::string msg( BIAL_ERROR( "Maximum element index is greater than data size. Given data size: " +
                                     std::to_string( data.size( ) ) + ", max element index: " +
                                     std::to_string( first + size ) ) );
        throw( std::logic_error( msg ) );
      }
      float sum = 0.0;
      size_t efec_size = 0.0;
      for( size_t elm = 0; elm < size; ++elm ) {
        if( !std::isnan( data( first + elm ) ) ) {
          sum += ( data( first + elm ) - average ) * ( data( first + elm ) - average );
          ++efec_size;
        }
      }
      return( std::sqrt( 1.0 / efec_size * sum ) );
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

#ifdef BIAL_EXPLICIT_StatisticsStdDev

  template float Statistics::StandardDeviation( const Image< int > &data );
  template float Statistics::StandardDeviation( const Image< int > &data, size_t first, size_t size );
  template float Statistics::StandardDeviation( const Image< int > &data, float average );
  template float Statistics::StandardDeviation( const Image< int > &data, size_t first, size_t size, float average );
  template float Statistics::StandardDeviation( const Image< llint > &data );
  template float Statistics::StandardDeviation( const Image< llint > &data, size_t first, size_t size );
  template float Statistics::StandardDeviation( const Image< llint > &data, float average );
  template float Statistics::StandardDeviation( const Image< llint > &data, size_t first, size_t size, float average );
  template float Statistics::StandardDeviation( const Image< float > &data );
  template float Statistics::StandardDeviation( const Image< float > &data, size_t first, size_t size );
  template float Statistics::StandardDeviation( const Image< float > &data, float average );
  template float Statistics::StandardDeviation( const Image< float > &data, size_t first, size_t size, float average );
  template float Statistics::StandardDeviation( const Image< double > &data );
  template float Statistics::StandardDeviation( const Image< double > &data, size_t first, size_t size );
  template float Statistics::StandardDeviation( const Image< double > &data, float average );
  template float Statistics::StandardDeviation( const Image< double > &data, size_t first, size_t size, float average );

  template float Statistics::StandardDeviation( const Vector< int > &data );
  template float Statistics::StandardDeviation( const Vector< int > &data, size_t first, size_t size );
  template float Statistics::StandardDeviation( const Vector< int > &data, float average );
  template float Statistics::StandardDeviation( const Vector< int > &data, size_t first, size_t size, float average );
  template float Statistics::StandardDeviation( const Vector< llint > &data );
  template float Statistics::StandardDeviation( const Vector< llint > &data, size_t first, size_t size );
  template float Statistics::StandardDeviation( const Vector< llint > &data, float average );
  template float Statistics::StandardDeviation( const Vector< llint > &data, size_t first, size_t size, float average );
  template float Statistics::StandardDeviation( const Vector< float > &data );
  template float Statistics::StandardDeviation( const Vector< float > &data, size_t first, size_t size );
  template float Statistics::StandardDeviation( const Vector< float > &data, float average );
  template float Statistics::StandardDeviation( const Vector< float > &data, size_t first, size_t size, float average );
  template float Statistics::StandardDeviation( const Vector< double > &data );
  template float Statistics::StandardDeviation( const Vector< double > &data, size_t first, size_t size );
  template float Statistics::StandardDeviation( const Vector< double > &data, float average );
  template float Statistics::StandardDeviation( const Vector< double > &data, size_t first, size_t size, float average );

#endif

}

#endif

#endif
