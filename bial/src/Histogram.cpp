/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/11 
 * @brief Creating histograms from data.
 */

#ifndef BIALHISTOGRAM_C
#define BIALHISTOGRAM_C

#include "Histogram.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_Histogram )
#define BIAL_EXPLICIT_Histogram
#endif

#if defined ( BIAL_EXPLICIT_Histogram ) || ( BIAL_IMPLICIT_BIN )

#include "Signal.hpp"
#include "Image.hpp"

namespace Bial {

  template< template< class D > class C, class D >
  Signal SignalType::Histogram( const C< D > &data, double data_step ) {
    try {
      if( data.size( ) == 0 ) {
        std::string msg( BIAL_ERROR( "Empty container to build the histogram." ) );
        throw( std::logic_error( msg ) );
      }
      double min = data.Minimum( );
      double max = data.Maximum( );
      size_t size = static_cast< size_t >( ( max - min ) / data_step ) + 1;
      Signal histogram( size, min, data_step );
      for( size_t it = 0; it < data.Size( ); ++it ) {
        histogram[ static_cast< size_t >( ( data( it ) - min ) / data_step ) ] += 1.0;
      }
      return( histogram );
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
  Signal SignalType::ZeroStartHistogram( const C< D > &data, double data_step ) {
    try {
      if( data.size( ) == 0 ) {
        std::string msg( BIAL_ERROR( "Empty container to build the histogram." ) );
        throw( std::logic_error( msg ) );
      }
      double min = std::min< double >( data.Minimum( ), 0.0 );
      double max = data.Maximum( );
      size_t size = static_cast< size_t >( ( max - min ) / data_step ) + 1;
      Signal histogram( size, min, data_step );
      for( size_t it = 0; it < data.Size( ); ++it ) {
        histogram[ static_cast< size_t >( ( data( it ) - min ) / data_step ) ] += 1.0;
      }
      return( histogram );
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
  Signal SignalType::FuzzyHistogram( const C< D > &data, double data_step ) {
    try {
      if( data.size( ) == 0 ) {
        std::string msg( BIAL_ERROR( "Empty container to build the histogram." ) );
        throw( std::logic_error( msg ) );
      }
      double min = data.Minimum( );
      double max = data.Maximum( );
      size_t size = static_cast< size_t >( ( max - min ) / data_step ) + 1;
      Signal histogram( size, min, data_step );
      for( size_t pxl = 0; pxl < data.size( ); ++pxl ) {
        /* Adding 0.5 to previous and postoterior bins of the histogram, and 1 to actual bin. */
        if( data( pxl ) - min != 0.0 ) {
          histogram[ static_cast< size_t >( ( data( pxl ) - min - 1 ) / data_step ) ] += 0.5;
        }
        histogram[ static_cast< size_t >( ( data( pxl ) - min ) / data_step ) ] += 1.0;
        if( data( pxl ) - min < size ) {
          histogram[ static_cast< size_t >( ( data( pxl ) - min + 1 ) / data_step ) ] += 0.5;
        }
      }
      return( histogram );
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

#ifdef BIAL_EXPLICIT_Histogram

  template Signal SignalType::Histogram( const Image< int > &data, double data_step );
  template Signal SignalType::ZeroStartHistogram( const Image< int > &data, double data_step );
  template Signal SignalType::FuzzyHistogram( const Image< int > &data, double data_step );
  template Signal SignalType::Histogram( const Image< llint > &data, double data_step );
  template Signal SignalType::ZeroStartHistogram( const Image< llint > &data, double data_step );
  template Signal SignalType::FuzzyHistogram( const Image< llint > &data, double data_step );
  template Signal SignalType::Histogram( const Image< float > &data, double data_step );
  template Signal SignalType::ZeroStartHistogram( const Image< float > &data, double data_step );
  template Signal SignalType::FuzzyHistogram( const Image< float > &data, double data_step );
  template Signal SignalType::Histogram( const Image< double > &data, double data_step );
  template Signal SignalType::ZeroStartHistogram( const Image< double > &data, double data_step );
  template Signal SignalType::FuzzyHistogram( const Image< double > &data, double data_step );

#endif

}

#endif

#endif
