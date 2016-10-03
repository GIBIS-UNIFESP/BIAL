/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/26 
 * Content: LSH graph adjacency.
 */

#ifndef BIALLSHGRAPHADJACENCY_C
#define BIALLSHGRAPHADJACENCY_C

#include "LSHGraphAdjacency.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_LSHGraphAdjacency )
#define BIAL_EXPLICIT_LSHGraphAdjacency
#endif

#if defined ( BIAL_EXPLICIT_LSHGraphAdjacency ) || ( BIAL_IMPLICIT_BIN )

#include "DFIDE.hpp"
#include "Feature.hpp"
#include "Sample.hpp"

namespace Bial {

  LSHGraphAdjacency::~LSHGraphAdjacency( ) {
    COMMENT( "Freeing LSH internal structure.", 0 );
    if( initialized ) {
      LSH_Free( );
      initialized = false;
    }
  }

  template< class D >
  void LSHGraphAdjacency::Initialize( const Feature< D > &feature, const Sample &sample, float scl_min, 
                                      float scl_max ) {
    try {
      COMMENT( "Computing the radius based on the given scale.", 1 );
      EstimateRadius( feature, scl_min, scl_max );
      size_t scales = scale.size( );
      COMMENT( "Scales: " << scale, 2 );
      COMMENT( "Initializing other structures.", 1 );
      size_t elements = sample.size( );
      COMMENT( "Samples: " << elements << ", rmax: " << scale( scales - 1 ), 1 );
      arc = Vector< Vector< size_t > >( elements );
      arc_weight = Vector< Vector< double > >( elements );
      adjacents = Vector< Vector< size_t > >( scales, Vector< size_t >( elements, 0 ) );
      COMMENT( "Picking the correct feature vector.", 1 );
      Feature< int > subfeature( Feature< int >( feature, sample ) );
      COMMENT( "Used feature elements: " << subfeature.Elements( ) << ", features: " << subfeature.Features( ), 1 );
      COMMENT( "Features: " << subfeature, 3 );
      COMMENT( "Initializing LSH.", 1 );
      float rmax = scale( scales - 1 );
      if( !initialized ) {
        LSH_Init( &subfeature( 0, 0 ), elements, subfeature.Features( ), &rmax, 1 );
        initialized = true;
      }
      COMMENT( "Creating LSH structures.", 1 );
      Vector< int > index( elements - 1 );
      Vector< float > distance( elements - 1 );
      COMMENT( "Computing the adjacent samples.", 1 )
      for( size_t src = 0; src < elements; ++src ) {
        /*
         * COMMENT( "Setting repeated samples to zero.", 4 );
         * size_t equal_samples = sample.size( src ) - 1;
         * for( size_t adj = 0; adj < equal_samples; ++adj ) {
         *   arc_weight( src ).push_back( 0.0 );
         *   arc( src ).push_back( src );
         * }
         */
        COMMENT( "Getting adjacents in radius by LSH.", 4 );
        size_t adjs = LSH_Query( src, rmax, &( index( 0 ) ), &( distance( 0 ) ) );
        COMMENT( "Copying adjacents to LSHGraphAdjacency.", 4 );
        size_t scl = 0;
        for( size_t adj = 0; adj < adjs; ++adj ) {
          arc( src ).push_back( index( adj ) );
          arc_weight( src ).push_back( distance( adj ) );
          COMMENT( "Computing the number of adjacents for a given scale for src sample.", 4 );
          while( ( scl < scales ) && ( scale( scl ) < distance( adj ) ) ) {
            adjacents( scl ) ( src ) = adj;
            ++scl;
          }
        }
        COMMENT( "Setting the number of adjacents for the scales that contain them all.", 4 );
        while( scl < scales ) {
          adjacents( scl ) ( src ) = adjs;
          ++scl;
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

  size_t LSHGraphAdjacency::Scales( ) const {
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

  double LSHGraphAdjacency::Sigma( size_t scl ) const {
    try {
      COMMENT( "scale[ " << scl << " ]: " << scale( scl ), 2 );
      return( scale( scl ) * scale( scl ) / 3.0 );
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

  size_t LSHGraphAdjacency::Arcs( size_t src, size_t scl ) const {
    try {
      return( adjacents( scl ) ( src ) );
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

  size_t LSHGraphAdjacency::Arc( size_t src, size_t adj ) const {
    try {
      return( arc( src ) ( adj ) );
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

  double LSHGraphAdjacency::ArcWeight( size_t src, size_t adj ) const {
    try {
      return( arc_weight( src ) ( adj ) );
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
  void LSHGraphAdjacency::EstimateRadius( const Feature< D > &feature, float scl_min, float scl_max ) {
    try {
      COMMENT( "Creating returning vector and auxiliaty vector.", 1 );
      Vector< double > radius;
      COMMENT( "Computing the radius of 4000 pairs of elements to estimate the expected radius.", 1 );
      for( size_t cont = 0; cont < 4000; ++cont ) {
        COMMENT( "Getting random elements.", 4 );
        size_t src_elm = static_cast< size_t >( rand( ) /
                                                static_cast< double >( RAND_MAX ) * feature.Elements( ) - 0.000001 );
        size_t tgt_elm = static_cast< size_t >( rand( ) /
                                                static_cast< double >( RAND_MAX ) * feature.Elements( ) - 0.000001 );
        COMMENT( "Computing the distance between them and inserting into the vector.", 4 );
        double dist = DFIDE::Distance( feature, feature, src_elm * feature.Features( ), tgt_elm * feature.Features( ),
                                       feature.Features( ) );
        /* double dist = ( *BialDistanceFunction )( &feature( src_elm, 0 ), &feature( tgt_elm, 0 ), feature.Features( )
         * ); */
        COMMENT( "Sorting the new element within the vector.", 4 );
        size_t elm;
        for( elm = 0; elm < radius.size( ); ++elm ) {
          if( dist < radius( elm ) ) {
            std::swap( dist, radius( elm ) );
          }
          else if( dist == radius( elm ) ) {
            break;
          }
        }
        COMMENT( "Checking if new distance is repeated. If not, insert into radius vector.", 4 );
        if( elm == radius.size( ) ) {
          radius.push_back( dist );
        }
      }
      COMMENT( "Radius: " << radius, 3 );
      COMMENT( "Getting first non-zero element and true size.", 1 );
      if( radius( 0 ) == 0.0 ) {
        radius.erase( radius.begin( ) );
      }
      size_t size = radius.size( );
      COMMENT( "Computing the power that satisfies: ( 0.1 * size ^ ( 1 / p ) ) ^ p = 2, so r = 0.1 implies k = 2.", 1 );
      float power = std::log( 2.0f / size ) / std::log( 0.1f );
      COMMENT( "Getting the required element indexes.", 1 );
      size_t min_index = static_cast< size_t >( std::pow( std::pow( size, 1.0f / power ) * scl_min, power ) + 0.5f );
      size_t max_index = static_cast< size_t >( std::pow( std::pow( size, 1.0f / power ) * scl_max, power ) + 0.5f );
      COMMENT( "Inserting elements into the resultant vector.", 1 );
      COMMENT( "Radius size: " << radius.size( ) << ", min_index: " << min_index << ", max_index: " << max_index, 1 );
      size_t displacement = 1;
      size_t count = 0;
      for( size_t elm = min_index; elm <= max_index; elm += displacement ) {
        COMMENT( "Displacement:" << displacement, 3 );
        scale.push_back( radius( elm ) );
        ++count;
        if( count == 10 ) {
          count = 0;
          displacement *= 3;
        }
      }
      if( scale.size( ) == 0 ) {
        std::string msg( BIAL_ERROR(
                           "No radius was found to estimate feature vector adjacents. This probably happened "
                           "because your data is too homogeneous for clustering." ) );
        throw( std::logic_error( msg ) );
      }
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

  Vector< Vector< size_t > > &LSHGraphAdjacency::HeterogeneousAdjacency( const Vector< double > &, size_t, double ) {
    return( arc );
  }

  const Matrix< size_t > &LSHGraphAdjacency::HomogeneousAdjacency( ) const {
    return( null_matrix );
  }

  size_t LSHGraphAdjacency::HomogeneousSize( size_t ) const {
    return( 0 );
  }

  const Vector< size_t > &LSHGraphAdjacency::HeterogeneousSize( size_t scl ) const {
    try {
      return( adjacents( scl ) );
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


  bool LSHGraphAdjacency::ValidNeighbor( size_t, size_t scl, double distance ) const {
    try {
      return( distance <= scale( scl ) );
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
  const size_t LSHGraphAdjacency::MAX_SAMPLES = 10000;

  /* Initializing LSH global variable */
  bool LSHGraphAdjacency::initialized = false;

#ifdef BIAL_EXPLICIT_LSHGraphAdjacency

  template class GraphAdjacency< LSHGraphAdjacency >;

  template void LSHGraphAdjacency::Initialize( const Feature< int > &feature, const Sample &sample, float scl_min, 
                                               float scl_max );
  template void LSHGraphAdjacency::EstimateRadius( const Feature< int > &feature, float scl_min, float scl_max );
  template void LSHGraphAdjacency::Initialize( const Feature< llint > &feature, const Sample &sample, float scl_min,
                                               float scl_max );
  template void LSHGraphAdjacency::EstimateRadius( const Feature< llint > &feature, float scl_min, float scl_max );
  template void LSHGraphAdjacency::Initialize( const Feature< float > &feature, const Sample &sample, float scl_min,
                                               float scl_max );
  template void LSHGraphAdjacency::EstimateRadius( const Feature< float > &feature, float scl_min, float scl_max );
  template void LSHGraphAdjacency::Initialize( const Feature< double > &feature, const Sample &sample, float scl_min,
                                               float scl_max );
  template void LSHGraphAdjacency::EstimateRadius( const Feature< double > &feature, float scl_min, float scl_max );

#endif

}

#endif

#endif
