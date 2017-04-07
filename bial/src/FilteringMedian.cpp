/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2013/Jul/12 
 * @brief Median filter.
 */

#ifndef BIALFILTERINGMEDIAN_C
#define BIALFILTERINGMEDIAN_C

#include "FilteringMedian.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_FilteringMedian )
#define BIAL_EXPLICIT_FilteringMedian
#endif

#if defined ( BIAL_EXPLICIT_FilteringMedian ) || ( BIAL_IMPLICIT_BIN )

#include "AdjacencyRound.hpp"
#include "AdjacencyIterator.hpp"
#ifdef BIAL_DEBUG
#include "FileImage.hpp"
#endif
#include "Image.hpp"

namespace Bial {

  template< class D >
  Image< D > Filtering::Median( const Image< D > &img, float radius ) {
    try {
      Image< D > res( img );
      Adjacency adj( AdjacencyType::HyperSpheric( radius, img.Dims( ) ) );

      try {
        size_t total_threads = 12;
        Vector< std::thread > threads;
        for( size_t thd = 0; thd < total_threads; ++thd )
          threads.push_back( std::thread( &Filtering::MedianThreads< D >, std::ref( img ), std::ref( adj ),
                                          std::ref( res ), thd, total_threads ) );
        for( size_t thd = 0; thd < total_threads; ++thd )
          threads[ thd ].join( );
      }
      catch( std::exception &e ) {
        BIAL_WARNING( "Failed to run in multi-thread. Exception: " << e.what( ) );
        Filtering::MedianThreads( img, adj, res, 0, 1 );
      }
      return( res );
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
  void Filtering::MedianThreads( const Image< D > &img, const Adjacency &adj, Image< D > &res, size_t thread,
                                 size_t total_threads ) {
    try {
      COMMENT( "Dealing with thread limits.", 2 );
      size_t min_index = thread * img.Size( ) / total_threads;
      size_t max_index = ( thread + 1 ) * img.Size( ) / total_threads;
      AdjacencyIterator adj_itr( img, adj );
      size_t adj_size = adj.size( );
      size_t adj_pxl;
      COMMENT( "Computing median filter.", 2 );
      for( size_t pxl = min_index; pxl < max_index; ++pxl ) {
        Vector< float > queue( adj_size );
        size_t idx = 0;
        for( size_t adj_idx = 0; adj_idx < adj_size; ++adj_idx ) {
          if( adj_itr.AdjIdx( pxl, adj_idx, adj_pxl ) ) {
            queue[ idx ] = img[ adj_pxl ];
            ++idx;
          }
        }
        sort( queue.begin( ), queue.begin( ) + idx );
        res[ pxl ] = queue[ ( idx - 1 ) / 2 ];
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

#ifdef BIAL_EXPLICIT_FilteringMedian

  template Image< int > Filtering::Median( const Image< int > &img, float radius );
  template void Filtering::MedianThreads( const Image< int > &img, const Adjacency &adj, Image< int > &res,
                                          size_t thread, size_t total_threads );
  template Image< llint > Filtering::Median( const Image< llint > &img, float radius );
  template void Filtering::MedianThreads( const Image< llint > &img, const Adjacency &adj, Image< llint > &res,
                                          size_t thread, size_t total_threads );
  template Image< float > Filtering::Median( const Image< float > &img, float radius );
  template void Filtering::MedianThreads( const Image< float > &img, const Adjacency &adj, Image< float > &res,
                                          size_t thread, size_t total_threads );
  template Image< double > Filtering::Median( const Image< double > &img, float radius );
  template void Filtering::MedianThreads( const Image< double > &img, const Adjacency &adj, Image< double > &res,
                                          size_t thread, size_t total_threads );

#endif

}


#endif

#endif
