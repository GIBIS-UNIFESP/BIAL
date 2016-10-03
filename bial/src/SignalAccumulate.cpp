/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/11 
 * @brief Computes the accumulated signal.
 */

#ifndef BIALSIGNALACCUMULATE_C
#define BIALSIGNALACCUMULATE_C

#include "SignalAccumulate.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_SignalAccumulate )
#define BIAL_EXPLICIT_SignalAccumulate
#endif

#if defined ( BIAL_EXPLICIT_SignalAccumulate ) || ( BIAL_IMPLICIT_BIN )

#include "Signal.hpp"

namespace Bial {

  void SignalOp::Accumulate( Signal &sgn ) {
    try {
      for( size_t it = 1; it < sgn.size( ); ++it ) {
        sgn[ it ] += sgn[ it - 1 ];
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

  Signal SignalOp::Accumulate( const Signal &sgn ) {
    try {
      Signal acc( sgn );
      Accumulate( acc );
      return( acc );
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

  double SignalOp::AccumulatedFrequency( const Signal &sgn, size_t min_bin ) {
    if( sgn.size( ) > 0 ) {
      return( AccumulatedFrequency( sgn, min_bin, sgn.size( ) - 1 ) );
    }
    return( 0 );
  }

  double SignalOp::AccumulatedFrequency( const Signal &sgn, size_t min_bin, size_t max_bin ) {
    try {
      double sum = 0.0;
      for( size_t it = min_bin; it <= max_bin; ++it ) {
        sum += static_cast< double >( sgn[ it ] );
      }
      return( sum );
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

  double SignalOp::AccumulatedWeightedFrequency( const Signal &sgn, size_t min_bin ) {
    try {
      if( sgn.size( ) > 0 ) {
        return( AccumulatedWeightedFrequency( sgn, min_bin, sgn.size( ) - 1 ) );
      }
      return( 0 );
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

  double SignalOp::AccumulatedWeightedFrequency( const Signal &sgn, size_t min_bin, size_t max_bin ) {
    try {
      double sum = 0.0;
      for( size_t it = min_bin; it <= max_bin; ++it ) {
        sum += static_cast< double >( sgn[ it ] * sgn.Data( it ) );
      }
      return( sum );
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


#ifdef BIAL_EXPLICIT_SignalAccumulate

#endif

}

#endif

#endif
