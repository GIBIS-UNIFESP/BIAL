/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/26 
 * @brief OPF spectral clustering algorithms. 
 */

#ifndef BIALOPFSPECTRALCLUSTERING_C
#define BIALOPFSPECTRALCLUSTERING_C

#include "OPFSpectralClustering.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_OPFSpectralClustering )
#define BIAL_EXPLICIT_OPFSpectralClustering
#endif
#if defined ( BIAL_EXPLICIT_OPFSpectralClustering ) || ( BIAL_IMPLICIT_BIN )

#include "Feature.hpp"
#include "Graph.hpp"
#include "KnnGraphAdjacency.hpp"
#include "LSHGraphAdjacency.hpp"

namespace Bial {

  template< class D >
  size_t OPF::SpectralClustering( Feature< D > &feature, float scale_min, float scale_max ) {
    try {
      Graph< KnnGraphAdjacency > graph;
      return( OPF::SpectralClustering( feature, scale_min, scale_max, graph ) );
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

  template< class D, class GRAPH_ADJACENCY >
  size_t OPF::SpectralClustering( Feature< D > &feature, float scale_min, float scale_max, 
                                  Graph< GRAPH_ADJACENCY > &graph ) {
    COMMENT( "Verifying if scale_max is lower than feature elements.", 1 );
    if( scale_max >= feature.Elements( ) ) {
      std::string msg( BIAL_ERROR( "scale_max greater than or equal to number of elements. Given: scale_max: " +
                                   std::to_string( scale_max ) + ", elements: " + std::to_string( feature.Elements( ) )
                                   + "." ) );
      throw( std::logic_error( msg ) );
    }
    COMMENT( "Verifying if 0.0 <= scale_min <= scale_max <= 1.0.", 1 );
    if( ( scale_min > scale_max ) || ( scale_max > 1.0f ) || ( scale_min < 0.0f ) ) {
      std::string msg( BIAL_ERROR( "scale_min greater than scale_max or scale_min, scale_max out of [0.0, 1.0] range." +
                                   " Given: Scale_Min: " + std::to_string( scale_min ) + ", scale_max: " +
                                   std::to_string( scale_max ) + "." ) );
      throw( std::logic_error( msg ) );
    }
    try {
      COMMENT( "Initializing graph with scales " << scale_min << " to " << scale_max << ".", 0 );
      graph.Initialize( feature, scale_min, scale_max );
      size_t scales = graph.Scales( );
      size_t best_scl = 0;
      double min_cut = std::numeric_limits< double >::max( );
      COMMENT( "Computing the best scale by means of the minimum cut.", 0 );
      for( size_t scl = 0; scl < scales; ++scl ) {
        COMMENT( "Clustering and getting the number of labels.", 1 );
        graph.Clustering( scl );
        COMMENT( "Computing the normalized cut value from clusters.", 1 );
        double cut = graph.NormalizedCut( scl );
        COMMENT( "Updating the best number of neighbors by the minimum cut value.", 1 );
        if( min_cut > cut ) {
          min_cut = cut;
          best_scl = scl;
        }
      }
      COMMENT( "Clustering and computing the number of labels for " << best_scl << " neighbors.", 0 );
      size_t nlabels = graph.Clustering( best_scl );
      graph.GnuPlot2DScatter( "final_cluster", feature, best_scl );
      COMMENT( "Propagating " << nlabels << " labels to all samples.", 0 );
      graph.PropagateLabel( feature, best_scl );
      return( nlabels );
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
  size_t OPF::SpectralClustering( Feature< D > &feature, size_t clusters ) {
    try {
      Graph< KnnGraphAdjacency > graph;
      return( OPF::SpectralClustering( feature, clusters, graph ) );
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

  template< class D, class GRAPH_ADJACENCY >
  size_t OPF::SpectralClustering( Feature< D > &feature, size_t clusters, Graph< GRAPH_ADJACENCY > &graph ) {
    COMMENT( "Verifying if there are enough samples and if there is at last one cluster.", 1 );
    if( ( feature.Elements( ) < clusters ) || ( clusters == 0 ) ) {
      std::string msg( BIAL_ERROR( "Not enough samples or asked for zero clusters. Given: samples: " +
                                   std::to_string( feature.Elements( ) ) + ", clsuters: " + 
                                   std::to_string( clusters ) + "." ) );
      throw( std::logic_error( msg ) );
    }
    try {
      COMMENT( "Initializing scales according to the number of clusters.", 0 );
      float scl = 0.5;
      float scl_step = scl;
      float best_scl = scl;
      COMMENT( "Initializing graph with scale " << scl << ".", 0 );
      graph.Initialize( feature, scl, scl );
      size_t best_labels = std::numeric_limits< size_t >::max( );
      Vector< int > old_label = graph.Label( );
      COMMENT( "Binary search for best scale that gives the expected number of clusters.", 0 );
      do {
        COMMENT( "Clustering. Goal: " << clusters << " clusters.", 1 );
        size_t nlabels = graph.Clustering( 0 );
        Vector< int > label = graph.Label( );
        COMMENT( "Updating the best number of neighbors by the closest value to the number of clusters.", 1 );
        if( best_labels - clusters > nlabels - clusters ) {
          best_labels = nlabels;
          best_scl = scl;
        }
        COMMENT( "bestk: " << best_scl << ", scl: " << scl << ", nlabels: " << nlabels << ", scl_step: " << scl_step,
                 1 );
        COMMENT( "Updating scl step and scl value.", 1 );
        if( nlabels < clusters ) {
          scl -= scl_step;
        }
        else {
          scl += scl_step;
        }
        if( ( nlabels != clusters ) && ( label != old_label ) ) {
          COMMENT( "Initializing graph with scale " << scl << ".", 1 );
          graph.Initialize( feature, scl, scl );
          scl_step /= 2;
          old_label = label;
        }
        else {
          COMMENT( "Re-initializing best graph with scale " << best_scl << ".", 1 );
          graph.Initialize( feature, best_scl, best_scl );
          graph.Clustering( 0 );
          break;
        }
      } while( true );
      COMMENT( "Propagating labels.", 0 );
      graph.PropagateLabel( feature, best_scl );
      return( best_labels );
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

#ifdef BIAL_EXPLICIT_OPFSpectralClustering

  template size_t OPF::SpectralClustering( Feature< int > &feature, float scale_min, float scale_max );
  template size_t OPF::SpectralClustering( Feature< int > &feature, size_t clusters );
  template size_t OPF::SpectralClustering( Feature< int > &feature, float scale_min, float scale_max,
                                           Graph< KnnGraphAdjacency > &graph );
  template size_t OPF::SpectralClustering( Feature< int > &feature, size_t clusters, 
                                           Graph< KnnGraphAdjacency > &graph );
  template size_t OPF::SpectralClustering( Feature< int > &feature, float scale_min, float scale_max, 
                                           Graph< LSHGraphAdjacency > &graph );
  template size_t OPF::SpectralClustering( Feature< int > &feature, size_t clusters, 
                                           Graph< LSHGraphAdjacency > &graph );

  template size_t OPF::SpectralClustering( Feature< llint > &feature, float scale_min, float scale_max );
  template size_t OPF::SpectralClustering( Feature< llint > &feature, size_t clusters );
  template size_t OPF::SpectralClustering( Feature< llint > &feature, float scale_min, float scale_max, 
                                           Graph< KnnGraphAdjacency > &graph );
  template size_t OPF::SpectralClustering( Feature< llint > &feature, size_t clusters, 
                                           Graph< KnnGraphAdjacency > &graph );
  template size_t OPF::SpectralClustering( Feature< llint > &feature, float scale_min, float scale_max,
                                           Graph< LSHGraphAdjacency > &graph );
  template size_t OPF::SpectralClustering( Feature< llint > &feature, size_t clusters, 
                                           Graph< LSHGraphAdjacency > &graph );

  template size_t OPF::SpectralClustering( Feature< float > &feature, float scale_min, float scale_max );
  template size_t OPF::SpectralClustering( Feature< float > &feature, size_t clusters );
  template size_t OPF::SpectralClustering( Feature< float > &feature, float scale_min, float scale_max,
                                           Graph< KnnGraphAdjacency > &graph );
  template size_t OPF::SpectralClustering( Feature< float > &feature, size_t clusters, 
                                           Graph< KnnGraphAdjacency > &graph );
  template size_t OPF::SpectralClustering( Feature< float > &feature, float scale_min, float scale_max,
                                           Graph< LSHGraphAdjacency > &graph );
  template size_t OPF::SpectralClustering( Feature< float > &feature, size_t clusters, 
                                           Graph< LSHGraphAdjacency > &graph );

  template size_t OPF::SpectralClustering( Feature< double > &feature, float scale_min, float scale_max );
  template size_t OPF::SpectralClustering( Feature< double > &feature, size_t clusters );
  template size_t OPF::SpectralClustering( Feature< double > &feature, float scale_min, float scale_max,
                                           Graph< KnnGraphAdjacency > &graph );
  template size_t OPF::SpectralClustering( Feature< double > &feature, size_t clusters, 
                                           Graph< KnnGraphAdjacency > &graph );
  template size_t OPF::SpectralClustering( Feature< double > &feature, float scale_min, float scale_max,
                                           Graph< LSHGraphAdjacency > &graph );
  template size_t OPF::SpectralClustering( Feature< double > &feature, size_t clusters, 
                                           Graph< LSHGraphAdjacency > &graph );

#endif

}

#endif

#endif
