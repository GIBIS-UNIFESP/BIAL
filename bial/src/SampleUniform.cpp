/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/27 
 * @brief Functions to sample uniformly from feature vector.
 */

#ifndef BIALSAMPLEUNIFORM_C
#define BIALSAMPLEUNIFORM_C

#include "SampleUniform.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_SampleUniform )
#define BIAL_EXPLICIT_SampleUniform
#endif
#if defined ( BIAL_EXPLICIT_SampleUniform ) || ( BIAL_IMPLICIT_BIN )

#include "Feature.hpp"
#include "Sample.hpp"

namespace Bial {

  template< class D >
  Sample SampleType::Uniform( const Feature< D > &feats, size_t step ) {
    try {
      COMMENT( "Checking if step is positive.", 1 );
      if( step == 0 ) {
        std::string msg( BIAL_ERROR( "Step must be a positive number. Given: 0." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Computing the uniform sampling.", 0 );
      size_t samples = feats.Elements( ) / step;
      Sample result( samples );
      for( size_t spl = 0; spl < samples; ++spl ) {
        result( spl ) = spl * step;
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

#ifdef BIAL_EXPLICIT_SampleUniform

  template Sample SampleType::Uniform( const Feature< int > &feats, size_t step );
  template Sample SampleType::Uniform( const Feature< llint > &feats, size_t step );
  template Sample SampleType::Uniform( const Feature< float > &feats, size_t step );
  template Sample SampleType::Uniform( const Feature< double > &feats, size_t step );

#endif

}

#endif

#endif
