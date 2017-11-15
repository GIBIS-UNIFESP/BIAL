/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/26 
 * Content: Knn graph adjacency.
 */

#ifndef BIALKNNGRAPHADJACENCY_C
#define BIALKNNGRAPHADJACENCY_C

#include "KnnGraphAdjacency.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_KnnGraphAdjacency )
#define BIAL_EXPLICIT_KnnGraphAdjacency
#endif

#if defined ( BIAL_EXPLICIT_KnnGraphAdjacency ) || ( BIAL_IMPLICIT_BIN )

#include "DFIDE.hpp"
#include "Feature.hpp"
#include "Sample.hpp"

namespace Bial {

  template< class D >
  void KnnGraphAdjacency::Initialize( const Feature< D > &feature, const Sample &sample, float scl_min, 
                                      float scl_max ) {
    try {
      COMMENT( "Computing the number of neighbors based on the given scale.", 1 );
      EstimateK( feature, scl_min, scl_max );
      size_t scales = scale.size( );
      COMMENT( "Scales: " << scale, 2 );
      COMMENT( "Initializing other structures.", 1 );
      size_t elements = sample.size( );
      size_t kmax = scale( scales - 1 );
      COMMENT( "Samples: " << elements << ", kmax: " << kmax, 1 );
      arc = Matrix< size_t >( elements, kmax );
      arc_weight = Matrix< double >( elements, kmax );
      null_heterogeneous_size = Vector< size_t >( 0 );
      COMMENT( "Picking the correct feature vector.", 1 );
      Feature< D > subfeature = elements < feature.Elements( ) ? Feature< D >( feature, sample ) :
        Feature< D >( 1, 1 );
      const Feature< D > &used_feature = elements < feature.Elements( ) ? subfeature : feature;
      COMMENT( "Used feature elements: " << used_feature.Elements( ) << ", features: " << used_feature.Features( ), 1 );
      COMMENT( "Features: " << used_feature, 3 );
      COMMENT( "Computing the adjacent samples.", 1 );
      for( size_t src = 0; src < elements; ++src ) {
        COMMENT( "Setting repeated samples to zero.", 3 );
        size_t equal_samples = std::min( sample.size( src ) - 1, kmax );
        COMMENT( "equal_samples: " << equal_samples << ", kmax: " << kmax, 3 );
        for( size_t knn = 0; knn < equal_samples; ++knn ) {
          arc_weight( src, knn ) = 0.0;
          arc( src, knn ) = src;
        }
        for( size_t knn = equal_samples; knn < kmax; ++knn ) {
          arc_weight( src, knn ) = std::numeric_limits< double >::max( );
        }
        COMMENT( "Checking if more arcs are necessary as there are not enough equal features.", 4 );
        if( equal_samples < kmax ) {
          COMMENT( "Compute and sort the nearst kmax of each node.", 4 );
          for( size_t tgt = 0; tgt < elements; ++tgt ) {
            if( tgt != src ) {
              COMMENT( "src: " << src << ", tgt: " << tgt << ", dms: " << used_feature.Features( ), 4 );
              double dist = DFIDE::Distance( used_feature, src * used_feature.Features( ),
                                             tgt * used_feature.Features( ), used_feature.Features( ) );
              /*
               * double dist = ( *BialDistanceFunction )( &used_feature( src, 0 ), &used_feature( tgt, 0 ),
               * used_feature.Features( ) );
               */
              size_t index = tgt;
              for( size_t knn = equal_samples; knn < kmax; ++knn ) {
                if( dist < arc_weight( src, knn ) ) {
                  std::swap( dist, arc_weight( src, knn ) );
                  std::swap( index, arc( src, knn ) );
                  if( dist == std::numeric_limits< double >::max( ) ) {
                    break;
                  }
                }
              }
            }
          }
        }
      }
      COMMENT( "Graph arcs: " << arc, 3 );
      COMMENT( "Arc weights: " << arc_weight, 3 );
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

  size_t KnnGraphAdjacency::Scales( ) const {
    try {
      return( scale.size( ) );
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

  double KnnGraphAdjacency::Sigma( size_t scl ) const {
    try {
      double mean_weight = 0.0;
      size_t elements = arc.size( 0 );
      COMMENT( "Computing mean weight for variance entimation. ", 2 );
      COMMENT( "Elements: " << elements << ", scale[ " << scl << " ]: " << scale( scl ) << ".", 2 );
      for( size_t elm = 0; elm < elements; ++elm ) {
        mean_weight += arc_weight( elm, scale( scl ) - 1 );
      }
      mean_weight /= elements;
      return( mean_weight * mean_weight );
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

  size_t KnnGraphAdjacency::Arcs( size_t, size_t scl ) const {
    try {
      return( scale( scl ) );
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

  size_t KnnGraphAdjacency::Arc( size_t src, size_t adj ) const {
    try {
      return( arc( src, adj ) );
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

  double KnnGraphAdjacency::ArcWeight( size_t src, size_t adj ) const {
    try {
      return( arc_weight( src, adj ) );
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
  void KnnGraphAdjacency::EstimateK( const Feature< D > &feature, float scl_min, float scl_max ) {
    try {
      COMMENT( "Getting the maximum number of neighbors given params: " << scl_min << " to " << scl_max << ".", 1 );
      size_t size = std::min( feature.Elements( ), MAX_SAMPLES ) - 1;
      COMMENT( "Computing the power that satisfies: ( 0.1*size^( 1/p ) )^p = 2, so r = 0.1 implies k = 2.", 1 );
      float power = std::log( 2.0f / size ) / std::log( 0.1f );
      COMMENT( "size: " << size << ", power: " << power << ". Getting the required element indexes.", 1 );
      size_t min_index = static_cast< size_t >( std::pow( std::pow( size, 1.0f / power ) * scl_min, power ) + 0.5f );
      if( min_index == 0 ) {
        min_index = 1;
      }
      size_t max_index = static_cast< size_t >( std::pow( std::pow( size, 1.0f / power ) * scl_max, power ) + 0.5f );
      COMMENT( "min_index: " << min_index << ", max_index: " << min_index, 1 );
      COMMENT( "Inserting elements into the resultant vector.", 1 );
      size_t displacement = 1;
      size_t count = 0;
      for( size_t elm = min_index; elm <= max_index; elm += displacement ) {
        COMMENT( "Displacement:" << displacement, 3 );
        scale.push_back( elm );
        ++count;
        if( count == 10 ) {
          count = 0;
          displacement *= 3;
        }
      }
      COMMENT( "scale: " << scale, 1 );
      if( scale.size( ) == 0 ) {
        std::string msg( BIAL_ERROR(
                           "No radius was found to estimate feature vector adjacents. This probably happened "
                           "because your data is too homogeneous for clustering." ) );
        throw( std::logic_error( msg ) );
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

  Vector< Vector< size_t > > &KnnGraphAdjacency::HeterogeneousAdjacency( const Vector< double > &density,
                                                                         size_t scl, double delta ) {
    try {
      COMMENT( "Initializing heterogeneous adjacency.", 2 );
      size_t elements = arc.size( 0 );
      plateau = Vector< Vector< size_t > >( elements, Vector< size_t >( ) );
      size_t neighbors = scale( scl );
      COMMENT( "Add arcs to guarantee symmetry on plateaus.", 2 );
      for( size_t src = 0; src < elements; ++src ) {
        for( size_t knn = 0; knn < neighbors; ++knn ) {
          size_t tgt = arc( src, knn );
          if( ( density( src ) + delta >= density( tgt ) ) &&
              ( density( src ) - delta <= density( tgt ) ) ) {
            COMMENT( "Insert src in heterogeneous adjacency of tgt, if src is not in knn adjacency of tgt.", 3 );
            bool insert_src = true;
            for( size_t bck = 0; bck < neighbors; ++bck ) {
              if( arc( tgt, bck ) == src ) {
                insert_src = false;
                break;
              }
            }
            if( insert_src ) {
              plateau( tgt ).push_back( src );
            }
          }
        }
      }
      return( plateau );
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

  const Matrix< size_t > &KnnGraphAdjacency::HomogeneousAdjacency( ) const {
    return( arc );
  }

  size_t KnnGraphAdjacency::HomogeneousSize( size_t scl ) const {
    try {
      return( scale( scl ) );
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

  const Vector< size_t > &KnnGraphAdjacency::HeterogeneousSize( size_t ) const {
    return( null_heterogeneous_size );
  }

  bool KnnGraphAdjacency::ValidNeighbor( size_t src, size_t scl, double distance ) const {
    try {
      return( distance <= arc_weight( src, scale( scl ) - 1 ) );
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

  /* Initializing Graphs Maximum samples. */
  const size_t KnnGraphAdjacency::MAX_SAMPLES = 10000;


#ifdef BIAL_EXPLICIT_KnnGraphAdjacency

  template class GraphAdjacency< KnnGraphAdjacency >;

  template void KnnGraphAdjacency::Initialize( const Feature< int > &feature, const Sample &sample, float scl_min, 
                                               float scl_max );
  template void KnnGraphAdjacency::EstimateK( const Feature< int > &feature, float scl_min, float scl_max );
  template void KnnGraphAdjacency::Initialize( const Feature< llint > &feature, const Sample &sample, float scl_min,
                                               float scl_max );
  template void KnnGraphAdjacency::EstimateK( const Feature< llint > &feature, float scl_min, float scl_max );
  template void KnnGraphAdjacency::Initialize( const Feature< float > &feature, const Sample &sample, float scl_min,
                                               float scl_max );
  template void KnnGraphAdjacency::EstimateK( const Feature< float > &feature, float scl_min, float scl_max );
  template void KnnGraphAdjacency::Initialize( const Feature< double > &feature, const Sample &sample, float scl_min,
                                               float scl_max );
  template void KnnGraphAdjacency::EstimateK( const Feature< double > &feature, float scl_min, float scl_max );

#endif

}

#endif

#endif
