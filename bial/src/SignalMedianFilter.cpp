/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/11 
 * @brief Computes median filter over the signal.
 */

#ifndef BIALSIGNALMEDIANFILTER_C
#define BIALSIGNALMEDIANFILTER_C

#include "SignalMedianFilter.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_SignalMedianFilter )
#define BIAL_EXPLICIT_SignalMedianFilter
#endif

#if defined ( BIAL_EXPLICIT_SignalMedianFilter ) || ( BIAL_IMPLICIT_BIN )

#include "RandomQueue.hpp"
#include "Signal.hpp"

namespace Bial {

  void SignalOp::MedianFilter( Signal &sgn, size_t kernel_size ) {
    try {
      if( sgn.size( ) >= kernel_size ) {
        COMMENT( "Fast median filter. Data size: " << sgn.size( ), 1 );
        RandomQueue adj( kernel_size );
        size_t p = 0;
        COMMENT( "Inserting first half of the kernel.", 1 );
        for( ; p < kernel_size / 2; ++p ) {
          COMMENT( "Inserting element " << p, 2 );
          adj.Insert( sgn[ p ] );
        }
        COMMENT( "Inserting until _data is finished.", 1 );
        for( ; p < sgn.size( ); ++p ) {
          sgn[ p - kernel_size / 2 ] = adj.Median( );
          adj.Insert( sgn[ p ] );
        }
        COMMENT( "Removing last half of the kernel.", 1 );
        for( ; p < sgn.size( ) + kernel_size / 2; ++p ) {
          sgn[ p - kernel_size / 2 ] = adj.Median( );
          adj.Remove( );
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

  Signal SignalOp::MedianFilter( const Signal &sgn, size_t kernel_size ) {
    try {
      Signal median( sgn );
      MedianFilter( median, kernel_size );
      return( median );
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

#ifdef BIAL_EXPLICIT_SignalMedianFilter

#endif

}

#endif

#endif
