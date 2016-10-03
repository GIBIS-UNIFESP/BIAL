/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/26 
 * @brief OPF hierarchical clustering algorithms. NOT WORKING YET.
 */

#ifndef BIALOPFHIERARCHICALCLUSTERING_C
#define BIALOPFHIERARCHICALCLUSTERING_C

#include "OPFHierarchicalClustering.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_OPFHierarchicalClustering )
#define BIAL_EXPLICIT_OPFHierarchicalClustering
#endif
#if defined ( BIAL_EXPLICIT_OPFHierarchicalClustering ) || ( BIAL_IMPLICIT_BIN )

#include "Feature.hpp"
#include "Graph.hpp"
#include "HierarchicalGraph.hpp"
#include "KnnGraphAdjacency.hpp"
#include "LSHGraphAdjacency.hpp"

namespace Bial {

  template< class D >
  size_t OPF::HierarchicalClustering( Feature< D > &feature, float scale_min, float scale_max ) {
    try {
      HierarchicalGraph< KnnGraphAdjacency > graph;
      return( OPF::HierarchicalClustering( feature, scale_min, scale_max, graph ) );
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
  size_t OPF::HierarchicalClustering( Feature< D > &feature, float scale_min, float scale_max,
                                      HierarchicalGraph< GRAPH_ADJACENCY > &graph ) {
    COMMENT( "Verifying if scale_max is lower than feature elements.", 1 );
    if( scale_max >= feature.Elements( ) ) {
      std::string msg( BIAL_ERROR( "scale_max greater than or equal to number of elements. Given: scale_max: " +
                                   std::to_string( scale_max ) + ", elements: " + std::to_string( feature.Elements( ) )
                                   + "." ) );
      throw( std::logic_error( msg ) );
    }
    COMMENT( "Verifying if 0.0 <= scale_min <= scale_max <= 1.0.", 1 );
    if( ( scale_min > scale_max ) || ( scale_max > 1.0f ) || ( scale_min < 0.0f ) ) {
      std::string msg( BIAL_ERROR( "scale_min greater than scale_max or scale_min, scale_max out of [0.0, 1.0] range. " 
                                   + "Given: Scale_Min: " + std::to_string( scale_min ) + ", scale_max: " +
                                   std::to_string( scale_max ) + "." ) );
      throw( std::logic_error( msg ) );
    }
    try {
      COMMENT( "Initializing graph with scales " << scale_min << " to " << scale_max << ".", 0 );
      graph.Initialize( feature, scale_min, scale_max );
      size_t scales = graph.Scales( );
      COMMENT( "Initializing merge data with scale 0.", 0 );
      dynamic_cast< Graph< GRAPH_ADJACENCY > & >( graph ).Clustering( 0 );
      graph.MergeLabel( ) = graph.Label( );
      graph.MergeCut( ) = graph.SampleNormalizedCut( graph.MergeLabel( ), 0 );
      COMMENT( "merge_label: " << graph.MergeLabel( ), 1 );
      COMMENT( "Initializing split data with scale " << scales - 1 << ".", 0 );
      graph.Clustering( scales - 1 );
      graph.SetCut( scales - 1 );
      graph.SplitClusters( scales - 1 );
      COMMENT( "split_label: " << graph.SplitLabel( ), 1 );
      size_t best_scl = 0;
      size_t nlabels = 1;
      COMMENT( "Computing the best scale among " << scales << ".", 0 );
      for( size_t scl = 1; scl < scales; ++scl ) {
        COMMENT( "Merging clusters with scale " << scl << ".", 1 );
        graph.Clustering( scl );
        graph.SetCut( scl );
        COMMENT( "Merging clusters... " << scl << ".", 1 );
        nlabels = graph.MergeClusters( scl );
        COMMENT( "merge_label: " << graph.MergeLabel( ), 1 );
        COMMENT( "label: " << graph.Label( ), 1 );
        COMMENT( nlabels << " labels after " << scl << " neighbors.", 1 );
        COMMENT( "Spliting clusters with scale " << scales - scl - 1 << ".", 1 );
        graph.Clustering( scales - scl - 1 );
        graph.SetCut( scales - scl - 1 );
        COMMENT( "Splitting clusters... " << scales - scl - 1 << ".", 1 );
        nlabels = graph.SplitClusters( scales - scl - 1 );
        COMMENT( "split_label: " << graph.SplitLabel( ), 1 );
        COMMENT( "label: " << graph.Label( ), 1 );
        COMMENT( nlabels << " labels after " << scl << " and " << scales - scl - 1 << " scales.", 1 );
        COMMENT( "Labels: " << graph.Label( ), 3 );
      }
      COMMENT( "Final merge label: " << graph.MergeLabel( ), 0 );
      COMMENT( "Final split label: " << graph.SplitLabel( ), 0 );
      COMMENT( "Propagating " << nlabels << " labels to all samples.", 0 );
      graph.Label( ) = graph.SplitLabel( );
      graph.PropagateLabel( feature, best_scl );
      COMMENT( "Final label: " << feature.Label( ), 4 );
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


#ifdef BIAL_EXPLICIT_OPFHierarchicalClustering

  template size_t OPF::HierarchicalClustering( Feature< int > &feature, float scale_min, float scale_max );
  template size_t OPF::HierarchicalClustering( Feature< int > &feature, float scale_min, float scale_max,
                                               HierarchicalGraph< KnnGraphAdjacency > &graph );
  template size_t OPF::HierarchicalClustering( Feature< int > &feature, float scale_min, float scale_max,
                                               HierarchicalGraph< LSHGraphAdjacency > &graph );

  template size_t OPF::HierarchicalClustering( Feature< llint > &feature, float scale_min, float scale_max );
  template size_t OPF::HierarchicalClustering( Feature< llint > &feature, float scale_min, float scale_max,
					       HierarchicalGraph< KnnGraphAdjacency > &graph );
  template size_t OPF::HierarchicalClustering( Feature< llint > &feature, float scale_min, float scale_max, 
					       HierarchicalGraph< LSHGraphAdjacency > &graph );

  template size_t OPF::HierarchicalClustering( Feature< float > &feature, float scale_min, float scale_max );
  template size_t OPF::HierarchicalClustering( Feature< float > &feature, float scale_min, float scale_max,
                                               HierarchicalGraph< KnnGraphAdjacency > &graph );
  template size_t OPF::HierarchicalClustering( Feature< float > &feature, float scale_min, float scale_max,
                                               HierarchicalGraph< LSHGraphAdjacency > &graph );

  template size_t OPF::HierarchicalClustering( Feature< double > &feature, float scale_min, float scale_max );
  template size_t OPF::HierarchicalClustering( Feature< double > &feature, float scale_min, float scale_max, 
					       HierarchicalGraph< KnnGraphAdjacency > &graph );
  template size_t OPF::HierarchicalClustering( Feature< double > &feature, float scale_min, float scale_max, 
					       HierarchicalGraph< LSHGraphAdjacency > &graph );

#endif

}

#endif

#endif
