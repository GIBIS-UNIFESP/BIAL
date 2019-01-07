/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2018/Nov/07 */
/* Content: Test file. */
/* Description: Computes sets of Edge and Flat pixels with standard deviation different of zero in their adjacency. */

#ifndef BIALFILTERING_C
#define BIALFILTERING_C

#include "AdjacencyIterator.hpp"
#include "AdjacencyRound.hpp"
#include "Common.hpp"
#include "DiffusionFunction.hpp"
#include "Filtering.hpp"
#include "FilteringMedian.cpp"
#include "Image.hpp"
#include "ImageFrame.hpp"

using namespace std;
using namespace Bial;

namespace Bial {

template< class D >
bool NoisyPixels( const Image< D > &img, Image< D > &noise_mask ) {
  try {
    Adjacency adj( AdjacencyType::HyperSpheric( 1.8, img.Dims( ) ) );
    AdjacencyIterator adj_itr( img, adj );
    size_t img_size = img.size( );
    size_t adj_size = adj.size( );
    size_t adj_pxl;
    size_t noise_pixels = 0;
    size_t zero_diff_pixels = 0;
    D img_min = img.Minimum( );
    Vector< D > noise_hist( img.Maximum( ) + 1 );
    for( size_t src_pxl = 0; src_pxl < img_size; ++src_pxl ) {
      int img_lower = 0;
      int img_higher = 0;
      D diff = 0;
      for( size_t adj_idx = 1; adj_idx < adj_size; ++adj_idx ) {
	if( adj_itr.AdjIdx( src_pxl, adj_idx, adj_pxl ) ) {
	  if( img[ src_pxl ] < img[ adj_pxl ] )
	    ++img_lower;
	  if( img[ src_pxl ] > img[ adj_pxl ] )
	    ++img_higher;
	  diff += std::abs( img[ src_pxl ] - img[ adj_pxl ] );
	}
      }
      COMMENT( "Computing the number of zero diff pixels: ", 0 );
      if( ( diff / adj_size < 2 ) && ( img[ src_pxl ] > 0.01 * img_min ) )
	++zero_diff_pixels;
      COMMENT( "Checking if src_pxl is noise pixel.", 3 );
      if( static_cast< size_t >( img_lower + img_higher ) == adj_size - 1 ) {
	if( ( img_lower == 0 ) || ( img_higher == 0 ) ) {
	  noise_mask[ src_pxl ] = 1;
	  ++noise_pixels;
	  ++noise_hist[ diff / ( adj_size - 1 ) ];
	}
      }
    }
    if( noise_pixels < zero_diff_pixels ) {
      std::cout << "No noise detected." << std::endl;
      return( false );
    }
    COMMENT( "Computing 10% of higher difference of intensity noise pixels.", 0 );
    size_t max_noise_pixels = static_cast< size_t >( std::min( static_cast< int >( noise_pixels ), std::max( static_cast< int >( 1000 ), static_cast< int >( noise_pixels * 0.9 ) ) ) );
    size_t count_noise_pixels = 0;
    size_t noise_pixel_index = 0;
    while( noise_pixel_index < noise_hist.size( ) ) {
      count_noise_pixels += noise_hist[ noise_pixel_index ];
      ++noise_pixel_index;
      if( count_noise_pixels > max_noise_pixels )
    	break;
    }
    //size_t noise_removed = 0;
    for( size_t src_pxl = 0; src_pxl < img_size; ++src_pxl ) {
      COMMENT( "Removing low contrast noise pixels.", 3 );
      if( noise_mask[ src_pxl ] == 1 ) {
    	D diff = 0;
    	for( size_t adj_idx = 1; adj_idx < adj_size; ++adj_idx ) {
    	  adj_pxl = adj_itr( src_pxl, adj_idx );
    	  diff += std::abs( img[ src_pxl ] - img[ adj_pxl ] );
    	}
    	if( diff / ( adj_size - 1 ) < noise_pixel_index ) {
    	  noise_mask[ src_pxl ] = 0;
    	  //++noise_removed;
    	  --noise_pixels;
    	}
      }
    }
    //std::cout << "noisy pixels: " << noise_pixels << ", noisy pixels removed: " << noise_removed << std::endl;
    std::cout << "zero_diff_pixels: " << zero_diff_pixels << ", img_size: " << img_size << ", noisy pixels: " << noise_pixels << std::endl;
    if( noise_pixels < 1000 ) {
      std::cout << "Too few noisy pixels." << std::endl;
      return( false );
    }
    return( true );
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
void PatchSimilarityMap( Vector< Image< D > > &map, const Image< D > &framed_img, const Vector< size_t > &img_dim,
			 const float search_radius, const float patch_radius, const size_t patch_adjacents,
			 const size_t thread, const size_t total_threads ) {
  COMMENT( "Dealing with thread limits.", 3 );
  size_t img_dims = ( img_dim[ 2 ] == 1 ? 2 : 3 );
  size_t img_xy_size = img_dim[ 0 ] * img_dim[ 1 ];
  size_t min_z = 0;
  size_t max_z = img_dim[ 2 ];
  size_t min_y = 0;
  size_t max_y = img_dim[ 1 ];
  if( img_dims == 3 ) {
    min_z = thread * max_z / total_threads;
    max_z = ( thread + 1 ) * max_z / total_threads;
  }
  else {
    min_y = thread * max_y / total_threads;
    max_y = ( thread + 1 ) * max_y / total_threads;
  }
  COMMENT( "Basic variables.", 3 );
  Adjacency search_adj( AdjacencyType::HyperSpheric( search_radius, img_dims ) ); // adjacency with size given by the radius for searching similar patches.
  Adjacency patch_adj( AdjacencyType::HyperSpheric( patch_radius, img_dims ) ); // adjacency with size given by the radius which defines the size of the patch.
  size_t search_adj_size = search_adj.size( );
  size_t patch_adj_size = patch_adj.size( );
  size_t frame_size = static_cast< size_t >( std::ceil( search_radius + patch_radius ) );
  AdjacencyIterator search_adj_itr( framed_img, search_adj );
  AdjacencyIterator patch_adj_itr( framed_img, patch_adj );
  size_t target_pxl;
  size_t target_adj_pxl;
  size_t source_adj_pxl;
  Vector< int > target_patch_dist( search_adj_size, 0 );
  Vector< size_t > target_patch_index( search_adj_size, 0 );
  size_t source_pxl_z = 0;
  if( img_dims == 3 )
    source_pxl_z = ( min_z + frame_size ) * framed_img.Displacement( 1 );
  for( size_t z = min_z; z < max_z; ++z, source_pxl_z += framed_img.Displacement( 1 ) ) {
    size_t source_pxl_yz = source_pxl_z + frame_size * framed_img.size( 0 );
    for( size_t y = min_y; y < max_y; ++y, source_pxl_yz += framed_img.size( 0 ) ) {
      size_t source_pxl = source_pxl_yz + frame_size;
      for( size_t x = 0; x < img_dim[ 0 ]; ++x, ++source_pxl ) {
	target_patch_dist = 0;
	target_patch_index = 0;
	for( size_t search_adj_idx = 1; search_adj_idx < search_adj_size; ++search_adj_idx ) {
	  COMMENT( "For each valid position, compare source and target patches.", 4 );
	  target_pxl = search_adj_itr( source_pxl, search_adj_idx );
      target_patch_index[ 0 ] = x + search_adj( search_adj_idx, 0 ) + ( y + search_adj( search_adj_idx, 1 ) ) * img_dim[ 0 ];
	  if( img_dims == 3 )
	    target_patch_index[ 0 ] += ( z + search_adj( search_adj_idx, 2 ) ) * img_xy_size;
	  for( size_t patch_adj_idx = 0; patch_adj_idx < patch_adj_size; ++patch_adj_idx ) {
	    source_adj_pxl = patch_adj_itr( source_pxl, patch_adj_idx );
	    target_adj_pxl = patch_adj_itr( target_pxl, patch_adj_idx );
	    target_patch_dist[ 0 ] += std::abs( framed_img[ source_adj_pxl ] - framed_img[ target_adj_pxl ] );
	  }
	  COMMENT( "Sorting target patches by number of common elements (inc) followed by distance (dec).", 4 );
	  for( size_t sort_adj_idx = 1; sort_adj_idx < search_adj_idx; ++sort_adj_idx ) {
	    if( target_patch_dist[ 0 ] > target_patch_dist[ sort_adj_idx ] ) {
	      std::swap( target_patch_dist[ 0 ], target_patch_dist[ sort_adj_idx ] );
	      std::swap( target_patch_index[ 0 ], target_patch_index[ sort_adj_idx ] );
	    }
	  }
	  COMMENT( "Assigning last patch data to last position.", 4 );
	  std::swap( target_patch_dist[ 0 ], target_patch_dist[ search_adj_idx ] );
	  std::swap( target_patch_index[ 0 ], target_patch_index[ search_adj_idx ] );
	}
	COMMENT( "Assigning the index of the best patches.", 3 );
	for( size_t adj_index = 0; adj_index < patch_adjacents; ++adj_index )
	  map[ adj_index ]( x, y, z ) = target_patch_index[ search_adj_size - 1 - adj_index ];
      }
    }
  }
}

Vector< double > NonLocalWeightVector( const Adjacency &adj, const size_t adj_size, const size_t img_dims, const float patch_distance ) {
  try {
    COMMENT( "Creating weight applied to each adjacent pixel based to its relative position to the filtered pixel.", 2 );
    Vector< double > weight( 2 * adj_size, 0.0 );
    COMMENT( "Local adjacents.", 0 );
    for( size_t adj_idx = 1; adj_idx < adj_size; ++adj_idx ) {
      COMMENT( "Computing L2 distance.", 4 );
      double distance = 0.0;
      for( size_t dim = 0; dim < img_dims; ++dim )
	distance += adj( adj_idx, dim ) * adj( adj_idx, dim );
      weight[ adj_idx ] = 1.0 / std::sqrt( distance );
    }
    COMMENT( "Non-local adjacents.", 0 );
    for( size_t adj_idx = 0; adj_idx < adj_size; ++adj_idx ) {
      COMMENT( "Computing L2 distance.", 4 );
      double distance = patch_distance * patch_distance;
      for( size_t dim = 0; dim < img_dims; ++dim )
	distance += adj( adj_idx, dim ) * adj( adj_idx, dim );
      weight[ adj_idx + adj_size ] = 1.0 / std::sqrt( distance );
    }
    return( weight );
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

double NonLocalIntegrationConstant( const Adjacency &adj, const size_t adj_size, const size_t img_dims,
				    const size_t patch_adjacents, const float patch_distance ) {
  try {
    COMMENT( "Computing integration constant.", 3 );
    double integration_constant = 0.0;
    COMMENT( "Local contribution.", 3 );
    for( size_t idx = 1; idx < adj_size; ++idx ) {
      double distance = 0.0;
      for( size_t dim = 0; dim < img_dims; ++dim )
	distance += std::abs( adj( idx, dim ) );
      integration_constant += 1.0 / distance;
    }
    COMMENT( "Non-local contribution.", 3 );
    for( size_t idx = 0; idx < adj_size; ++idx ) {
      double distance = patch_distance;
      for( size_t dim = 0; dim < img_dims; ++dim )
	distance += std::abs( adj( idx, dim ) );
      integration_constant += patch_adjacents / distance;
    }
    integration_constant = 1.0 / integration_constant;
    return( integration_constant );
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
float NonLocalFlatRegionKappa( const Image< D > &source, const Image< D > &mask, const Adjacency &adj, const AdjacencyIterator &adj_itr ) {
  try {
    COMMENT( "Computing mask vector.", 3 );
    Vector< size_t > mask_vector;
    for( size_t pxl = 0; pxl < mask.size( ); ++pxl ) {
      if( mask[ pxl ] != 0 )
	mask_vector.push_back( pxl );
    }
    size_t mask_vector_size = mask_vector.size( );
    COMMENT( "edges pixel vector.", 3 );
    Vector< int > edges( mask_vector_size );
    Vector< float > kappas;
    Vector< float > stds;
    Image< D > filtered( source.Dim( ), source.PixelSize( ) );
    ullint diff_sum = 0;
    for( size_t pxl_idx = 0; pxl_idx < mask_vector_size; ++pxl_idx ) {
      size_t src_pxl = mask_vector[ pxl_idx ];
      for( size_t adj_idx = 1; adj_idx < adj.size( ); ++adj_idx ) {
	size_t adj_pxl = adj_itr( src_pxl, adj_idx );
	diff_sum += std::abs( source[ src_pxl ] - source[ adj_pxl ] );
      }
    }
    double mean_diff = ( diff_sum /= adj.size( ) ) / static_cast< double >( mask_vector_size );
    std::cout << "Initial flat region mean difference: " << mean_diff << std::endl;
    return( static_cast< float >( mean_diff ) );
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
void QuickNonLocalAnisotropicDiffusionThread( const Image< D > &img, Image< D > &res,
					      const RobustDiffusionFunction &diff_func, float kappa,
					      const Adjacency &adj, const AdjacencyIterator &adj_itr,
					      const Vector< double > &weight, double integration_constant,
					      const Vector< Image< D > > &map, size_t thread,
					      size_t total_threads ) {
  try {
    COMMENT( "Dealing with thread limits.", 3 );
    size_t img_size = img.size( );
    size_t min_index = thread * img_size / total_threads;
    size_t max_index = ( thread + 1 ) * img_size / total_threads;
    size_t adj_size = adj.size( );
    size_t adj_pxl;
    for( size_t pxl = min_index; pxl < max_index; ++pxl ) {
      COMMENT( "Computing intensity flow from adjacents.", 4 );
      double flow = 0.0;
      for( size_t adj_idx = 1; adj_idx < adj_size; ++adj_idx ) {
	if( adj_itr.AdjIdx( pxl, adj_idx, adj_pxl ) ) {
	  D grad = img[ adj_pxl ] - img[ pxl ];
	  flow += weight[ adj_idx ] * grad * diff_func( kappa, grad );
	}
      }
      for( size_t nonlocal_idx = 0; nonlocal_idx < map.size( ); ++nonlocal_idx ) {
	size_t nonlocal_pxl = map[ nonlocal_idx ][ pxl ];
	for( size_t adj_idx = 0; adj_idx < adj_size; ++adj_idx ) {
	  if( adj_itr.AdjIdx( nonlocal_pxl, adj_idx, adj_pxl ) ) {
	    D grad = img[ adj_pxl ] - img[ pxl ];
	    flow += weight[ adj_size + adj_idx ] * grad * diff_func( kappa, grad );
	  }
	}
      }
      COMMENT( "Updating pixel intensity with adjacent flow.", 4 );
      res[ pxl ] = img[ pxl ] + integration_constant * flow;
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
Image< D > QuickNonLocalAnisotropicDiffusion( const Image< D > &img, const RobustDiffusionFunction &diff_func,
					      float kappa, const Adjacency &adj, const AdjacencyIterator &adj_itr,
					      const Vector< double > &weight, double integration_constant,
					      const Vector< Image< D > > &map ) {
  try {
    Image< D > res( img );
    COMMENT( "Computing diffusion filter.", 2 );
    // try {
    //   size_t total_threads = 12;
    //   Vector< std::thread > threads;
    //   for( size_t thd = 0; thd < total_threads; ++thd ) {
    // 	threads.push_back( std::thread( &QuickNonLocalAnisotropicDiffusionThread< D >, std::ref( img ), std::ref( res ),
    // 					std::ref( diff_func ), kappa, std::ref( adj ), std::ref( adj_itr ),
    // 					std::ref( weight ), integration_constant, std::ref( map ), thd,
    // 					total_threads ) );
    //   }
    //   for( size_t thd = 0; thd < total_threads; ++thd ) {
    // 	threads( thd ).join( );
    //   }
    // }
    // catch( std::exception &e ) {
    //   BIAL_WARNING( "Failed to run in multi-thread. Exception: " << e.what( ) );
    QuickNonLocalAnisotropicDiffusionThread( img, res, diff_func, kappa, adj, adj_itr, weight, integration_constant,
					     map, 0, 1 );
    //}
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
Image< D > AdaptiveNonLocalAnisotropicDiffusion( Image< D > img, const RobustDiffusionFunction &diff_func,
                         float init_kappa, const Adjacency &adj, float reduction_factor,
						 const AdjacencyIterator &adj_itr, const Vector< double > &weight,
						 double integration_constant, const Vector< Image< D > > &map ) {
  try {
    COMMENT( "Avoid flitering flat image.", 1 );
    if( init_kappa < 10.0 )
      return( Image< D >( img ) );
    COMMENT( "Initial filtering with kappa: " << init_kappa, 1 );
    Image< D > res = QuickNonLocalAnisotropicDiffusion( img, diff_func, init_kappa, adj, adj_itr, weight, integration_constant, map );
    COMMENT( "Reducing kappa.", 1 );
    float kappa = init_kappa - reduction_factor * init_kappa;
    COMMENT( "Filtering until low value of kappa is reached.", 1 );
    while( kappa >= 10.0 ) {
      COMMENT( "New kappa: " << kappa, 2 );
      std::swap( img, res );
      res = QuickNonLocalAnisotropicDiffusion( img, diff_func, kappa, adj, adj_itr, weight, integration_constant, map );
      kappa -= reduction_factor * kappa;
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
Image< D > NonLocalOptimumADFilter( Image< D > img, const RobustDiffusionFunction &diff_func, const Image< D > &backg,
		      const float search_radius, const float patch_radius, const float patch_distance,
		      const size_t patch_adjacents, const float reduction, const float conservativeness ) {
  try {
    COMMENT( "Computing the map of adjacent patches.", 0 );
    Vector< Image< D > > map; // Resulting map with a image for each patch adjacent.
    for( size_t patch = 0; patch < patch_adjacents; ++patch )
      map.push_back( Image< D >( img.Dim( ) ) );
    if( patch_adjacents > 0 ) {
      size_t frame_size = std::ceil( search_radius + patch_radius );
      Image< D > framed_img( Filtering::Median( ImageOp::AddFrame( img, frame_size ), 1.8f ) );
      Vector< size_t > img_dim( img.Dim( ) );
      // try {
      // 	size_t total_threads = 12;
      // 	Vector< std::thread > threads;
      // 	for( size_t thd = 0; thd < total_threads; ++thd ) {
      // 	  threads.push_back( std::thread( &PatchSimilarityMap< D >, std::ref( map ), std::ref( framed_img ),
      // 					  std::ref( img_dim ), search_radius, patch_radius, patch_adjacents,
      // 					  thd, total_threads ) );
      // 	}
      // 	for( size_t thd = 0; thd < total_threads; ++thd ) {
      // 	  threads( thd ).join( );
      // 	}
      // }
      // catch( std::exception &e ) {
      // 	BIAL_WARNING( "Failed to run in multi-thread. Exception: " << e.what( ) );
      PatchSimilarityMap( map, framed_img, img.Dim( ), search_radius, patch_radius, patch_adjacents, 0, 1 );
      //}
    }
    COMMENT( "Creating initial data structures.", 0 );
    Adjacency adj( AdjacencyType::HyperSpheric( patch_radius, img.Dims( ) ) );
    AdjacencyIterator adj_itr( img, adj );
    size_t adj_size = adj.size( );
    size_t img_dims = img.Dims( );
    COMMENT( "Creating weight applied to each adjacent pixel based to its relative position to the filtered pixel.", 2 );
    Vector< double > weight( NonLocalWeightVector( adj, adj_size, img_dims, patch_distance ) );
    COMMENT( "Computing integration constant.", 2 );
    double integration_constant = NonLocalIntegrationConstant( adj, adj_size, img_dims, patch_adjacents, patch_distance );
    COMMENT( "Computing initial kappa using edge and flat regions.", 0 );
    float flat_kappa = NonLocalFlatRegionKappa( img, backg, adj, adj_itr );
    COMMENT( "Returning adaptive filter based on kappa value.", 0 );
    float init_kappa = flat_kappa * conservativeness;
    return( AdaptiveNonLocalAnisotropicDiffusion( img, diff_func, init_kappa, adj, reduction, adj_itr, weight, integration_constant, map ) );
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

  template bool NoisyPixels( const Image< int > &img, Image< int > &noise_mask );

  template void PatchSimilarityMap( Vector< Image< int > > &map, const Image< int > &framed_img, const Vector< size_t > &img_dim,
                 const float search_radius, const float patch_radius, const size_t patch_adjacents,
                 const size_t thread, const size_t total_threads );

  template float NonLocalFlatRegionKappa( const Image< int > &source, const Image< int > &mask, const Adjacency &adj,
                   const AdjacencyIterator &adj_itr );

  template void QuickNonLocalAnisotropicDiffusionThread( const Image< int > &img, Image< int > &res,
                          const RobustDiffusionFunction &diff_func, float kappa,
                          const Adjacency &adj, const AdjacencyIterator &adj_itr,
                          const Vector< double > &weight, double integration_constant,
                          const Vector< Image< int > > &map, size_t thread, size_t total_threads );

  template Image< int > QuickNonLocalAnisotropicDiffusion( const Image< int > &img, const RobustDiffusionFunction &diff_func,
                          float kappa, const Adjacency &adj, const AdjacencyIterator &adj_itr,
                          const Vector< double > &weight, double integration_constant,
                          const Vector< Image< int > > &map );

  template Image< int > AdaptiveNonLocalAnisotropicDiffusion( Image< int > img, const RobustDiffusionFunction &diff_func,
                             float init_kappa, const Adjacency &adj, float reduction_factor,
                             const AdjacencyIterator &adj_itr, const Vector< double > &weight,
                             double integration_constant, const Vector< Image< int > > &map );

  template Image< int > NonLocalOptimumADFilter( Image< int > img, const RobustDiffusionFunction &diff_func, const Image< int > &backg,
              const float search_radius, const float patch_radius, const float patch_distance,
              const size_t patch_adjacents, const float reduction, const float conservativeness );


  template bool NoisyPixels( const Image< float > &img, Image< float > &noise_mask );

  template void PatchSimilarityMap( Vector< Image< float > > &map, const Image< float > &framed_img, const Vector< size_t > &img_dim,
                 const float search_radius, const float patch_radius, const size_t patch_adjacents,
                 const size_t thread, const size_t total_threads );

  template float NonLocalFlatRegionKappa( const Image< float > &source, const Image< float > &mask, const Adjacency &adj,
                   const AdjacencyIterator &adj_itr );

  template void QuickNonLocalAnisotropicDiffusionThread( const Image< float > &img, Image< float > &res,
                          const RobustDiffusionFunction &diff_func, float kappa,
                          const Adjacency &adj, const AdjacencyIterator &adj_itr,
                          const Vector< double > &weight, double integration_constant,
                          const Vector< Image< float > > &map, size_t thread, size_t total_threads );

  template Image< float > QuickNonLocalAnisotropicDiffusion( const Image< float > &img, const RobustDiffusionFunction &diff_func,
                          float kappa, const Adjacency &adj, const AdjacencyIterator &adj_itr,
                          const Vector< double > &weight, double integration_constant,
                          const Vector< Image< float > > &map );

  template Image< float > AdaptiveNonLocalAnisotropicDiffusion( Image< float > img, const RobustDiffusionFunction &diff_func,
                             float init_kappa, const Adjacency &adj, float reduction_factor,
                             const AdjacencyIterator &adj_itr, const Vector< double > &weight,
                             double integration_constant, const Vector< Image< float > > &map );

  template Image< float > NonLocalOptimumADFilter( Image< float > img, const RobustDiffusionFunction &diff_func, const Image< float > &backg,
              const float search_radius, const float patch_radius, const float patch_distance,
              const size_t patch_adjacents, const float reduction, const float conservativeness );

}

#endif
