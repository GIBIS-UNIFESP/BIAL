/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Nov/19 
 * @brief Gabor kernel generator.
 */

#ifndef BIALKERNELGABOR_C
#define BIALKERNELGABOR_C

#include "KernelGabor.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_KernelGabor )
#define BIAL_EXPLICIT_KernelGabor
#endif
#if defined ( BIAL_EXPLICIT_KernelGabor ) || ( BIAL_IMPLICIT_BIN )

#include "KernelRound.hpp"

namespace Bial {

  Kernel KernelType::Gabor( float sigma, size_t dimensions, size_t direction ) {
    try {
      if( direction >= dimensions ) {
        std::string msg( BIAL_ERROR( "Direction should be lower than the number of dimensions. Given direction: " +
                                     std::to_string( direction ) + ", dimensions: " + std::to_string( dimensions ) + 
                                     "." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Computing kernel size.", 2 );
      COMMENT( "Optical Flow Using Spatiotemporal Filters 1987 David Heeger.", 5 );
      COMMENT( "Formula: gabor = gabor_norm * std::exp( -( x * x + y * y + z * z ) / ( 2.0f * sigma * sigma ) );", 5 );
      float gabor_norm = 1.0f / ( std::sqrt( 2.0f ) * std::pow( M_PI, dimensions / 2.0f ) *
                                  std::pow( sigma, dimensions ) );
      COMMENT( "gabor_norm: " << gabor_norm, 1 );
      COMMENT( "Computing the maximum distance to cover elements with 5% of the maximum value element, " <<
               "which lies at distance 1 from the center.", 1 );
      float gabor_dist_1 = std::exp( -1 / ( 2.0f * sigma * sigma ) );
      float max_dist = sigma * std::sqrt( -2.0f * std::log( 0.05f * gabor_dist_1 ) );
      COMMENT( "max dist: " << max_dist, 1 );
      COMMENT( "Creating kernel.", 2 );
      Kernel krn( KernelType::HyperSpheric( max_dist, dimensions ) );
      COMMENT( "Computing kernel coefficients and displacements.", 2 );
      size_t adj_size = krn.size( );
      size_t dims = krn.Dims( );
      for( size_t elm = 0; elm < adj_size; ++elm ) {
        float dist = 0.0f;
        float signal = 1.0f;
        for( size_t dms = 0; dms < dims; ++dms ) {
          if( dms == direction ) {
            COMMENT( "Gradient direction.", 4 );
            if( krn( elm, dms ) < 0 ) {
              signal = -1.0f;
              dist += std::abs( krn( elm, dms ) );
            }
            else if( krn( elm, dms ) > 0 ) {
              signal = 1.0f;
              dist += std::abs( krn( elm, dms ) );
            }
            else {
              COMMENT( "Zero displacement.", 4 );
              dist = 0.0f;
              break;
            }
          }
          else {
            COMMENT( "Smoothing direction.", 4 );
            dist += std::abs( krn( elm, dms ) );
          }
        }
        if( dist == 0.0f )
          krn.Value( elm ) = 0.0f;
        else
          krn.Value( elm ) = signal * gabor_norm * std::exp( -( dist * dist ) / ( 2.0f * sigma * sigma ) );
        COMMENT( "Element: " << elm << ", distance: " << dist << ", coefficient: " << krn.Value( elm ), 4 );
      }
      COMMENT( "Final kernel: " << krn, 2 );
      return( krn );
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

  Kernel KernelType::NormalizedGabor( float sigma, size_t dimensions, size_t direction ) {
    try {
      Kernel krn = KernelType::Gabor( sigma, dimensions, direction );
      krn.SignalNormalize( );
      return( krn );
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

#ifdef BIAL_EXPLICIT_KernelGabor

#endif

}

#endif

#endif
