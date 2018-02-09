/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Jun/26 
 * @brief Feature vector extracted from image. Gets the median intensity pixels around each valid pixel.
 * A feature vector is generated from each valid pixel.
 */


#ifndef BIALMEDIANFEATURE_C
#define BIALMEDIANFEATURE_C

#include "MedianFeature.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_MedianFeature )
#define BIAL_EXPLICIT_MedianFeature
#endif

#if defined ( BIAL_EXPLICIT_MedianFeature ) || ( BIAL_IMPLICIT_BIN )

#include "Adjacency.hpp"
#include "AdjacencyIterator.hpp"
#include "Feature.hpp"
#include "Image.hpp"
#include "Matrix.hpp"

namespace Bial {

  template< class D >
  Feature< D > MedianFeature( const Image< D > &src, const Adjacency &adj_rel, float proportion ) {
    try {
      if( ( proportion > 1.0 ) || ( proportion <= 0.0 ) ) {
        std::string msg( BIAL_ERROR( "Invalid proportion. Expected: 0 < proportion < 1." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Creating feature vector.", 0 );
      size_t features = adj_rel.size( ) * proportion;
      if( features == 0 )
        features = 1;
      size_t elements = src.size( );
      Feature< D > res( elements, features );
      COMMENT( "Computing median features.", 0 );
      try {
        size_t total_threads = 12;
        Vector< std::thread > threads;
        for( size_t thd = 0; thd < total_threads; ++thd ) {
          threads.push_back( std::thread( &MedianFeatureThread< D >, std::ref( src ), std::ref( adj_rel ),
                                          std::ref( res ), thd, total_threads ) );
        }
        for( size_t thd = 0; thd < total_threads; ++thd ) {
          threads( thd ).join( );
        }
      }
      catch( std::exception &e ) {
        std::cout << "Warning: Failed to run in multi-thread. Exception: " << e.what( ) << std::endl;
        MedianFeatureThread( src, adj_rel, res, 0, 1 );
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
  Feature< D > MedianFeature( const Image< D > &src, const Image< D > &msk, const Adjacency &adj_rel, 
                              float proportion ) {
    try {
      AdjacencyIterator adj_itr( src, adj_rel );
      size_t adj_size = adj_rel.size( );
      size_t img_size = src.size( );
      size_t adj_pxl;
      IF_DEBUG( ( proportion >= 1.0 ) || ( proportion <= 0.0 ) ) {
        std::string msg( BIAL_ERROR( "Invalid proportion. Expected: 0 < proportion < 1." ) );
        throw( std::logic_error( msg ) );
      }
      IF_DEBUG( src.Dims( ) != msk.Dims( ) ) {
        std::string msg( BIAL_ERROR( "Source image and mask image dimensions do not match." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Creating feature vector.", 0 );
      size_t features = adj_size * proportion;
      if( features == 0 )
        features = 1;
      size_t elements = 0;
      for( size_t pxl = 0; pxl < img_size; ++pxl ) {
        if( msk( pxl ) != 0 )
          ++elements;
      }
      Feature< D > res( elements, features );
      COMMENT( "Computing median features.", 0 );
      Vector< D > median( adj_size );
      size_t adj_base = ( adj_size / 2 ) - ( features / 2 );
      size_t tgt_pxl = 0;
      for( size_t src_pxl = 0; src_pxl < img_size; ++src_pxl ) {
        if( msk[ src_pxl ] != 0 ) {
          for( size_t adj = 0; adj < adj_size; ++adj ) {
            if( ( adj_itr.AdjIdx( src_pxl, adj, adj_pxl ) ) && ( msk[ adj_pxl ] != 0 ) )
              median( adj ) = src[ adj_pxl ];
            else
              median( adj ) = src[ src_pxl ];
          }
          sort( median.begin( ), median.end( ) );
          for( size_t ftr = 0; ftr < features; ++ftr ) {
            res( tgt_pxl, ftr ) = median( adj_base + ftr );
            res.Index( tgt_pxl ) = src_pxl;
          }
          ++tgt_pxl;
        }
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
  void MedianFeatureThread( const Image< D > &src, const Adjacency &adj_rel, Feature< D > &res, size_t thread,
                            size_t total_threads ) {
    try {
      COMMENT( "Dealing with thread limits.", 2 );
      size_t features = res.Features( );
      size_t elements = res.Elements( );
      size_t min_index = thread * elements / total_threads;
      size_t max_index = ( thread + 1 ) * elements / total_threads;
      COMMENT( "Computing median features.", 2 );
      AdjacencyIterator adj_itr( src, adj_rel );
      size_t adj_size = adj_rel.size( );
      size_t adj_pxl;
      Vector< D > median( adj_size );
      size_t adj_base = ( adj_size / 2 ) - ( features / 2 );
      for( size_t src_pxl = min_index; src_pxl < max_index; ++src_pxl ) {
        for( size_t adj = 0; adj < adj_size; ++adj ) {
          if( adj_itr.AdjIdx( src_pxl, adj, adj_pxl ) )
            median( adj ) = src[ adj_pxl ];
          else
            median( adj ) = src[ src_pxl ];
        }
        std::sort( median.begin( ), median.end( ) );
        for( size_t ftr = 0; ftr < features; ++ftr )
          res( src_pxl, ftr ) = median( adj_base + ftr );
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

#ifdef BIAL_EXPLICIT_MedianFeature

  template Feature< int > MedianFeature( const Image< int > &src, const Adjacency &adj, float proportion );
  template Feature< llint > MedianFeature( const Image< llint > &src, const Adjacency &adj, float proportion );
  template Feature< float > MedianFeature( const Image< float > &src, const Adjacency &adj, float proportion );
  template Feature< double > MedianFeature( const Image< double > &src, const Adjacency &adj, float proportion );
  
  template Feature< int > MedianFeature( const Image< int > &src, const Image< int > &msk, const Adjacency &adj,
                                float proportion );
  template Feature< llint > MedianFeature( const Image< llint > &src, const Image< llint > &msk, const Adjacency &adj,
                                  float proportion );
  template Feature< float > MedianFeature( const Image< float > &src, const Image< float > &msk, const Adjacency &adj,
                                  float proportion );
  template Feature< double > MedianFeature( const Image< double > &src, const Image< double > &msk, 
                                            const Adjacency &adj, float proportion );

  template void MedianFeatureThread( const Image< int > &src, const Adjacency &adj_rel, Feature< int > &res,
                                     size_t thread, size_t total_threads );
  template void MedianFeatureThread( const Image< llint > &src, const Adjacency &adj_rel, Feature< llint > &res,
                                     size_t thread, size_t total_threads );
  template void MedianFeatureThread( const Image< float > &src, const Adjacency &adj_rel, Feature< float > &res,
                                     size_t thread, size_t total_threads );
  template void MedianFeatureThread( const Image< double > &src, const Adjacency &adj_rel, Feature< double > &res,
                                     size_t thread, size_t total_threads );
  
#endif

}

#endif

#endif
