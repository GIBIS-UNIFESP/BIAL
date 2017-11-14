/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Jun/26 
 * @brief Generates a feature vector composed by red, green, and blue colors as the features.
 * A vector is generated for each valid pixel.
 */

#ifndef BIALCOLORFEATURE_C
#define BIALCOLORFEATURE_C

#include "ColorFeature.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_ColorFeature )
#define BIAL_EXPLICIT_ColorFeature
#endif

#if defined ( BIAL_EXPLICIT_ColorFeature ) || ( BIAL_IMPLICIT_BIN )

#include "Adjacency.hpp"
#include "AdjacencyIterator.hpp"
#include "Color.hpp"
#include "Feature.hpp"
#include "Image.hpp"

namespace Bial {

  template< class D >
  Feature< D > ColorFeature( const Image< Color > &src, const Adjacency &adj_rel ) {
    try {
      IF_DEBUG( src.Dims( ) != adj_rel.Dims( ) ) {
        std::string msg( BIAL_ERROR( "Image and adjacency relation dimensions do not match." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Creating feature vector.", 0 );
      size_t features = adj_rel.size( ) * 3;
      if( features == 0 )
        features = 3;
      size_t elements = src.size( );
      Feature< D > res( elements, features );
      COMMENT( "Computing median features.", 0 );
      try {
        size_t total_threads = 12;
        Vector< std::thread > threads;
        for( size_t thd = 0; thd < total_threads; ++thd ) {
          threads.push_back( std::thread( &ColorFeatureThread< D >, std::ref( src ), std::ref( adj_rel ),
                                          std::ref( res ), thd, total_threads ) );
        }
        for( size_t thd = 0; thd < total_threads; ++thd ) {
          threads( thd ).join( );
        }
      }
      catch( std::exception &e ) {
        std::cout << "Warning: Failed to run in multi-thread. Exception: " << e.what( ) << std::endl;
        ColorFeatureThread( src, adj_rel, res, 0, 1 );
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
  void ColorFeatureThread( const Image< Color > &src, const Adjacency &adj_rel, Feature< D > &res, size_t thread, 
                           size_t total_threads ) {
    try {
      COMMENT( "Dealing with thread limits.", 2 );
      size_t img_size = src.size( );
      size_t adj_size = adj_rel.size( );
      size_t min_index = thread * img_size / total_threads;
      size_t max_index = ( thread + 1 ) * img_size / total_threads;
      COMMENT( "Creating adjacency iterator.", 2 );
      AdjacencyIterator adj_itr( src, adj_rel );
      size_t adj_elm;
      COMMENT( "Computing features.", 2 );
      for( size_t src_elm = min_index; src_elm < max_index; ++src_elm ) {
        COMMENT( "Setting indexes.", 4 );
        res.Index( src_elm ) = src_elm;
        COMMENT( "Computing neighbors.", 4 );
        for( size_t chn = 1; chn < 4; ++chn ) {
          for( size_t adj_position = 0; adj_position < adj_size; ++adj_position ) {
            COMMENT( "Assigning adjacent colors if it is valid and src element colors otherwise.", 4 );
            if( adj_itr.AdjIdx( src_elm, adj_position, adj_elm ) )
              res( src_elm, adj_position + adj_size * ( chn - 1 ) ) = src( adj_elm )( chn );
            else
              res( src_elm, adj_position + adj_size * ( chn - 1 ) ) = src( src_elm )( chn );
          }
        }
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

#ifdef BIAL_EXPLICIT_ColorFeature

  template Feature< int > ColorFeature( const Image< Color > &src, const Adjacency &adj_rel );
  template Feature< llint > ColorFeature( const Image< Color > &src, const Adjacency &adj_rel );
  template Feature< float > ColorFeature( const Image< Color > &src, const Adjacency &adj_rel );
  template Feature< double > ColorFeature( const Image< Color > &src, const Adjacency &adj_rel );

  template void ColorFeatureThread( const Image< Color > &src, const Adjacency &adj_rel, Feature< int > &res,
                                    size_t thread, size_t total_threads );
  template void ColorFeatureThread( const Image< Color > &src, const Adjacency &adj_rel, Feature< llint > &res,
                                    size_t thread, size_t total_threads );
  template void ColorFeatureThread( const Image< Color > &src, const Adjacency &adj_rel, Feature< float > &res,
                                    size_t thread, size_t total_threads );
  template void ColorFeatureThread( const Image< Color > &src, const Adjacency &adj_rel, Feature< double > &res,
                                    size_t thread, size_t total_threads );
  
#endif

}

#endif

#endif
