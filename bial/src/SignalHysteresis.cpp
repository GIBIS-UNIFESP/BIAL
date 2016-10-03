/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/11 
 * @brief Functions to compute signal hysteresis
 */

#ifndef BIALSIGNALHYSTERESIS_C
#define BIALSIGNALHYSTERESIS_C

#include "SignalHysteresis.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_SignalHysteresis )
#define BIAL_EXPLICIT_SignalHysteresis
#endif

#if defined ( BIAL_EXPLICIT_SignalHysteresis ) || ( BIAL_IMPLICIT_BIN )

#include "Signal.hpp"
#include "SignalMedianFilter.hpp"

namespace Bial {

  double SignalOp::LowHysteresis( const Signal &sgn, size_t krl_size, float low_threshold, float hgh_threshold ) {
    try {
      if( ( low_threshold < 0.0 ) || ( low_threshold > hgh_threshold ) || ( hgh_threshold > 1.0 ) ) {
        std::string msg( BIAL_ERROR( "Invaid low and/or high threshold. Expected: 0.0 <= low_threshold " +
                                     "<= high_threshold <= 1.0. Given: low threshold: " +
                                     std::to_string( low_threshold ) + ", high threshold: " +
                                     std::to_string( hgh_threshold ) ) );
        throw( std::logic_error( msg ) );
      }
      Signal median( MedianFilter( sgn, krl_size ) );
      size_t elements = median.size( );
      /* Compute amplitude. */
      double max_freq = median.MaximumFrequency( );
      size_t clip_bin = elements - 2;
      if( elements > krl_size ) {
        clip_bin = 1;
        while( ( median[ clip_bin ] <= hgh_threshold * max_freq ) && ( clip_bin < elements - 1 ) )
          ++clip_bin;
        while( ( median[ clip_bin ] >= low_threshold * max_freq ) && ( clip_bin > 0 ) )
          --clip_bin;
      }
      else
        clip_bin = 0;
      /* std::cout << "Cut: " << sgn.Data( clip_bin ) << ", max bin: " << sgn.Data( elements - 1 ) << std::endl; */
      return( sgn.Data( clip_bin ) );
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

  double SignalOp::HighHysteresis( const Signal &sgn, size_t krl_size, float low_threshold, float hgh_threshold ) {
    try {
      if( ( low_threshold < 0.0 ) || ( low_threshold > hgh_threshold ) || ( hgh_threshold > 1.0 ) ) {
        std::string msg( BIAL_ERROR( "Invaid low and/or high threshold. Expected: 0.0 <= low_threshold <= " +
                                     "high_threshold <= 1.0. Given: low threshold: " +
                                     std::to_string( low_threshold ) + ", high threshold: " +
                                     std::to_string( hgh_threshold ) ) );
        throw( std::logic_error( msg ) );
      }
      Signal median( MedianFilter( sgn, krl_size ) );
      size_t elements = median.size( );
      /* Compute amplitude. */
      double max_freq = median.MaximumFrequency( );
      size_t clip_bin = elements - 2;
      if( elements > krl_size ) {
        clip_bin = elements - 2;
        while( ( median[ clip_bin ] <= hgh_threshold * max_freq ) && ( clip_bin > 0 ) )
          --clip_bin;
        while( ( median[ clip_bin ] >= low_threshold * max_freq ) && ( clip_bin < elements - 1 ) )
          ++clip_bin;
      }
      else
        clip_bin = elements - 1;
      /* std::cout << "Cut: " << sgn.Data( clip_bin ) << ", max bin: " << sgn.Data( elements - 1 ) << std::endl; */
      return( sgn.Data( clip_bin ) );
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

#ifdef BIAL_EXPLICIT_SignalHysteresis

#endif

}

#endif

#endif
