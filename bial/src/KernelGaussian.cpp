/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Nov/19 
 * @brief Gaussian kernel generator.
 */

#ifndef BIALKERNELGAUSSIAN_C
#define BIALKERNELGAUSSIAN_C

#include "KernelGaussian.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_KernelGaussian )
#define BIAL_EXPLICIT_KernelGaussian
#endif
#if defined ( BIAL_EXPLICIT_KernelGaussian ) || ( BIAL_IMPLICIT_BIN )

namespace Bial {

  Kernel KernelType::Gaussian( size_t dimensions, float radius, float std_dev, float amplitude ) {
    try {
      if( radius <= 0.0f ) {
        std::string msg( BIAL_ERROR( "Non positive radius given." ) );
        throw( std::logic_error( msg ) );
      }
      if( std_dev <= 0.0f ) {
        std::string msg( BIAL_ERROR( "Non positive standard deviation." ) );
        throw( std::logic_error( msg ) );
      }
      if( amplitude < 0.0f ) {
        amplitude = 1.0f / std::pow( std::sqrt( 2 * M_PI ) * std_dev, dimensions );
      }
      /* amplitude = 1.0f / ( std_dev * std::sqrt( 2 * M_PI ) ); */
      COMMENT( "Computing number of adjacents.", 0 );
      size_t adjacents = 0;
      int discrete_radius = static_cast< int >( radius );
      Vector< int > delta( dimensions, discrete_radius ); /* Counter. */
      size_t box_size = std::pow( 2 * discrete_radius + 1, dimensions ); /* Size of outer box containing the kernel. */
      for( size_t elm = 0; elm < box_size; ++elm ) {
        COMMENT( "Updating coordinates.", 4 );
        for( size_t dms = 0; dms < dimensions; ++dms ) {
          if( delta( dms ) < static_cast< int >( discrete_radius ) ) {
            ++delta( dms );
            break;
          }
          delta( dms ) = -discrete_radius;
        }
        COMMENT( "Computing distance.", 4 );
        size_t distance = 0;
        for( size_t dms = 0; dms < dimensions; ++dms ) {
          distance += delta( dms ) * delta( dms );
        }
        COMMENT( "Verify if pixel is inside the hypersphere.", 4 );
        if( static_cast< float >( distance ) <= radius * radius ) {
          ++adjacents;
        }
      }
      COMMENT( "Setting kernel coefficients and displacements.", 0 );
      Kernel krn( dimensions, adjacents );
      for( size_t dms = 0; dms < dimensions; ++dms ) {
        delta( dms ) = discrete_radius;
      }
      size_t adj = 0;
      for( size_t elm = 0; elm < box_size; ++elm ) {
        COMMENT( "Updating coordinates.", 4 );
        for( size_t dms = 0; dms < dimensions; ++dms ) {
          if( delta( dms ) < static_cast< int >( discrete_radius ) ) {
            ++delta( dms );
            break;
          }
          else {
            delta( dms ) = -discrete_radius;
          }
        }
        COMMENT( "Computing distance.", 4 );
        size_t distance = 0;
        for( size_t dms = 0; dms < dimensions; ++dms ) {
          distance += delta( dms ) * delta( dms );
        }
        COMMENT( "Verify if pixel is inside the hypersphere.", 4 );
        if( static_cast< float >( distance ) <= radius * radius ) {
          COMMENT( "Assigning coefficients and displacements.", 4 );
          for( size_t dms = 0; dms < dimensions; ++dms ) {
            krn( adj, dms ) = delta( dms );
          }
          float value = amplitude * std::exp( -static_cast< float >( distance ) / ( 2.0f * std_dev * std_dev ) );
          krn.Value( adj, value );
          ++adj;
        }
      }
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

  Kernel KernelType::NormalizedGaussian( size_t dimensions, float radius, float std_dev ) {
    try {
      Kernel krn = KernelType::Gaussian( dimensions, radius, std_dev, 1.0f );
      krn.Normalize( );
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

#ifdef BIAL_EXPLICIT_KernelGaussian

#endif

}

#endif

#endif
