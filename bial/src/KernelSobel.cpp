/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Nov/19 
 * @brief Sobel kernel generator.
 */

#ifndef BIALKERNELSOBEL_C
#define BIALKERNELSOBEL_C

#include "KernelSobel.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_KernelSobel )
#define BIAL_EXPLICIT_KernelSobel
#endif
#if defined ( BIAL_EXPLICIT_KernelSobel ) || ( BIAL_IMPLICIT_BIN )

#include "KernelBox.hpp"

namespace Bial {

  Kernel KernelType::Sobel( size_t dimensions, size_t direction ) {
    try {
      if( direction >= dimensions ) {
        std::string msg( BIAL_ERROR( "Direction should be lower than the number of dimensions. Given direction: " +
                                     std::to_string( direction ) + ", dimensions: " + std::to_string(
												     dimensions ) + "." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Creating kernel.", 2 );
      Kernel krn( KernelType::Box( dimensions, 3 ) );
      COMMENT( "Computing kernel coefficients and displacements.", 2 );
      for( size_t elm = 0; elm < krn.size( ); ++elm ) {
        float coef = 1.0f;
        for( size_t dms = 0; dms < krn.Dims( ); ++dms ) {
          COMMENT( "Gradient direction.", 4 );
          if( dms == direction ) {
            if( krn( elm, dms ) < 0 )
              coef *= -1.0f;
            else if( krn( elm, dms ) > 0 )
              coef *= 1.0f;
            else {
              coef = 0.0f;
              break;
            }
          }
          else {
            COMMENT( "Smoothing direction.", 4 );
            if( krn( elm, dms ) == 0 )
              coef *= 2.0f;
          }
        }
        krn.Value( elm ) = coef;
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

  Kernel KernelType::NormalizedSobel( size_t dimensions, size_t direction ) {
    try {
      Kernel krn( KernelType::Sobel( dimensions, direction ) );
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

#ifdef BIAL_EXPLICIT_KernelSobel

#endif

}

#endif

#endif
