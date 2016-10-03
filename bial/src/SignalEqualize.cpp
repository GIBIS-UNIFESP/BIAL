/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/11 
 * @brief Signal equalization functions.
 */

#ifndef BIALSIGNALEQUALIZE_C
#define BIALSIGNALEQUALIZE_C

#include "SignalEqualize.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_SignalEqualize )
#define BIAL_EXPLICIT_SignalEqualize
#endif

#if defined ( BIAL_EXPLICIT_SignalEqualize ) || ( BIAL_IMPLICIT_BIN )

#include "Signal.hpp"
#include "SignalAccumulate.hpp"

namespace Bial {

  void SignalOp::Equalize( Signal &sgn ) {
    try {
      SignalOp::Accumulate( sgn );
      double factor = ( sgn.size( ) - 1 ) / sgn[ sgn.size( ) - 1 ];
      for( size_t bin = 0; bin < sgn.size( ); ++bin ) {
        sgn[ bin ] *= factor;
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

  Signal SignalOp::Equalize( const Signal &sgn ) {
    try {
      Signal equa( sgn );
      Equalize( equa );
      return( equa );
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

  void SignalOp::SegmentEqualize( Signal &sgn ) {
    /*
     * try {
     * // Computing an arraw containing the histogram intensities that contain at last one data.
     * Vector< size_t > valid_bin;
     * for( size_t bin = 0; bin < sgn.size( ); ++bin ) {
     *   if( sgn[ bin ] > 0 )
     *  valid_bin.push_back( bin );
     * }
     */

    /*
     * // Getting median bin
     * size_t valid_bins = valid_bin.size( );
     * size_t median_bin = valid_bin( valid_bins / 2 );
     * if( valid_bins % 2 == 0 )
     *   median_bin = ( median_bin + valid_bin( valid_bins / 2 + 1 ) ) / 2;
     */

    /* Parei aqui. */

    /* /home/fabio/experimentos/DOC/DOC.c:      WriteCurve2Gnuplot( c, file ); */


    /*
     * Accumulate( );
     * double factor = ( _data.size( ) - 1 ) / frequency( _data.size( ) - 1 );
     * for( size_t bin = 0; bin < _data.size( ); ++bin )
     *   frequency( bin ) *= factor;
     * }
     *   catch( std::bad_alloc &e ) {
     *     std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
     *     throw( std::runtime_error( msg ) );
     *   }
     *   catch( std::runtime_error &e ) {
     *     std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
     *     throw( std::runtime_error( msg ) );
     *   }    //   catch( const std::out_of_range& e ) {
     *     std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
     *     throw( std::out_of_range( msg ) );
     *   }
     *   catch( const std::logic_error& e ) {
     *     std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
     *     throw( std::logic_error( msg ) );
     *   }
     */
  }

  Signal SignalOp::SegmentEqualize( const Signal &sgn ) {
    try {
      Signal equa( sgn );
      SegmentEqualize( equa );
      return( equa );
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

#ifdef BIAL_EXPLICIT_SignalEqualize

#endif

}

#endif

#endif
