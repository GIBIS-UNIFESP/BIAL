/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/11 
 * @brief Signal normalization.
 */

#ifndef BIALSIGNALNORMALIZE_C
#define BIALSIGNALNORMALIZE_C

#include "SignalNormalize.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_SignalNormalize )
#define BIAL_EXPLICIT_SignalNormalize
#endif

#if defined ( BIAL_EXPLICIT_SignalNormalize ) || ( BIAL_IMPLICIT_BIN )

#include "RandomQueue.hpp"
#include "Signal.hpp"
#include "SignalAccumulate.hpp"

namespace Bial {

  void SignalOp::Normalize( Signal &sgn ) {
    try {
      size_t total_frequency = SignalOp::AccumulatedFrequency( sgn );
      for( size_t it = 0; it < sgn.size( ); ++it ) {
        sgn[ it ] /= total_frequency;
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

  Signal SignalOp::Normalize( const Signal &sgn ) {
    try {
      Signal norm( sgn );
      Normalize( norm );
      return( norm );
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

#ifdef BIAL_EXPLICIT_SignalNormalize


#endif

}

#endif

#endif
