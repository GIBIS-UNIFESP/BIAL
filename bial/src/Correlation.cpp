/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/**
 * @date 2013/Jul/12 
 * @brief Image filtering methods.
 */

#ifndef BIALCORRELATION_C
#define BIALCORRELATION_C

#include "Correlation.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_Correlation )
#define BIAL_EXPLICIT_Correlation
#endif

#if defined ( BIAL_EXPLICIT_Correlation ) || ( BIAL_IMPLICIT_BIN )

#include "AdjacencyIterator.hpp"
#ifdef BIAL_DEBUG
#include "FileImage.hpp"
#endif
#include "Image.hpp"
#include "Kernel.hpp"

namespace Bial {

  template< class D >
  Image< D > Correlation( const Image< D > &img, const Kernel &krn ) {
    try {
      if( img.Dims( ) != krn.Dims( ) ) {
        std::string msg( BIAL_ERROR( "Image and kernel dimensions do not match." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Creating resulting image.", 1 );
      Image< D > result( img );
      try {
        size_t total_threads = 12;
        Vector< std::thread > threads;
        COMMENT( "Starting threads.", 1 );
        for( size_t thd = 0; thd < total_threads; ++thd )
          threads.push_back( std::thread( &CorrelationThreads< D >, std::ref( img ), std::ref( krn ),
                                          std::ref( result ), thd, total_threads ) );
        for( size_t thd = 0; thd < total_threads; ++thd ) {
          COMMENT( "Merging.", 1 );
          threads( thd ).join( );
        }
        COMMENT( "Finished threads.", 1 );
      }
      catch( std::exception &e ) {
        BIAL_WARNING( "Failed to run in multi-thread. Exception: " << e.what( ) );
        CorrelationThreads( img, krn, result, 0, 1 );
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
    catch( const std::exception &e ) {
      BIAL_WARNING( "Thread execution exception." );
      throw( std::exception( e ) );
    }
  }

  template< class D >
  void CorrelationThreads( const Image< D > &img, const Kernel &krn, Image< D > &res, size_t thread, 
                           size_t total_threads ) {
    try {
      size_t img_size = img.Size( );
      size_t adj_size = krn.size( );
      AdjacencyIterator adj_itr( img, krn );
      size_t adj_pxl;
      size_t min_index = thread * img_size / total_threads;
      size_t max_index = ( thread + 1 ) * img_size / total_threads;
      COMMENT( "Running algorithm from " << min_index << " to " << max_index - 1, 1 );
      for( size_t pxl = min_index; pxl < max_index; ++pxl ) {
        double sum = 0.0;
        for( size_t idx = 0; idx < adj_size; ++idx ) {
          if( adj_itr.AdjIdx( pxl, idx, adj_pxl ) )
            sum += static_cast< double >( img[ adj_pxl ] ) * krn.Value( idx );
        }
        res[ pxl ] = static_cast< D >( sum );
      }
      COMMENT( "Finishing.", 1 );
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
    catch( const std::exception &e ) {
      BIAL_WARNING( "Thread execution exception." );
      throw( std::exception( e ) );
    }
  }
  
#ifdef BIAL_EXPLICIT_Correlation

  template Image< int > Correlation( const Image< int > &img, const Kernel &krn );
  template void CorrelationThreads( const Image< int > &img, const Kernel &krn, Image< int > &res, size_t thread, 
                                    size_t total_threads );

  template Image< llint > Correlation( const Image< llint > &img, const Kernel &krn );
  template void CorrelationThreads( const Image< llint > &img, const Kernel &krn, Image< llint > &res, size_t thread,
                                    size_t total_threads );

  template Image< float > Correlation( const Image< float > &img, const Kernel &krn );
  template void CorrelationThreads( const Image< float > &img, const Kernel &krn, Image< float > &res, size_t thread,
                                    size_t total_threads );

  template Image< double > Correlation( const Image< double > &img, const Kernel &krn );
  template void CorrelationThreads( const Image< double > &img, const Kernel &krn, Image< double > &res, size_t thread,
                                    size_t total_threads );

#endif

}


#endif

#endif
