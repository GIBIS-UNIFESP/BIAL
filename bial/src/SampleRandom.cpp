/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/27 
 * @brief Random sampling elements from feature vector.
 */

#ifndef BIALSAMPLERANDOM_C
#define BIALSAMPLERANDOM_C

#include "SampleRandom.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_SampleRandom )
#define BIAL_EXPLICIT_SampleRandom
#endif
#if defined ( BIAL_EXPLICIT_SampleRandom ) || ( BIAL_IMPLICIT_BIN )

#include "Feature.hpp"
#include "Sample.hpp"

namespace Bial {

  Sample SampleType::Random( size_t subsamples, size_t total_samples ) {
    try {
      if( subsamples > total_samples ) {
        std::string msg( BIAL_ERROR( "More subsamples than total number of samples." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Creating sample vector with the expected number of elements.", 0 );
      Vector< bool > used( total_samples, false );
      Sample result( subsamples );
      COMMENT( "Assigning the random samples.", 0 );
      for( size_t spl = 0; spl < subsamples; ++spl ) {
        size_t elm;
        do {
          elm = static_cast< size_t >( rand( ) / static_cast< float >( RAND_MAX ) * total_samples - 0.000001 );
        } while( used( elm ) );
        result( spl ) = elm;
        used( elm ) = true;
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

  template< class D >
  Sample SampleType::RandomDistinct( const Feature< D > &feats, size_t subsamples ) {
    try {
      size_t total_samples = feats.Elements( );
      if( subsamples > total_samples ) {
        std::string msg( BIAL_ERROR( "More subsamples than total number of samples." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Creating sample vector and auxiliary structures.", 0 );
      Vector< bool > used( total_samples, false );
      Sample result;
      for( size_t spl = 0; spl < subsamples; ++spl ) {
        COMMENT( "Selecting the next sample.", 4 );
        size_t elm;
        do {
          elm = static_cast< size_t >( rand( ) / static_cast< float >( RAND_MAX ) * total_samples - 0.000001 );
        } while( used( elm ) );
        used( elm ) = true;
        COMMENT( "Checking if sample features were already inserted in resultant vector.", 4 );
        bool inserted = false;
        size_t pos;
        for( pos = 0; pos < result.size( ); ++pos ) {
          for( size_t ftr = 0; ftr < feats.Features( ); ++ftr ) {
            if( feats( result( pos ), ftr ) != feats( elm, ftr ) ) {
              break;
            }
            if( ( feats( result( pos ),
                         ftr ) == feats( elm, ftr ) ) && ( ftr == feats.Features( ) - 1 ) ) {
              inserted = true;
              break;
            }
          }
          if( inserted ) {
            break;
          }
        }
        if( !inserted ) {
          COMMENT( "If not inserted, grow the resultant vector.", 4 );
          result.push_back( );
        }
        COMMENT( "Inserting new sample into the resultant vector.", 4 );
        result.push_back( pos, elm );
      }
      COMMENT( "Total subsamples: " << subsamples << ", distinct samples: " << result.size( ), 0 );
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

#ifdef BIAL_EXPLICIT_SampleRandom

  template Sample SampleType::RandomDistinct( const Feature< int > &feats, size_t subsamples );
  template Sample SampleType::RandomDistinct( const Feature< llint > &feats, size_t subsamples );
  template Sample SampleType::RandomDistinct( const Feature< float > &feats, size_t subsamples );
  template Sample SampleType::RandomDistinct( const Feature< double > &feats, size_t subsamples );

#endif

}

#endif

#endif
