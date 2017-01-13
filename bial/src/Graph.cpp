/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/26 
 * Content: Graph structure for clustering.
 */

#ifndef BIALGRAPH_C
#define BIALGRAPH_C

#include "Graph.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_Graph )
#define BIAL_EXPLICIT_Graph
#endif

#if defined ( BIAL_EXPLICIT_Graph ) || ( BIAL_IMPLICIT_BIN )

#include "ClusteringIFT.hpp"
#include "DFIDE.hpp"
#include "Feature.hpp"
#include "GrowingBucketQueue.hpp"
#include "KnnGraphAdjacency.hpp"
#include "LSHGraphAdjacency.hpp"
#include "MinPathFunction.hpp"
#include "SampleRandom.hpp"
#include "SampleUniform.hpp"
#include "SortingSort.hpp"

namespace Bial {

  template< class GRAPH_ADJACENCY >
  template< class D >
  void Graph< GRAPH_ADJACENCY >::Initialize( const Feature< D > &feature, float scl_min, float scl_max ) {
    try {
      COMMENT( "Checking if subsampling is required.", 1 );
      size_t elements = std::min( feature.Elements( ), static_cast< size_t >( adjacency.MaxSamples( ) ) );
      if( elements < feature.Elements( ) ) {
        sample = SampleType::RandomDistinct( feature, elements );
      }
      else {
        sample = SampleType::Uniform( feature );
      }
      elements = sample.size( );

      COMMENT( "Initializing other structures.", 1 );
      density = Vector< double >( elements );
      label = Vector< int >( elements, -1 );

      COMMENT( "Initializing adjacency.", 0 );
      adjacency.Initialize( feature, sample, scl_min, scl_max );
      COMMENT( "Scales: " << adjacency.Scales( ), 2 );
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
  size_t Graph< GRAPH_ADJACENCY >::MaxSamples( ) const {
    try {
      return( adjacency.MaxSamples( ) );
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
  size_t Graph< GRAPH_ADJACENCY >::Scales( ) const {
    try {
      return( adjacency.Scales( ) );
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
  size_t Graph< GRAPH_ADJACENCY >::Samples( ) const {
    return( sample.size( ) );
  }

  template< class GRAPH_ADJACENCY >
  Vector< int > &Graph< GRAPH_ADJACENCY >::Label( ) {
    return( label );
  }

  template< class GRAPH_ADJACENCY >
  const Vector< int > &Graph< GRAPH_ADJACENCY >::Label( ) const {
    return( label );
  }

  template< class GRAPH_ADJACENCY >
  double Graph< GRAPH_ADJACENCY >::PDF( size_t scl ) {
    try {
      COMMENT( "Computing sigma, i.e. the variance of gaussian.", 2 );
      double sigma = adjacency.Sigma( scl );
      if( sigma == 0.0 ) {
        sigma = 1.0;
      }
      double mindens = std::numeric_limits< double >::max( );
      double maxdens = std::numeric_limits< double >::min( );

      COMMENT( "Sigma: " << sigma << ". Computing nodes density and the minimal and maximal densities.", 2 );
      for( size_t src = 0; src < density.size( ); ++src ) {
        COMMENT( "src: " << src, 4 );
        density( src ) = 1.0;
        COMMENT( "Computing the density from the adjacents.", 4 );
        size_t neighbors = adjacency.Arcs( src, scl );
        for( size_t adj = 0; adj < neighbors; ++adj ) {
          double weight = adjacency.ArcWeight( src, adj );
          density( src ) += exp( -weight * weight / sigma );
          COMMENT( "adj: " << adj << " weight: " << weight << ", exp: " << -weight * weight / sigma << ", dens: " <<
                   exp( -weight * weight / sigma ), 4 );
          COMMENT( "partial sum: " << density( src ), 4 );
        }
        COMMENT( "Here, see the difference when additional arcs from symmetry are inserted.", 4 );
        /* density( src ) /= ( neighbors + 1 ); */
        COMMENT( "final density: " << density( src ), 4 );
        COMMENT( "Getting maximum and minimum density for delta computing.", 4 );
        if( mindens > density( src ) ) {
          mindens = density( src );
        }
        if( maxdens < density( src ) ) {
          maxdens = density( src );
        }
      }
      COMMENT( "Density: " << density, 3 );

      COMMENT( "Computing IFT bucket size.", 2 );
      if( maxdens == mindens ) { /* Hardly happens. */
        return( maxdens / 10000.0 );
      }
      else {
        return( ( maxdens - mindens ) / 10000.0 );
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

  template< class GRAPH_ADJACENCY >
  size_t Graph< GRAPH_ADJACENCY >::Clustering( size_t scl ) {
    try {
      COMMENT( "Computing the PDF and delta value for clustering.", 2 );
      double delta = PDF( scl );

      COMMENT( "Initializing the labels of the graph nodes.", 2 );
      label.Set( -1 );

      COMMENT( "Clustering.", 2 );
      Vector< double > value( density );
      size_t size = value.size( );
      MinPathFunction< Vector, double > pf( value, &label, nullptr, true, value + delta );
      GrowingBucketQueue queue( size, delta, false, true );
      for( size_t elm = 0; elm < size; ++elm )
        queue.Insert( elm, value[ elm ] );
      if( adjacency.HomogeneousSize( scl ) == 0 ) {
        COMMENT( "LSH IFT.", 2 );
        IF_DEBUG( true ) {
          std::cout << "Prior adjacency sizes: " << std::endl;
          for( size_t elm = 0; elm < size; ++elm )
            std::cout << adjacency.Arcs( scl, elm ) << " ";
          std::cout << std::endl;
        }
        ClusteringIFT< Vector, double > 
          ift( value, &pf, adjacency.HeterogeneousAdjacency( density, scl, delta ),
               adjacency.HeterogeneousSize( scl ), &queue );
        ift.Run( );
      }
      else {
        COMMENT( "KNN IFT.", 2 );
        ClusteringIFT< Vector, double >
          ift( value, &pf, adjacency.HomogeneousAdjacency( ), adjacency.HomogeneousSize( scl ),
               adjacency.HeterogeneousAdjacency( density, scl, delta ), &queue );
        ift.Run( );
      }
      COMMENT( "Sorting value vector.", 2 );
      ordered = Sorting::Sort( value, false );
      COMMENT( "label: " << label, 2 );
      size_t nlabels = label.Maximum( ) + 1;
      COMMENT( "nlabels: " << nlabels << ".", 2 );
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

  template< class GRAPH_ADJACENCY >
  double Graph< GRAPH_ADJACENCY >::NormalizedCut( size_t scl ) const {
    try {
      COMMENT( "Initializing structures.", 2 );
      size_t nlabels = label.Maximum( ) + 1;
      size_t elements = label.size( );
      Vector< double > internal_weight( nlabels ); /* acumulate weights inside each class */
      Vector< double > external_weight( nlabels ); /* acumulate weights between the class and a distinct one */

      COMMENT( "Computing internal and external weights. nlabels: " << nlabels << ", elements: " << elements, 2 );
      for( size_t src = 0; src < elements; ++src ) {
        size_t src_lbl = label( src );
        size_t neighbors = adjacency.Arcs( src, scl );
        COMMENT( "src: " << src << ", src_lbl: " << src_lbl << ", neighbors: " << neighbors, 2 );
        for( size_t ngb = 0; ngb < neighbors; ++ngb ) {
          size_t tgt_lbl = label( adjacency.Arc( src, ngb ) );
          double weight = adjacency.ArcWeight( src, ngb );
          if( src_lbl == tgt_lbl ) {
            internal_weight( src_lbl ) += ( 1.0 / ( 1.0 + weight ) ); /* intra-class weight */
          }
          else {
            external_weight( src_lbl ) += ( 1.0 / ( 1.0 + weight ) ); /* inter-class weight */
          }
        }
      }
      COMMENT( "Computing cut value.", 2 );
      double ncut = 0.0;
      for( size_t lbl = 0; lbl < nlabels; ++lbl ) {
        ncut += external_weight( lbl ) / ( internal_weight( lbl ) + external_weight( lbl ) );
      }
      return( ncut );
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
  Vector< double > Graph< GRAPH_ADJACENCY >::SampleNormalizedCut( const Vector< int > &label, size_t scl )
  const {
    try {
      COMMENT( "Initializing structures.", 2 );
      size_t elements = label.size( );
      Vector< double > cut( elements ); /* cut of each cluster. */

      COMMENT( "Computing internal and external weights for cut.", 2 );
      for( size_t src = 0; src < elements; ++src ) {
        size_t src_lbl = label( src );
        double internal_weight = 0.0; /* acumulate weights inside each class. */
        double external_weight = 0.0; /* acumulate weights between the class and a distinct one. */
        size_t neighbors = adjacency.Arcs( src, scl );
        for( size_t ngb = 0; ngb < neighbors; ++ngb ) {
          size_t tgt_lbl = label( adjacency.Arc( src, ngb ) );
          double weight = adjacency.ArcWeight( src, ngb );
          if( src_lbl == tgt_lbl ) {
            internal_weight += ( 1.0 / ( 1.0 + weight ) ); /* intra-class weight */
          }
          else {
            external_weight += ( 1.0 / ( 1.0 + weight ) ); /* inter-class weight */
          }
        }
        cut( src ) = external_weight / ( internal_weight + external_weight );
      }
      return( cut );
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
  void Graph< GRAPH_ADJACENCY >::SetCut( size_t scl ) {
    try {
      cut = SampleNormalizedCut( label, scl );
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
  template< class D >
  void Graph< GRAPH_ADJACENCY >::PropagateLabel( Feature< D > &feature, size_t scl ) const {
    try {
      COMMENT( "Assigning subset labels.", 1 );
      for( size_t elm = 0; elm < feature.Elements( ); ++elm ) {
        feature.Label( elm ) = -1;
      }
      for( size_t elm = 0; elm < sample.size( ); ++elm ) {
        for( size_t rpt = 0; rpt < sample.size( elm ); ++rpt ) {
          feature.Label( sample( elm, rpt ) ) = label( elm );
        }
      }
      COMMENT( "Propagating other labels.", 1 );
      try {
        size_t total_threads = 12;
        Vector< std::thread > threads;
        for( size_t thd = 0; thd < total_threads; ++thd ) {
          threads.push_back( std::thread( &Graph::PropagateLabelThread< D >, this, std::ref( feature ), scl, thd,
                                          total_threads ) );
        }
        for( size_t thd = 0; thd < total_threads; ++thd ) {
          threads( thd ).join( );
        }
      }
      catch( std::exception &e ) {
        BIAL_WARNING( "Failed to run in multi-thread. Exception: " << e.what( ) );
        PropagateLabelThread( feature, scl, 0, 1 );
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

  template< class GRAPH_ADJACENCY >
  template< class D >
  void Graph< GRAPH_ADJACENCY >::PropagateLabelThread( Feature< D > &feature, size_t scl, size_t thread,
                                                       size_t total_threads ) const {
    try {
      size_t min_spl = thread * feature.Elements( ) / total_threads;
      size_t max_spl = ( thread + 1 ) * feature.Elements( ) / total_threads;

      COMMENT( "Propagating from samples to the entire feature set.", 2 );
      for( size_t spl = min_spl; spl < max_spl; ++spl ) {
        if( feature.Label( spl ) != -1 ) {
          continue;
        }
        size_t bst = 0;
        double min_distance = std::numeric_limits< double >::max( );

        COMMENT( "Getting the label of the most dense sample in range from " << spl, 4 );
        for( size_t adj_idx = 0; adj_idx < ordered.size( ); ++adj_idx ) {
          size_t spl_adj = ordered( adj_idx );
          size_t adj = sample( spl_adj );
          double distance = DFIDE::Distance( feature, feature, spl * feature.Features( ), adj * feature.Features( ),
                                             feature.Features( ) );
          /* double distance = ( *BialDistanceFunction )( &feature( spl, 0 ), &feature( adj, 0 ), feature.Features( ) );
          **/
          if( adjacency.ValidNeighbor( spl_adj, scl, distance ) ) {
            feature.Label( spl ) = feature.Label( adj );
            break;
          }
          if( min_distance > distance ) {
            min_distance = distance;
            bst = adj;
          }
        }
        COMMENT( "If no sample is in range, assign the most dense label. This may be changed by detecting outliers.",
                 4 );
        if( feature.Label( spl ) == -1 ) {
          feature.Label( spl ) = feature.Label( bst );
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

  template< class GRAPH_ADJACENCY >
  template< class D >
  void Graph< GRAPH_ADJACENCY >::GnuPlot2DScatter( const std::string &basename, const Feature< D > &feat, size_t,
                                                   size_t x, size_t y ) {
    try {
      OFile ofile;
      COMMENT( "Opening gnuplot file.", 0 );
      ofile.exceptions( std::fstream::failbit | std::fstream::badbit );
      ofile.open( basename + ".gnp" );
      COMMENT( "Writing to gnuplot file.", 0 );
      ofile << "set palette defined ( 1 \"antiquewhite\", 2 \"aquamarine\", 3 \"beige\", 4 \"black\", 5 \"blue\", " <<
      "6 \"brown\", 7 \"chartreuse\", 8 \"coral\", 9 \"cyan\", 10 \"gold\", 11 \"gray\", 12 \"green\", " <<
      "13\"magenta\", 14\"olive\", 15 \"pink\", 16 \"red\", 17 \"skyblue\", 18 \"slategray\", 19 \"violet\", " <<
      "20 \"yellow\" )" << std::endl;
      ofile << "splot \"" << basename.c_str( ) << ".txt\" u 1:2:3:4 with points palette pt 7" << std::endl;
      /* , \"" << basename.c_str( ) << "_pred.txt\" u 1:2:($3-$1):($4-$2) with vectors" << std::endl; */
      ofile.close( );

      COMMENT( "Opening graph file.", 0 );
      ofile.open( basename + ".txt" );
      COMMENT( "Writing to graph file.", 0 );
      size_t samples = sample.size( );
      for( size_t elm = 0; elm < samples; ++elm ) {
        ofile << feat( sample[ elm ], x ) << " " << feat( sample[ elm ], y ) << " " <<
        density( elm ) << " " << label( elm ) % 20 << std::endl;
      }
      ofile.close( );

      /*
       * COMMENT( "Opening predecessor file.", 0 );
       * ofile.open( basename + "_pred.txt" );
       * COMMENT( "Writing to predecessor file.", 0 );
       * for( size_t src = 0; src < samples; ++src ) {
       *   size_t arcs = adjacency.Arcs( src, scale );
       *   for( size_t adj = 0; adj < arcs; ++adj ) {
       *     size_t tgt = adjacency.Arc( src, adj );
       *     ofile << feat( sample[ src ], x ) << " " << feat( sample[ src ], y ) << " "
       *           << feat( sample[ tgt ], x ) << " " << feat( sample[ tgt ], y ) << std::endl;
       *   }
       * }
       * ofile.close( );
       */
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "I/O error while reading file." ) );
      throw( std::ios_base::failure( msg ) );
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

#ifdef BIAL_EXPLICIT_Graph

  template class Graph< KnnGraphAdjacency >;
  template class Graph< LSHGraphAdjacency >;

  template void Graph< KnnGraphAdjacency >::Initialize( const Feature< int > &feature, float scl_min, float scl_max );
  template void Graph< KnnGraphAdjacency >::PropagateLabel( Feature< int > &feature, size_t scl ) const;
  template void Graph< KnnGraphAdjacency >::PropagateLabelThread( Feature< int > &feature, size_t scl, size_t thread,
                                                                  size_t total_threads ) const;
  template void Graph< KnnGraphAdjacency >::GnuPlot2DScatter( const std::string & basename, const Feature< int > &feat,
                                                              size_t, size_t x, size_t y );

  template void Graph< KnnGraphAdjacency >::Initialize( const Feature< llint > &feature, float scl_min, float scl_max );
  template void Graph< KnnGraphAdjacency >::PropagateLabel( Feature< llint > &feature, size_t scl ) const;
  template void Graph< KnnGraphAdjacency >::PropagateLabelThread( Feature< llint > &feature, size_t scl, size_t thread,
                                                                  size_t total_threads ) const;
  template void Graph< KnnGraphAdjacency >::GnuPlot2DScatter( const std::string & basename, 
                                                              const Feature< llint > &feat, size_t, size_t x, 
                                                              size_t y );

  template void Graph< KnnGraphAdjacency >::Initialize( const Feature< float > &feature, float scl_min, float scl_max );
  template void Graph< KnnGraphAdjacency >::PropagateLabel( Feature< float > &feature, size_t scl ) const;
  template void Graph< KnnGraphAdjacency >::PropagateLabelThread( Feature< float > &feature, size_t scl, size_t thread,
                                                                  size_t total_threads ) const;
  template void Graph< KnnGraphAdjacency >::GnuPlot2DScatter( const std::string & basename, 
                                                              const Feature< float > &feat, size_t, size_t x, 
                                                              size_t y );

  template void Graph< KnnGraphAdjacency >::Initialize( const Feature< double > &feature, float scl_min, 
                                                        float scl_max );
  template void Graph< KnnGraphAdjacency >::PropagateLabel( Feature< double > &feature, size_t scl ) const;
  template void Graph< KnnGraphAdjacency >::PropagateLabelThread( Feature< double > &feature, size_t scl, size_t thread,
                                                                  size_t total_threads ) const;
  template void Graph< KnnGraphAdjacency >::GnuPlot2DScatter( const std::string & basename, 
                                                              const Feature< double > &feat, size_t, size_t x, 
                                                              size_t y );
  
  template void Graph< LSHGraphAdjacency >::Initialize( const Feature< int > &feature, float scl_min, float scl_max );
  template void Graph< LSHGraphAdjacency >::PropagateLabel( Feature< int > &feature, size_t scl ) const;
  template void Graph< LSHGraphAdjacency >::PropagateLabelThread( Feature< int > &feature, size_t scl, size_t thread,
                                                                  size_t total_threads ) const;
  template void Graph< LSHGraphAdjacency >::GnuPlot2DScatter( const std::string & basename, const Feature< int > &feat,
                                                              size_t, size_t x, size_t y );

  template void Graph< LSHGraphAdjacency >::Initialize( const Feature< llint > &feature, float scl_min, float scl_max );
  template void Graph< LSHGraphAdjacency >::PropagateLabel( Feature< llint > &feature, size_t scl ) const;
  template void Graph< LSHGraphAdjacency >::PropagateLabelThread( Feature< llint > &feature, size_t scl, size_t thread,
                                                                  size_t total_threads ) const;
  template void Graph< LSHGraphAdjacency >::GnuPlot2DScatter( const std::string & basename, 
                                                              const Feature< llint > &feat, size_t, size_t x, 
                                                              size_t y );

  template void Graph< LSHGraphAdjacency >::Initialize( const Feature< float > &feature, float scl_min, float scl_max );
  template void Graph< LSHGraphAdjacency >::PropagateLabel( Feature< float > &feature, size_t scl ) const;
  template void Graph< LSHGraphAdjacency >::PropagateLabelThread( Feature< float > &feature, size_t scl, size_t thread,
                                                                  size_t total_threads ) const;
  template void Graph< LSHGraphAdjacency >::GnuPlot2DScatter( const std::string & basename, 
                                                              const Feature< float > &feat, size_t, size_t x, 
                                                              size_t y );

  template void Graph< LSHGraphAdjacency >::Initialize( const Feature< double > &feature, float scl_min, 
                                                        float scl_max );
  template void Graph< LSHGraphAdjacency >::PropagateLabel( Feature< double > &feature, size_t scl ) const;
  template void Graph< LSHGraphAdjacency >::PropagateLabelThread( Feature< double > &feature, size_t scl, size_t thread,
                                                                  size_t total_threads ) const;
  template void Graph< LSHGraphAdjacency >::GnuPlot2DScatter( const std::string & basename, 
                                                              const Feature< double > &feat, size_t, size_t x, 
                                                              size_t y );

#endif

}

#endif

#endif
