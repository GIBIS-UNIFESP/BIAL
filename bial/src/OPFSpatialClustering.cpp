/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/26 
 * @brief OPF spatial clustering algorithms. 
 */

#ifndef BIALOPFSPATIALCLUSTERING_C
#define BIALOPFSPATIALCLUSTERING_C

#include "OPFSpatialClustering.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_OPFSpatialClustering )
#define BIAL_EXPLICIT_OPFSpatialClustering
#endif
#if defined ( BIAL_EXPLICIT_OPFSpatialClustering ) || ( BIAL_IMPLICIT_BIN )

#include "Adjacency.hpp"
#include "AdjacencyIterator.hpp"
#include "DFIDE.hpp"
#include "Feature.hpp"
#include "GrowingBucketQueue.hpp"
#include "Image.hpp"
#include "ImageIFT.hpp"
#include "MinPathFunction.hpp"

namespace Bial {

  template< class D >
  size_t OPF::SpatialClustering( const Feature< D > &feature, Image< int > &label, const Adjacency &adjacency,
                                 float intensity_fraction ) {
    COMMENT( "Sanity check: verify if feature, label and adjacency dimensions are compatible, and if intensity_fraction"
             << " is in the expected range.", 1 );
    size_t elements = label.size( );
    if( feature.Elements( ) > elements ) {
      std::string msg( BIAL_ERROR( "Number of samples in feature vector is greater than label image size. " +
                                   "Given: samples: " + std::to_string( feature.Elements( ) ) + ", label size: " +
                                   std::to_string( elements ) + "." ) );
      throw( std::logic_error( msg ) );
    }
    if( label.Dims( ) != adjacency.Dims( ) ) {
      std::string msg( BIAL_ERROR( "Image and adjacency relation dimensions do not match. Given: Image dimensions: " +
                                   std::to_string( label.Dims( ) ) + ", adjacency dimensions: " +
                                   std::to_string( adjacency.size( ) ) ) );
      throw( std::logic_error( msg ) );
    }
    if( ( intensity_fraction < 0.0 ) || ( intensity_fraction > 1.0 ) ) {
      std::string msg( BIAL_ERROR( "Intensity frequency out of range [0.0, 1.0]. Given: " +
                                   std::to_string( intensity_fraction ) ) );
      throw( std::logic_error( msg ) );
    }
    try {
      COMMENT( "Creating density image for IFT computation.", 0 );
      Image< float > density( label.Dim( ), label.PixelSize( ) );
      COMMENT( "Computing arc weights.", 0 );
      float max_weight = OPF::MaxWeight( feature, label, adjacency, intensity_fraction );
      COMMENT( "max_weight: " << max_weight, 1 );
      COMMENT( "Computing PDF", 0 );
      float delta = OPF::PDF( feature, adjacency, density, max_weight );
      COMMENT( "delta: " << delta, 1 );
      COMMENT( "Clustering.", 1 );
      Image< float > value( density );
      density += delta;
      GrowingBucketQueue queue( elements, delta, false, true );
      COMMENT( "Inserting seeds.", 0 );
      for( size_t elm = 0; elm < elements; ++elm )
        queue.Insert( elm, value[ elm ] );
      MinPathFunction< Image, float > pf( value, &label, nullptr, true, density );
      ImageIFT< float > ift( value, adjacency, &pf, &queue );
      ift.Run( );
      COMMENT( "Getting the number of labels.", 0 );
      size_t nlabels = label.Maximum( ) + 1;
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
  float OPF::MaxWeight( const Feature< D > &feature, Image< int > &label, const Adjacency &adjacency,
                        float intensity_fraction ) {
    try {
      COMMENT( "Computing the maximum arc weight.", 1 );
      size_t total_threads = 12;
      Vector< float > max_distance( total_threads, 0.0 );
      Vector< std::thread > threads;
      for( size_t thd = 0; thd < total_threads; ++thd ) {
        threads.push_back( std::thread( &OPF::MaxWeightThread< D >, std::ref( feature ), std::ref( label ),
                                        std::ref( adjacency ), std::ref( max_distance( thd ) ),
                                        thd, total_threads ) );
      }
      for( size_t thd = 0; thd < total_threads; ++thd ) {
        threads( thd ).join( );
      }
      for( size_t thd = 1; thd < total_threads; ++thd ) {
        max_distance( 0 ) = std::max( max_distance( 0 ), max_distance( thd ) );
      }
      return( static_cast< float >( intensity_fraction ) * max_distance( 0 ) );
    }
    catch( std::exception &e ) {
      BIAL_WARNING( "Failed to run in multi-thread. Exception: " << e.what( ) );
      float max_distance = 0.0;
      MaxWeightThread( feature, label, adjacency, max_distance, 0, 1 );
      return( static_cast< float >( intensity_fraction ) * max_distance );
    }
  }

  template< class D >
  void OPF::MaxWeightThread( const Feature< D > &feature, Image< int > &label, const Adjacency &adjacency,
                             float &max_distance, size_t thread, size_t total_threads ) {
    try {
      size_t size = label.size( );
      size_t adj_size = adjacency.size( );
      size_t min_pxl = thread * size / total_threads;
      size_t max_pxl = ( thread + 1 ) * size / total_threads;
      AdjacencyIterator adj_itr( label, adjacency );
      size_t adj_pxl;
      COMMENT( "Computing distance from all samples.", 2 );
      for( size_t pxl = min_pxl; pxl < max_pxl; ++pxl ) {
        for( size_t idx = 0; idx < adj_size; ++idx ) {
          if( adj_itr.AdjIdx( pxl, idx, adj_pxl ) ) {
            double distance = DFIDE::Distance( feature, pxl * feature.Features( ), adj_pxl * feature.Features( ),
                                               feature.Features( ) );
            /* float distance = ( *BialDistanceFunction )( &feature( pxl, 0 ), &feature( adj_pxl, 0 ), 
               feature.Features( ) ); 
            */
            if( max_distance < distance )
              max_distance = distance;
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

  template< class D >
  float OPF::PDF( const Feature< D > &feature, const Adjacency &adjacency, Image< float > &density,
                  float max_weight ) {
    COMMENT( "Computing sigma or variance of gaussian.", 1 );
    float sigma = ( 2.0 * max_weight / 9.0 );
    if( sigma == 0.0 ) {
      sigma = 1.0;
    }
    COMMENT( "sigma: " << sigma, 2 );
    try {
      size_t total_threads = 12;
      Vector< float > max_dens_diff( total_threads, 0.0 );
      Vector< std::thread > threads;
      for( size_t thd = 0; thd < total_threads; ++thd ) {
        threads.push_back( std::thread( &OPF::PDFThread< D >, std::ref( feature ), std::ref( adjacency ),
                                        std::ref( density ), sigma, std::ref( max_dens_diff( thd ) ), thd,
                                        total_threads ) );
      }
      for( size_t thd = 0; thd < total_threads; ++thd ) {
        threads( thd ).join( );
      }
      for( size_t thd = 1; thd < total_threads; ++thd ) {
        max_dens_diff( 0 ) = std::max( max_dens_diff( 0 ), max_dens_diff( thd ) );
      }
      return( max_dens_diff( 0 ) / 10000.0 );
    }
    catch( std::exception &e ) {
      BIAL_WARNING( "Failed to run in multi-thread. Exception: " << e.what( ) );
      float max_dens_diff = 0.0;
      PDFThread( feature, adjacency, density, sigma, max_dens_diff, 0, 1 );
      return( max_dens_diff / 10000.0 );
    }
  }

  template< class D >
  float OPF::PDFThread( const Feature< D > &feature, const Adjacency &adjacency, Image< float > &density, float sigma,
                        float &max_dens_diff, size_t thread, size_t total_threads ) {
    try {
      size_t adj_size = adjacency.size( );
      size_t size = density.size( );
      size_t min_pxl = thread * size / total_threads;
      size_t max_pxl = ( thread + 1 ) * size / total_threads;
      float mindens = std::numeric_limits< float >::max( );
      float maxdens = std::numeric_limits< float >::min( );
      AdjacencyIterator adj_itr( density, adjacency );
      size_t adj_pxl;
      COMMENT( "Computing nodes density and the minimal and maximal densities.", 2 );
      for( size_t pxl = min_pxl; pxl < max_pxl; ++pxl ) {
        density[ pxl ] = 1.0;
        size_t pixels = 1;
        for( size_t idx = 0; idx < adj_size; ++idx ) {
          if( adj_itr.AdjIdx( pxl, idx, adj_pxl ) ) {
            double arc_weight = DFIDE::Distance( feature, pxl * feature.Features( ), adj_pxl * feature.Features( ),
                                                 feature.Features( ) );
            /* float arc_weight = ( *BialDistanceFunction )( &feature( pxl, 0 ), &feature( adj_pxl, 0 ), feature.Features(
             * ) ); 
             */
            ++pixels;
            density[ pxl ] += exp( -arc_weight / sigma );
          }
        }
        density[ pxl ] /= pixels;
        if( mindens > density[ pxl ] ) {
          mindens = density[ pxl ];
        }
        if( maxdens < density[ pxl ] ) {
          maxdens = density[ pxl ];
        }
        max_dens_diff = maxdens - mindens;
      }
      COMMENT( "min_dens: " << mindens << ", maxdens:" << maxdens, 2 );
      COMMENT( "Computing IFT bucket size.", 1 );
      if( maxdens == mindens ) { /* Hardly happens. */
        return( maxdens );
      }
      else {
        return( maxdens - mindens );
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

#ifdef BIAL_EXPLICIT_OPFSpatialClustering

  template size_t OPF::SpatialClustering( const Feature< int > &feature, Image< int > &label, 
				     const Adjacency &adjacency, float intensity_fraction );
  template float OPF::MaxWeight( const Feature< int > &feature, Image< int > &label,
				 const Adjacency &adjacency, float intensity_fraction );
  template void OPF::MaxWeightThread( const Feature< int > &feature, Image< int > &label,
				      const Adjacency &adjacency, float &max_distance,
				      size_t thread, size_t total_threads );
  template float OPF::PDF( const Feature< int > &feature, const Adjacency &adjacency, Image< float > &density,
			   float max_weight );
  template float OPF::PDFThread( const Feature< int > &feature, const Adjacency &adjacency, 
				 Image< float > &density, float sigma, float &max_dens_diff, size_t thread,
				 size_t total_threads );

  template size_t OPF::SpatialClustering( const Feature< llint > &feature, Image< int > &label, 
				     const Adjacency &adjacency, float intensity_fraction );
  template float OPF::MaxWeight( const Feature< llint > &feature, Image< int > &label,
				 const Adjacency &adjacency, float intensity_fraction );
  template void OPF::MaxWeightThread( const Feature< llint > &feature, Image< int > &label,
				      const Adjacency &adjacency, float &max_distance,
				      size_t thread, size_t total_threads );
  template float OPF::PDF( const Feature< llint > &feature, const Adjacency &adjacency, Image< float > &density,
			   float max_weight );
  template float OPF::PDFThread( const Feature< llint > &feature, const Adjacency &adjacency, 
				 Image< float > &density, float sigma, float &max_dens_diff, size_t thread,
				 size_t total_threads );

  template size_t OPF::SpatialClustering( const Feature< float > &feature, Image< int > &label,
                                          const Adjacency &adjacency, float intensity_fraction );
  template float OPF::MaxWeight( const Feature< float > &feature, Image< int > &label,
				 const Adjacency &adjacency, float intensity_fraction );
  template void OPF::MaxWeightThread( const Feature< float > &feature, Image< int > &label,
				      const Adjacency &adjacency, float &max_distance,
				      size_t thread, size_t total_threads );
  template float OPF::PDF( const Feature< float > &feature, const Adjacency &adjacency, Image< float > &density,
			   float max_weight );
  template float OPF::PDFThread( const Feature< float > &feature, const Adjacency &adjacency, 
				 Image< float > &density, float sigma, float &max_dens_diff, size_t thread,
				 size_t total_threads );

  template size_t OPF::SpatialClustering( const Feature< double > &feature, Image< int > &label, 
				     const Adjacency &adjacency, float intensity_fraction );
  template float OPF::MaxWeight( const Feature< double > &feature, Image< int > &label,
				 const Adjacency &adjacency, float intensity_fraction );
  template void OPF::MaxWeightThread( const Feature< double > &feature, Image< int > &label,
				      const Adjacency &adjacency, float &max_distance,
				      size_t thread, size_t total_threads );
  template float OPF::PDF( const Feature< double > &feature, const Adjacency &adjacency, Image< float > &density,
			   float max_weight );
  template float OPF::PDFThread( const Feature< double > &feature, const Adjacency &adjacency, 
				 Image< float > &density, float sigma, float &max_dens_diff, size_t thread,
				 size_t total_threads );

#endif

}

#endif

#endif
