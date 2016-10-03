/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/11 
 * @brief Computes otsu threshold of signals signal.
 */

#ifndef BIALSIGNALOTSUTHRESHOLD_C
#define BIALSIGNALOTSUTHRESHOLD_C

#include "SignalOtsuThreshold.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_SignalOtsuThreshold )
#define BIAL_EXPLICIT_SignalOtsuThreshold
#endif

#if defined ( BIAL_EXPLICIT_SignalOtsuThreshold ) || ( BIAL_IMPLICIT_BIN )

#include "Signal.hpp"
#include "SignalAccumulate.hpp"

namespace Bial {

  size_t SignalOp::OtsuThreshold( const Signal &sgn ) {
    try {
      if( sgn.size( ) <= 1 ) {
        return( 0 );
      }
      double total_accumulated_frequency = static_cast< double >( AccumulatedWeightedFrequency( sgn ) );
      double total_frequency = static_cast< double >( AccumulatedFrequency( sgn ) );
      double maximum_intraclass_variance = -1.0;
      size_t optimum_threshold = 0;
      double low_frequency = 0.0; /* Initializing frequencies. */
      double low_accumulated_frequency = 0.0; /* Initializing accumulated_frequencies. */
      for( size_t threshold = 0; threshold < sgn.size( ); ++threshold ) {
        low_frequency += static_cast< double >( sgn[ threshold ] ); /* Computing frequencies. */
        double high_frequency = total_frequency - low_frequency;
        double low_probability = low_frequency / total_frequency; /* Computing probabilities. */
        double high_probability = 1.0 - low_probability;
        /* Computing accum. frequencies. */
        low_accumulated_frequency += static_cast< double >( sgn[ threshold ] * sgn.Data( threshold ) );
        double high_accumulated_frequency = total_accumulated_frequency - low_accumulated_frequency;
        double low_mean = low_accumulated_frequency / low_frequency; /* Computing means. */
        double high_mean = high_accumulated_frequency / high_frequency;
        double intraclass_variance = ( low_probability * high_probability * ( low_mean - high_mean ) *
                                       ( low_mean - high_mean ) ); /* Computing intra-class variance. */
        if( intraclass_variance > maximum_intraclass_variance ) { /* Optimum threshold selection. */
          maximum_intraclass_variance = intraclass_variance;
          optimum_threshold = threshold;
        }
      }
      return( optimum_threshold );
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

#ifdef BIAL_EXPLICIT_SignalOtsuThreshold

#endif

}

#endif

#endif
