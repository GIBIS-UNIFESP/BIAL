/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/26 
 * Content: Hierarchical Graph for clustering.
 */

#ifndef BIALHIERARCHICALGRAPH_C
#define BIALHIERARCHICALGRAPH_C

#include "HierarchicalGraph.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_HierarchicalGraph )
#define BIAL_EXPLICIT_HierarchicalGraph
#endif

#if defined ( BIAL_EXPLICIT_HierarchicalGraph ) || ( BIAL_IMPLICIT_BIN )

#include "ClusteringIFT.hpp"
#include "Feature.hpp"
#include "GrowingBucketQueue.hpp"
#include "KnnGraphAdjacency.hpp"
#include "LSHGraphAdjacency.hpp"
#include "HierarchicalPathFunction.hpp"
#include "Sample.hpp"
#include "SortingSort.hpp"

namespace Bial {

  template< class GRAPH_ADJACENCY >
  template< class D >
  void HierarchicalGraph< GRAPH_ADJACENCY >::Initialize( const Feature< D > &feature, float scl_min, float scl_max ) {
    try {
      dynamic_cast< Graph< GRAPH_ADJACENCY >* >( this )->Initialize( feature, scl_min, scl_max );
      COMMENT( "Initializing merge and split labels.", 0 );
      size_t elements = feature.Elements( );
      COMMENT( "Initializing merge and split labels.", 0 );
      merge_label = Vector< int >( elements );
      for( size_t elm = 0; elm < elements; ++elm ) {
        merge_label( elm ) = elm;
      }
      split_label = Vector< int >( elements, 0 );
      COMMENT( "Initializing merge and split cuts.", 0 );
      merge_cut = Vector< double >( elements, std::numeric_limits< double >::max( ) );
      split_cut = Vector< double >( elements, std::numeric_limits< double >::max( ) );
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

  template< class GRAPH_ADJACENCY >
  Vector< int > &HierarchicalGraph< GRAPH_ADJACENCY >::MergeLabel( ) {
    return( merge_label );
  }

  template< class GRAPH_ADJACENCY >
  const Vector< int > &HierarchicalGraph< GRAPH_ADJACENCY >::MergeLabel( ) const {
    return( merge_label );
  }

  template< class GRAPH_ADJACENCY >
  Vector< int > &HierarchicalGraph< GRAPH_ADJACENCY >::SplitLabel( ) {
    return( split_label );
  }

  template< class GRAPH_ADJACENCY >
  const Vector< int > &HierarchicalGraph< GRAPH_ADJACENCY >::SplitLabel( ) const {
    return( split_label );
  }

  template< class GRAPH_ADJACENCY >
  Vector< double > &HierarchicalGraph< GRAPH_ADJACENCY >::MergeCut( ) {
    return( merge_cut );
  }

  template< class GRAPH_ADJACENCY >
  const Vector< double > &HierarchicalGraph< GRAPH_ADJACENCY >::MergeCut( ) const {
    return( merge_cut );
  }

  template< class GRAPH_ADJACENCY >
  Vector< double > &HierarchicalGraph< GRAPH_ADJACENCY >::SplitCut( ) {
    return( split_cut );
  }

  template< class GRAPH_ADJACENCY >
  const Vector< double > &HierarchicalGraph< GRAPH_ADJACENCY >::SplitCut( ) const {
    return( split_cut );
  }

  template< class GRAPH_ADJACENCY >
  size_t HierarchicalGraph< GRAPH_ADJACENCY >::Clustering( size_t scl ) {
    try {
      COMMENT( "Computing the PDF and delta value for clustering.", 2 );
      double delta = this->PDF( scl );
      COMMENT( "Initializing the labels of the graph nodes.", 2 );
      this->label.Set( -1 );
      COMMENT( "Clustering.", 2 );
      Vector< double > value( this->density );
      size_t size = value.size( );
      HierarchicalPathFunction< Vector, double > pf( value, this->label, nullptr, true, this->density + delta,
                                                     &merge_label, &split_label );
      GrowingBucketQueue queue( size, delta, false, true );
      for( size_t elm = 0; elm < size; ++elm )
        queue.Insert( elm, value[ elm ] );
      if( this->adjacency.HomogeneousSize( scl ) == 0 ) {
        COMMENT( "LSH IFT.", 2 );
        ClusteringIFT< Vector, double > 
          ift( value, &pf, this->adjacency.HeterogeneousAdjacency( this->density, scl, delta ),
               this->adjacency.HeterogeneousSize( scl ), &queue );
        ift.Run( );
      }
      else {
        COMMENT( "KNN IFT.", 2 );
        ClusteringIFT< Vector, double > 
          ift( value, &pf, this->adjacency.HomogeneousAdjacency( ), this->adjacency.HomogeneousSize( scl ),
               ( this->adjacency.HeterogeneousAdjacency( this->density, scl, delta ) ), &queue );
        ift.Run( );
      }
      COMMENT( "Sorting value vector.", 2 );
      this->ordered = Sorting::Sort( value, false );
      return( this->label.Maximum( ) + 1 );
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

  template< class GRAPH_ADJACENCY >
  size_t HierarchicalGraph< GRAPH_ADJACENCY >::MergeClusters( size_t ) {
    try {
      COMMENT( "Counting labels.", 2 );
      size_t old_labels = merge_label.Maximum( ) + 1;
      size_t new_labels = this->label.Maximum( ) + 1;
      size_t elements = merge_label.size( );

      COMMENT( "Computing new and old cuts for each old cluster.", 2 );
      Vector< double > old_label_cut( new_labels, 0.0 );
      Vector< double > new_label_cut( new_labels, 0.0 );
      Vector< Vector< bool > > merges( old_labels, Vector< bool >( new_labels, false ) );
      for( size_t elm = 0; elm < elements; ++elm ) {
        old_label_cut( this->label( elm ) ) += merge_cut( elm );
        new_label_cut( this->label( elm ) ) += this->cut( elm );
        merges( merge_label ( elm ) )( this->label( elm ) ) = true;
      }
      COMMENT( "old_cut: " << old_label_cut, 3 );
      COMMENT( "new_cut: " << new_label_cut, 3 );

      COMMENT( "Detecting removed labels and their new values.", 2 );
      Vector< int > root_label( old_labels );
      for( size_t lbl = 0; lbl < old_labels; ++lbl ) {
        root_label( lbl ) = lbl;
      }
      COMMENT( "For each new label, check for old labels that should merge and keep just one.", 2 );
      for( size_t new_lbl = 0; new_lbl < new_labels; ++new_lbl ) {
        if( old_label_cut( new_lbl ) > new_label_cut( new_lbl ) ) {
          int src_lbl = -1;
          for( size_t tgt_lbl = 0; tgt_lbl < old_labels; ++tgt_lbl ) {
            if( merges( tgt_lbl ) ( new_lbl ) ) {
              if( src_lbl == -1 ) {
                src_lbl = tgt_lbl;
              }
              root_label( tgt_lbl ) = src_lbl;
            }
          }
        }
      }
      COMMENT( "Root label: " << root_label, 2 );

      COMMENT( "Setting roots to the updated labels.", 2 );
      Vector< int > update_label( root_label );
      int next_label = 0;
      int last_label = old_labels - 1;
      while( next_label < last_label ) {
        if( root_label( next_label ) == next_label ) {
          ++next_label;
        }
        else {
          if( root_label( last_label ) == last_label ) {
            update_label( last_label ) = next_label;
            ++next_label;
          }
          --last_label;
        }
      }
      COMMENT( "Update label: " << update_label, 2 );
      COMMENT( "Setting non-roots to the updated labels.", 2 );
      for( size_t lbl = 0; lbl < old_labels; ++lbl ) {
        if( root_label( lbl ) != static_cast< int >( lbl ) ) {
          update_label( lbl ) = update_label( root_label( lbl ) );
        }
      }
      COMMENT( "Update label: " << update_label, 2 );
      COMMENT( "Updating labels and cut if new cut is better.", 2 );
      for( size_t elm = 0; elm < elements; ++elm ) {
        if( old_label_cut( this->label( elm ) ) > new_label_cut( this->label( elm ) ) ) {
          merge_cut( elm ) = this->cut( elm );
        }
        merge_label( elm ) = update_label( merge_label( elm ) );
      }
      return( merge_label.Maximum( ) + 1 );
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

  template< class GRAPH_ADJACENCY >
  size_t HierarchicalGraph< GRAPH_ADJACENCY >::SplitClusters( size_t ) {
    try {
      COMMENT( "Counting labels.", 2 );
      size_t old_labels = split_label.Maximum( ) + 1;
      size_t new_labels = this->label.Maximum( ) + 1;
      size_t elements = merge_label.size( );
      COMMENT( "Computing new and old cuts for each old cluster.", 2 );
      COMMENT( "old_labels: " << old_labels << ", new_labels: " << new_labels, 1 );
      Vector< double > old_label_cut( old_labels, 0.0 );
      Vector< double > new_label_cut( old_labels, 0.0 );
      Vector< Vector< bool > > splits( old_labels, Vector< bool >( new_labels, false ) );
      COMMENT( "elements: " << elements, 1 );
      for( size_t elm = 0; elm < elements; ++elm ) {
        old_label_cut( split_label( elm ) ) += split_cut( elm );
        new_label_cut( split_label( elm ) ) += this->cut( elm );
        splits( split_label ( elm ) )( this->label( elm ) ) = true;
      }
      COMMENT( "Detecting removed labels.", 2 );
      Vector< int > removed_label;
      for( size_t lbl = 0; lbl < old_labels; ++lbl ) {
        if( old_label_cut( lbl ) > new_label_cut( lbl ) ) {
          removed_label.push_back( lbl );
        }
      }
      if( removed_label.size( ) > 0 ) {
        COMMENT( "Defining the new added labels.", 2 );
        size_t next_label = removed_label( 0 );
        size_t label_idx = 0;
        Vector< Vector< int > > update_label( old_labels, Vector< int >( new_labels, -1 ) );
        for( size_t old_lbl = 0; old_lbl < old_labels; ++old_lbl ) {
          if( old_label_cut( old_lbl ) > new_label_cut( old_lbl ) ) {
            for( size_t new_lbl = 0; new_lbl < new_labels; ++new_lbl ) {
              if( splits( old_lbl ) ( new_lbl ) ) {
                update_label( old_lbl ) ( new_lbl ) = next_label;
                ++label_idx;
                if( label_idx < removed_label.size( ) ) {
                  next_label = removed_label( label_idx );
                }
                else {
                  next_label = old_labels + label_idx - removed_label.size( );
                }
              }
            }
          }
        }
        COMMENT( "Updating labels if new cut is better.", 2 );
        for( size_t elm = 0; elm < elements; ++elm ) {
          if( old_label_cut( split_label( elm ) ) > new_label_cut( split_label( elm ) ) ) {
            split_cut( elm ) = this->cut( elm );
            split_label( elm ) = update_label( split_label ( elm ) )( this->label( elm ) );
          }
        }
      }
      return( split_label.Maximum( ) + 1 );
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

#ifdef BIAL_EXPLICIT_HierarchicalGraph

  template void HierarchicalGraph< KnnGraphAdjacency >::Initialize( const Feature< int > &feature, float scl_min,
                                                                    float scl_max );
  template void HierarchicalGraph< KnnGraphAdjacency >::Initialize( const Feature< llint > &feature, float scl_min, 
                                                                    float scl_max );
  template void HierarchicalGraph< KnnGraphAdjacency >::Initialize( const Feature< float > &feature, float scl_min,
                                                                    float scl_max );
  template void HierarchicalGraph< KnnGraphAdjacency >::Initialize( const Feature< double > &feature, float scl_min,
                                                                    float scl_max );

  template void HierarchicalGraph< LSHGraphAdjacency >::Initialize( const Feature< int > &feature, float scl_min, 
                                                                    float scl_max );
  template void HierarchicalGraph< LSHGraphAdjacency >::Initialize( const Feature< llint > &feature, float scl_min,
                                                                    float scl_max );
  template void HierarchicalGraph< LSHGraphAdjacency >::Initialize( const Feature< float > &feature, float scl_min,
                                                                    float scl_max );
  template void HierarchicalGraph< LSHGraphAdjacency >::Initialize( const Feature< double > &feature, float scl_min,
                                                                    float scl_max );

  template Vector< int > &HierarchicalGraph< KnnGraphAdjacency >::MergeLabel( );
  template const Vector< int > &HierarchicalGraph< KnnGraphAdjacency >::MergeLabel( ) const;
  template Vector< int > &HierarchicalGraph< KnnGraphAdjacency >::SplitLabel( );
  template const Vector< int > &HierarchicalGraph< KnnGraphAdjacency >::SplitLabel( ) const;
  template Vector< double > &HierarchicalGraph< KnnGraphAdjacency >::MergeCut( );
  template const Vector< double > &HierarchicalGraph< KnnGraphAdjacency >::MergeCut( ) const;
  template Vector< double > &HierarchicalGraph< KnnGraphAdjacency >::SplitCut( );
  template const Vector< double > &HierarchicalGraph< KnnGraphAdjacency >::SplitCut( ) const;
  template size_t HierarchicalGraph< KnnGraphAdjacency >::Clustering( size_t scl );
  template size_t HierarchicalGraph< KnnGraphAdjacency >::MergeClusters( size_t );
  template size_t HierarchicalGraph< KnnGraphAdjacency >::SplitClusters( size_t );

  template Vector< int > &HierarchicalGraph< LSHGraphAdjacency >::MergeLabel( );
  template const Vector< int > &HierarchicalGraph< LSHGraphAdjacency >::MergeLabel( ) const;
  template Vector< int > &HierarchicalGraph< LSHGraphAdjacency >::SplitLabel( );
  template const Vector< int > &HierarchicalGraph< LSHGraphAdjacency >::SplitLabel( ) const;
  template Vector< double > &HierarchicalGraph< LSHGraphAdjacency >::MergeCut( );
  template const Vector< double > &HierarchicalGraph< LSHGraphAdjacency >::MergeCut( ) const;
  template Vector< double > &HierarchicalGraph< LSHGraphAdjacency >::SplitCut( );
  template const Vector< double > &HierarchicalGraph< LSHGraphAdjacency >::SplitCut( ) const;
  template size_t HierarchicalGraph< LSHGraphAdjacency >::Clustering( size_t scl );
  template size_t HierarchicalGraph< LSHGraphAdjacency >::MergeClusters( size_t );
  template size_t HierarchicalGraph< LSHGraphAdjacency >::SplitClusters( size_t );

#endif

}

#endif

#endif
