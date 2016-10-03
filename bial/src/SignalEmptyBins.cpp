/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/11 
 * @brief Functions to deal with empty bins in a signal. Useful to locate key points.
 */

#ifndef BIALSIGNALEMPTYBINS_C
#define BIALSIGNALEMPTYBINS_C

#include "SignalEmptyBins.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_SignalEmptyBins )
#define BIAL_EXPLICIT_SignalEmptyBins
#endif

#if defined ( BIAL_EXPLICIT_SignalEmptyBins ) || ( BIAL_IMPLICIT_BIN )

#include "Signal.hpp"

namespace Bial {

  Signal SignalOp::RemoveEmptyBins( const Signal &sgn ) {
    try {
      size_t nonzeros = 0;
      size_t min = 0;
      for( size_t ths_it = 0; ths_it < sgn.size( ); ++ths_it ) {
        if( sgn[ ths_it ] != 0 ) {
          if( min == 0 ) {
            min = ths_it;
          }
          ++nonzeros;
        }
      }
      Signal result( nonzeros );
      size_t rst_it = 0;
      for( size_t ths_it = 0; ths_it < sgn.size( ); ++ths_it ) {
        if( sgn[ ths_it ] > 0 ) {
          result[ rst_it ] = sgn[ ths_it ];
          result.Data( rst_it ) = sgn.Data( ths_it );
          ++rst_it;
        }
      }
      return( result );
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

  Vector< size_t > SignalOp::MapBins( const Signal &source, const Signal &target ) {
    try {
      size_t src_it = 0;
      Vector< size_t > result( source.Size( ), 0 );
      for( size_t tgt_it = 0; ( src_it < source.size( ) ) && ( tgt_it < target.size( ) ); ++tgt_it ) {
        if( source.Data( src_it ) == ( target.Data( tgt_it ) ) ) {
          result[ src_it ] = tgt_it;
          ++src_it;
        }
      }
      return( result );
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

#ifdef BIAL_EXPLICIT_SignalEmptyBins

#endif

}

#endif

#endif
