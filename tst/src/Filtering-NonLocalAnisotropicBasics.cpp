/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2012/Sep/21 */
/* Content: Test file. */
/* Description: Test with filtering. */

#include "AdjacencyIterator.hpp"
#include "AdjacencyRound.hpp"
#include "Common.hpp"
#include "DiffusionFunction.hpp"
#include "FileImage.hpp"
#include "FilteringOptimalAnisotropicDiffusion.hpp"
#include "FilteringAnisotropicDiffusion.hpp"
#include "FilteringMedian.cpp"
#include "GradientCanny.hpp"
#include "Image.hpp"
#include "ImageFrame.hpp"
#include "SegmentationBackground.hpp"
#include "SignalMedianFilter.hpp"
#include "StatisticsStdDev.hpp"

using namespace std;
using namespace Bial;

float reduction_factor = 1.0f;

template< class D >
bool FlatEdgePixels( const Image< D > &img, Image< D > &edges_mask, Image< D > &noise_mask ) {
  try {
    Adjacency adj( AdjacencyType::HyperSpheric( 1.8f, img.Dims( ) ) );
    AdjacencyIterator adj_itr( img, adj );
    size_t img_size = img.size( );
    size_t adj_size = adj.size( );
    size_t adj_pxl;
    Image< int > median( Filtering::Median( img, 1.8f ) );
    size_t edges_pixels = 0;
    size_t noise_pixels = 0;
    Vector< D > noise_hist( img.Maximum( ) + 1 );
    Vector< D > edges_hist( img.Maximum( ) + 1 );
    for( size_t src_pxl = 0; src_pxl < img_size; ++src_pxl ) {
      int img_lower = 0;
      int img_higher = 0;
      int med_lower = 0;
      int med_higher = 0;
      D diff = 0;
      for( size_t adj_idx = 1; adj_idx < adj_size; ++adj_idx ) {
	if( adj_itr.AdjIdx( src_pxl, adj_idx, adj_pxl ) ) {
	  if( img[ src_pxl ] < img[ adj_pxl ] )
	    ++img_lower;
	  if( img[ src_pxl ] > img[ adj_pxl ] )
	    ++img_higher;
	  if( median[ src_pxl ] < median[ adj_pxl ] )
	    ++med_lower;
	  if( median[ src_pxl ] > median[ adj_pxl ] )
	    ++med_higher;
	  diff += std::abs( img[ src_pxl ] - img[ adj_pxl ] );
	}
      }
      COMMENT( "Checking if src_pxl is edge and noise pixel.", 3 );
      if( static_cast< size_t >( med_lower + med_higher ) == adj_size - 1 ) {
	COMMENT( "All adjacents are different.", 3 );
	if( static_cast< size_t >( std::abs( med_lower - med_higher ) ) < 2 ) {
	  edges_mask[ src_pxl ] = 1;
	  ++edges_pixels;
	  ++edges_hist[ diff / ( adj_size - 1 ) ];
	}
	if( ( img_lower == 0 ) || ( img_higher == 0 ) ) {
	  noise_mask[ src_pxl ] = 1;
	  ++noise_pixels;
	  ++noise_hist[ diff / ( adj_size - 1 ) ];
	}
      }
    }
    COMMENT( "Computing 5% of higher difference of intensity noise pixels.", 0 );
    size_t max_noise_pixels = std::min( static_cast< int >( noise_pixels ), std::max( static_cast< int >( 1000 ), static_cast< int >( noise_pixels / 4 ) ) );
    size_t count_noise_pixels = 0;
    size_t noise_pixel_index = 0;
    while( noise_pixel_index < noise_hist.size( ) ) {
      count_noise_pixels += noise_hist[ noise_pixel_index ];
      ++noise_pixel_index;
      if( count_noise_pixels > max_noise_pixels )
	break;
    }
    COMMENT( "Computing 25% of higher difference of intensity edges pixels.", 0 );
    size_t max_edges_pixels = std::min( static_cast< int >( edges_pixels ), std::max( static_cast< int >( 1000 ), static_cast< int >( edges_pixels / 4 ) ) );
    size_t count_edges_pixels = 0;
    size_t edges_pixel_index = 0;
    while( edges_pixel_index < edges_hist.size( ) ) {
      count_edges_pixels += edges_hist[ edges_pixel_index ];
      ++edges_pixel_index;
      if( count_edges_pixels > max_edges_pixels )
	break;
    }
    size_t edges_removed = 0;
    size_t noise_removed = 0;
    for( size_t src_pxl = 0; src_pxl < img_size; ++src_pxl ) {
      COMMENT( "Removing edge pixels which are isolated and with low contrast.", 3 );
      if( edges_mask[ src_pxl ] == 1 ) {
	size_t adjs = 0;
	D diff = 0;
	for( size_t adj_idx = 1; adj_idx < adj_size; ++adj_idx ) {
	  adj_pxl = adj_itr( src_pxl, adj_idx );
	  diff += std::abs( median[ src_pxl ] - median[ adj_pxl ] );
	  if( edges_mask[ adj_pxl ] == 1 )
	    ++adjs;
	}
	if( adjs < 4 ) {
	  edges_mask[ src_pxl ] = 0;
	  ++edges_removed;
	  --edges_pixels;
	}
	if( diff / ( adj_size - 1 ) < edges_pixel_index ) {
	  edges_mask[ src_pxl ] = 0;
	  ++edges_removed;
	  --edges_pixels;
	}
      }
      COMMENT( "Removing low contrast noise pixels.", 3 );
      if( noise_mask[ src_pxl ] == 1 ) {
	D diff = 0;
	for( size_t adj_idx = 1; adj_idx < adj_size; ++adj_idx ) {
	  adj_pxl = adj_itr( src_pxl, adj_idx );
	  diff += std::abs( median[ src_pxl ] - median[ adj_pxl ] );
	}
	if( diff / ( adj_size - 1 ) < noise_pixel_index ) {
	  noise_mask[ src_pxl ] = 0;
	  ++noise_removed;
	  --noise_pixels;
	}
      }
    }
    std::cout << "edges removed: " << edges_removed << ", noise removed: " << noise_removed << std::endl;
    if( ( noise_pixels < 100 ) || ( edges_pixels < 100 ) ) {
      std::cout << "Too few noise or edge pixels." << std::endl;
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
    size_t max_noise_pixels = std::min( static_cast< int >( noise_pixels ), std::max( static_cast< int >( 1000 ), static_cast< int >( noise_pixels * 0.9 ) ) );
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
  size_t frame_size = std::ceil( search_radius + patch_radius );
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

template< class D >
void NonLocalAnisotropicDiffusionThread( Image< D > &img, Image< D > &res, const Vector< Image< D > > &map,
					 const Vector< double > &weight, const double integration_constant,
					 const DiffusionFunction *diff_func, const float kappa, const Adjacency &adj,
					 const size_t thread, const size_t total_threads ) {
  try {
    COMMENT( "Dealing with thread limits.", 3 );
    size_t min_index = thread * img.Size( ) / total_threads;
    size_t max_index = ( thread + 1 ) * img.Size( ) / total_threads;
    AdjacencyIterator adj_itr( img, adj );
    size_t adj_size = adj.size( );
    size_t adj_pxl;
    for( size_t pxl = min_index; pxl < max_index; ++pxl ) {
      double flow = 0.0;
      COMMENT( "Computing intensity flow from adjacents.", 4 );
      for( size_t adj_idx = 1; adj_idx < adj_size; ++adj_idx ) {
	if( adj_itr.AdjIdx( pxl, adj_idx, adj_pxl ) ) {
	  D grad = img[ adj_pxl ] - img[ pxl ];
	  flow += weight[ adj_idx ] * grad * ( *diff_func )( kappa, grad );
	}
      }
      COMMENT( "Computing intensity flow from non-locals.", 4 );
      for( size_t adj_idx = 0; adj_idx < map.size( ); ++adj_idx ) {
	D grad = img[ map[ adj_idx ][ pxl ] ] - img[ pxl ];
	flow += grad * ( *diff_func )( kappa, grad );
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
Image< D > NonLocalAnisotropicDiffusion( Image< D > img, const DiffusionFunction *diff_func, const float kappa,
					 const size_t iterations, const float search_radius, const float patch_radius,
					 const size_t patch_adjacents ) {
  try {
    Vector< Image< D > > map( patch_adjacents, Image< D >( img.Dim( ) ) ); // Resulting map with a image for each patch adjacent.
    size_t frame_size = std::ceil( search_radius + patch_radius );
    Image< D > framed_img( ImageOp::AddFrame( img, frame_size ) );
    Vector< size_t > img_dim( img.Dim( ) );
    try {
      size_t total_threads = 12;
      Vector< std::thread > threads;
      for( size_t thd = 0; thd < total_threads; ++thd ) {
	threads.push_back( std::thread( &PatchSimilarityMap< D >, std::ref( map ), std::ref( framed_img ),
					std::ref( img_dim ), search_radius, patch_radius, patch_adjacents,
					thd, total_threads ) );
      }
      for( size_t thd = 0; thd < total_threads; ++thd ) {
	threads( thd ).join( );
      }
    }
    catch( std::exception &e ) {
      BIAL_WARNING( "Failed to run in multi-thread. Exception: " << e.what( ) );
      PatchSimilarityMap( map, framed_img, img.Dim( ), search_radius, patch_radius, patch_adjacents, 0, 1 );
    }
    return( NonLocalAnisotropicDiffusion( img, diff_func, kappa, iterations, map ) );
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
Image< D > NonLocalAnisotropicDiffusion( Image< D > img, const DiffusionFunction *diff_func, const float kappa,
					 const size_t iterations, float radius, const Vector< Image< D > > &map ) {
  try {
    Image< D > res( img );
    Adjacency adj( AdjacencyType::HyperSpheric( radius, img.Dims( ) ) );
    size_t adj_size = adj.size( );
    COMMENT( "Creating weight applied to each adjacent pixel based to its relative position to the filtered pixel.", 2 );
    Vector< double > weight( adj_size, 0 );
    size_t img_dims = img.Dims( );
    for( size_t adj_idx = 1; adj_idx < adj_size; ++adj_idx ) {
      COMMENT( "Computing L2 distance.", 4 );
      double distance = 0.0;
      for( size_t dim = 0; dim < img_dims; ++dim )
	distance += adj( adj_idx, dim ) * adj( adj_idx, dim );
      weight[ adj_idx ] = 1.0 / std::sqrt( distance );
    }
    COMMENT( "Computing integration constant.", 2 );
    double integration_constant = 0.0;
    for( size_t idx = 1; idx < adj_size; ++idx ) {
      double distance = 0.0;
      for( size_t dim = 0; dim < img.Dims( ); ++dim )
	distance += std::abs( adj( idx, dim ) );
      integration_constant += 1.0 / distance;
    }
    integration_constant = 1.0 / integration_constant;
    for( size_t itr = 0; itr < iterations; ++itr ) {
      COMMENT( "Computing diffusion filter.", 2 );
      try {
	size_t total_threads = 12;
	Vector< std::thread > threads;
	for( size_t thd = 0; thd < total_threads; ++thd ) {
	  threads.push_back( std::thread( &NonLocalAnisotropicDiffusionThread< D >, std::ref( img ), std::ref( res ),
					  std::ref( map ), weight, integration_constant, diff_func, kappa,
					  std::ref( adj ), thd, total_threads ) );
	}
	for( size_t thd = 0; thd < total_threads; ++thd ) {
	  threads( thd ).join( );
	}
      }
      catch( std::exception &e ) {
	BIAL_WARNING( "Failed to run in multi-thread. Exception: " << e.what( ) );
	NonLocalAnisotropicDiffusionThread( img, res, map, weight, integration_constant, diff_func, kappa, adj, 0, 1 );
      }
      COMMENT( "Updating image.", 2 );
      std::swap( img, res );
    }
    return( img );
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
void QuickNonLocalAnisotropicDiffusionThread( const Image< D > &img, Image< D > &res,
					      const DiffusionFunction *diff_func, float kappa,
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
	  flow += weight[ adj_idx ] * grad * ( *diff_func )( kappa, grad );
	}
      }
      for( size_t nonlocal_idx = 0; nonlocal_idx < map.size( ); ++nonlocal_idx ) {
	size_t nonlocal_pxl = map[ nonlocal_idx ][ pxl ];
	for( size_t adj_idx = 0; adj_idx < adj_size; ++adj_idx ) {
	  if( adj_itr.AdjIdx( nonlocal_pxl, adj_idx, adj_pxl ) ) {
	    D grad = img[ adj_pxl ] - img[ pxl ];
	    flow += weight[ adj_size + adj_idx ] * grad * ( *diff_func )( kappa, grad );
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
Image< D > QuickNonLocalAnisotropicDiffusion( const Image< D > &img, const DiffusionFunction *diff_func, float kappa,
					      const Adjacency &adj, const AdjacencyIterator &adj_itr,
					      const Vector< double > &weight, double integration_constant,
					      const Vector< Image< D > > &map ) {
  try {
    Image< D > res( img );
    COMMENT( "Computing diffusion filter.", 2 );
    try {
      size_t total_threads = 12;
      Vector< std::thread > threads;
      for( size_t thd = 0; thd < total_threads; ++thd ) {
	threads.push_back( std::thread( &QuickNonLocalAnisotropicDiffusionThread< D >, std::ref( img ), std::ref( res ),
					std::ref( diff_func ), kappa, std::ref( adj ), std::ref( adj_itr ),
					std::ref( weight ), integration_constant, std::ref( map ), thd,
					total_threads ) );
      }
      for( size_t thd = 0; thd < total_threads; ++thd ) {
	threads( thd ).join( );
      }
    }
    catch( std::exception &e ) {
      BIAL_WARNING( "Failed to run in multi-thread. Exception: " << e.what( ) );
      QuickNonLocalAnisotropicDiffusionThread( img, res, diff_func, kappa, adj, adj_itr, weight, integration_constant,
						     map, 0, 1 );
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
Image< D > QuickNonLocalAnisotropicDiffusion( const Image< D > &img, const Vector< size_t > &mask,
					      const DiffusionFunction *diff_func, float kappa,
					      const Adjacency &adj, const AdjacencyIterator &adj_itr,
					      const Vector< double > &weight, double integration_constant,
					      const Vector< Image< D > > &map ) {
  try {
    Image< D > res( img );
    size_t adj_size = adj.size( );
    size_t mask_size = mask.size( );
    COMMENT( "Computing diffusion filter.", 2 );
    size_t adj_pxl;
    for( size_t src_pxl = 0; src_pxl < mask_size; ++src_pxl ) {
      size_t pxl = mask[ src_pxl ];
      COMMENT( "Computing intensity flow from adjacents.", 4 );
      double flow = 0.0;
      for( size_t adj_idx = 1; adj_idx < adj_size; ++adj_idx ) {
	if( adj_itr.AdjIdx( pxl, adj_idx, adj_pxl ) ) {
	  D grad = img[ adj_pxl ] - img[ pxl ];
	  flow += weight[ adj_idx ] * grad * ( *diff_func )( kappa, grad );
	}
      }
      for( size_t nonlocal_idx = 0; nonlocal_idx < map.size( ); ++nonlocal_idx ) {
	size_t nonlocal_pxl = map[ nonlocal_idx ][ pxl ];
	for( size_t adj_idx = 0; adj_idx < adj_size; ++adj_idx ) {
	  if( adj_itr.AdjIdx( nonlocal_pxl, adj_idx, adj_pxl ) ) {
	    D grad = img[ adj_pxl ] - img[ pxl ];
	    flow += weight[ adj_size + adj_idx ] * grad * ( *diff_func )( kappa, grad );
	  }
	}
      }
      COMMENT( "Updating pixel intensity with adjacent flow.", 4 );
      res[ pxl ] = img[ pxl ] + integration_constant * flow;
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
float NonLocalEdgeRegionKappa( const Image< D > &source, const Image< D > &mask, 
			       const DiffusionFunction *diff_func, const Vector< double > &weight,
			       const double integration_constant, const Adjacency &adj,
			       const AdjacencyIterator &adj_itr, float &step, const Vector< Image< D > > &map ) {
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
    float kappa = 10.0f;
    step = 10;
    Vector< float > kappas;
    Vector< float > stds;
    size_t min_std_idx;
    float min_std = 99999999.9f;
    float max_std = 0.0f;
    size_t idx = 0;

    ullint diff_sum = 0;
    for( size_t pxl_idx = 0; pxl_idx < mask_vector_size; ++pxl_idx ) {
      size_t src_pxl = mask_vector[ pxl_idx ];
      for( size_t adj_idx = 1; adj_idx < adj.size( ); ++adj_idx ) {
	size_t adj_pxl = adj_itr( src_pxl, adj_idx );
	diff_sum += std::abs( source[ src_pxl ] - source[ adj_pxl ] );
      }
    }
    double mean_diff = ( diff_sum /= adj.size( ) ) / static_cast< double >( mask_vector_size );
    std::cout << "Initial edge region mean difference: " << mean_diff << std::endl;
    return( mean_diff );

    
    Image< D > filtered( source.Dim( ), source.PixelSize( ) );
    for( float kappa = 10.0f; kappa < 1500.0f; kappa +=step ) {
      filtered = QuickNonLocalAnisotropicDiffusion( source, mask_vector, diff_func, kappa, adj, adj_itr,
						    weight, integration_constant, map );
      for( size_t pxl = 0; pxl < mask_vector_size; ++pxl )
	edges[ pxl ] = filtered[ mask_vector[ pxl ] ];
      float std = Statistics::StandardDeviation( edges );
      kappas.push_back( kappa );
      stds.push_back( std );
      if( min_std > std ) {
	min_std = std;
	min_std_idx = idx;
      }
      ++idx;
    }
    SignalOp::MedianFilter( stds, 5 );
    //std::cout << "standard deviations:" << std::endl;
    //for( size_t i = 0; i < stds.size( ); ++i )
    //std::cout << kappas[ i ] << ": " << stds[ i ] << std::endl;
    //std::cout << std::endl;
    COMMENT( "Computing maximum index before the global minimum.", 1 );
    for( idx = 0; idx < min_std_idx; ++idx ) {
      if( max_std < stds[ idx ] )
	max_std = stds[ idx ];
    }
    COMMENT( "Computing the best std.", 1 );
    float edge_std = max_std - ( max_std - min_std ) * 0.1f; // Edge Rule: Imax before min; Imax-(Imax-Imin)*0.1
    for( idx = 0; idx < min_std_idx; ++idx ) {
      if( stds[ idx ] <= edge_std ) {
	kappa = kappas[ idx ];
	break;
      }
    }
    std::cout << "min_std: " << min_std << ", min_kappa: " << kappas[ min_std_idx ] << ", max_std:" << max_std << ", edge_std: " << edge_std << ", edge_kappa: " << kappa << std::endl;
    return( kappa );
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

// template< class D >
// float NonLocalEdgeRegionKappa( const Image< D > &source, const Image< D > &mask, 
// 			       const DiffusionFunction *diff_func, const Vector< double > &weight,
// 			       const double integration_constant, const Adjacency &adj,
// 			       const AdjacencyIterator &adj_itr, float &step, const Vector< Image< D > > &map ) {
//   try {
//     COMMENT( "Get initial kappa, minimum standard deviation of flat region, and maximum standard deviation of edge "
// 	     << "region.", 3 );
//     COMMENT( "Computing mask vector.", 3 );
//     Vector< size_t > mask_vector;
//     for( size_t pxl = 0; pxl < mask.size( ); ++pxl ) {
//       if( mask[ pxl ] != 0 )
// 	mask_vector.push_back( pxl );
//     }
//     size_t mask_vector_size = mask_vector.size( );
//     COMMENT( "edges pixel vector.", 3 );
//     Vector< int > edges( mask_vector_size );
//     for( size_t pxl = 0; pxl < mask_vector_size; ++pxl ) {
//       edges[ pxl ] = source[ mask_vector[ pxl ] ];
//       //std::cout << edges[ pxl ] << " ";
//     }
//     //std::cout << std::endl;
//     float base_std = Statistics::StandardDeviation( edges );
//     COMMENT( "Estimate best edge kappa based on the maximum standard deviation in a binary search.", 3 );
//     float kappa = base_std / 50;
//     step = kappa;
//     //std::cout << "QuickNonLocalAnisotropicDiffusion: " << "kappa: " << kappa << ", weight: " << weight <<
//     //", integration_constant: " << integration_constant << std::endl;
//     Image< D > filtered( QuickNonLocalAnisotropicDiffusion( source, mask_vector, diff_func, kappa, adj,
// 							    adj_itr, weight, integration_constant, map ) );
//     for( size_t pxl = 0; pxl < mask_vector_size; ++pxl ) {
//       edges[ pxl ] = filtered[ mask_vector[ pxl ] ];
//       //std::cout << edges[ pxl ] << " ";
//     }
//     //std::cout << std::endl;
//     float prev_std = Statistics::StandardDeviation( edges );
//     Vector< float > kappas;
//     Vector< float > std_diff;
//     kappas.push_back( kappa );
//     std_diff.push_back( 0.0f );
//     float max_std_diff = 0.0f;
//     do {
//       filtered = QuickNonLocalAnisotropicDiffusion( source, mask_vector, diff_func, kappa + step, adj, adj_itr,
// 						    weight, integration_constant, map );
//       for( size_t pxl = 0; pxl < mask_vector_size; ++pxl )
// 	edges[ pxl ] = filtered[ mask_vector[ pxl ] ];
//       float std = Statistics::StandardDeviation( edges );
//       kappas.push_back( kappa + step );
//       std_diff.push_back( prev_std - std );
//       //std::cout << "prev_std: " << prev_std << ", std: " << std << std::endl;
//       if( max_std_diff < prev_std - std )
// 	max_std_diff = prev_std - std;
//       prev_std = std;
//       kappa += step;
//     } while( ( kappa < base_std ) || ( std_diff[ std_diff.size( ) - 2 ] > std_diff[ std_diff.size( ) - 1 ] ) );
//     COMMENT( "max_std_diff: " << max_std_diff << "\nstd_diff: " << std_diff << "\nkappas: " << kappas, 3 );
//     //std::cout << "max_std_diff: " << max_std_diff << "\nstd_diff: " << std_diff << "\nkappas: " << kappas << std::endl;
//     for( size_t idx = 1; idx < std_diff.size( ); ++idx ) {
//       if( std_diff[ idx ] > 0.01 * max_std_diff ) {
// 	COMMENT( "edge kappa: " << kappas[ idx ], 3 );
// 	return( kappas[ idx ] );
//       }
//     }
//     return( kappa );
//   }
//   catch( std::bad_alloc &e ) {
//     std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
//     throw( std::runtime_error( msg ) );
//   }
//   catch( std::runtime_error &e ) {
//     std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
//     throw( std::runtime_error( msg ) );
//   }
//   catch( const std::out_of_range &e ) {
//     std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
//     throw( std::out_of_range( msg ) );
//   }
//   catch( const std::logic_error &e ) {
//     std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
//     throw( std::logic_error( msg ) );
//   }
// }


template< class D >
float NonLocalFlatRegionKappa( const Image< D > &source, const Image< D > &mask, const DiffusionFunction *diff_func,
			       const Vector< double > &weight, const double integration_constant, const Adjacency &adj,
			       const AdjacencyIterator &adj_itr, float step, const Vector< Image< D > > &map ) {
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
    float kappa = 10.0f;
    step = 10;
    Vector< float > kappas;
    Vector< float > stds;
    size_t min_std_idx;
    size_t max_std_idx = 0;
    float min_std = 99999999.9f;
    float max_std = 0.0f;
    size_t idx = 0;
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
    return( mean_diff );

    
    for( float kappa = 10.0f; kappa < 1500.0f; kappa +=step ) {
      filtered = QuickNonLocalAnisotropicDiffusion( source, mask_vector, diff_func, kappa, adj, adj_itr,
						    weight, integration_constant, map );
      for( size_t pxl = 0; pxl < mask_vector_size; ++pxl )
	edges[ pxl ] = filtered[ mask_vector[ pxl ] ];
      float std = Statistics::StandardDeviation( edges );
      kappas.push_back( kappa );
      stds.push_back( std );
      if( min_std > std ) {
	min_std = std;
	min_std_idx = idx;
      }
      ++idx;
    }
    SignalOp::MedianFilter( stds, 5 );
    //std::cout << "standard deviations:" << std::endl;
    //for( size_t i = 0; i < stds.size( ); ++i )
    //std::cout << kappas[ i ] << ": " << stds[ i ] << std::endl;
    //std::cout << std::endl;
    COMMENT( "Computing maximum index before the global minimum.", 1 );
    for( idx = 0; idx < min_std_idx; ++idx ) {
      if( max_std < stds[ idx ] ) {
	max_std = stds[ idx ];
	max_std_idx = idx;
      }
    }
    COMMENT( "Computing the best std.", 1 );
    float flat_std = std::max( ( max_std - min_std ) * 0.5f + min_std, min_std * 1.05f ); // noise rule: max((Dmax-Dmin)*0.15+Dmin,Dmin*1.01)
    if( flat_std > max_std )
      flat_std = std::min( ( max_std - min_std ) * 0.5f + min_std, min_std * 1.05f ); // noise rule: min((Dmax-Dmin)*0.15+Dmin,Dmin*1.01)
    if( max_std_idx < min_std_idx )
      idx = max_std_idx;
    else
      idx = 0;
    for( ; idx < min_std_idx; ++idx ) {
      if( stds[ idx ] <= flat_std ) {
	kappa = kappas[ idx ];
	break;
      }
    }
    std::cout << "min_std: " << min_std << ", min_kappa: " << kappas[ min_std_idx ] << ", max_std:" << max_std << ", flat_std: " << flat_std << ", flat_kappa: " << kappa << std::endl;
    return( kappa );
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

// template< class D >
// float NonLocalFlatRegionKappa( const Image< D > &source, const Image< D > &mask, const DiffusionFunction *diff_func,
// 			       const Vector< double > &weight, const double integration_constant, const Adjacency &adj,
// 			       const AdjacencyIterator &adj_itr, float step, const Vector< Image< D > > &map ) {
//   try {
//     COMMENT( "Computing mask vector.", 3 );
//     Vector< size_t > mask_vector;
//     for( size_t pxl = 0; pxl < mask.size( ); ++pxl ) {
//       if( mask[ pxl ] != 0 )
// 	mask_vector.push_back( pxl );
//     }
//     size_t mask_vector_size = mask_vector.size( );
//     COMMENT( "Get initial kappa, minimum standard deviation of flat region, and maximum standard deviation of edge region.", 0 );
//     float kappa = step;
//     Vector< int > backg( mask_vector_size );
//     COMMENT( "Getting standard deviation given by strong mean filter of input image.", 0 );
//     Image< D > filtered( QuickNonLocalAnisotropicDiffusion( source, mask_vector, diff_func, kappa, adj, adj_itr,
// 							    weight, integration_constant, map ) );
//     for( size_t pxl = 0; pxl < mask_vector_size; ++pxl )
//       backg[ pxl ] = filtered[ mask_vector[ pxl ] ];
//     float prev_std = Statistics::StandardDeviation( backg );
//     COMMENT( "Estimate best kappa for flat region.", 0 );
//     float max_diff = 0.0f;
//     Vector< float > diff( 4, 0.0 );
//     bool ten_pc = false;
//     float ten_kappa;
//     bool five_pc = false;
//     float five_kappa;
//     do {
//       filtered = QuickNonLocalAnisotropicDiffusion( source, mask_vector, diff_func, kappa + step, adj, adj_itr,
// 						    weight, integration_constant, map );
//       for( size_t pxl = 0; pxl < mask_vector_size; ++pxl )
// 	backg[ pxl ] = filtered[ mask_vector[ pxl ] ];
//       float std = Statistics::StandardDeviation( backg );
//       COMMENT( "Computing average difference in standard deviation.", 0 );
//       diff[ 0 ] = diff[ 1 ];
//       diff[ 1 ] = diff[ 2 ];
//       diff[ 2 ] = prev_std - std;
//       diff[ 3 ] = diff[ 0 ] + diff[ 1 ] + diff[ 2 ];
//       if( max_diff < diff[ 3 ] )
// 	max_diff = diff[ 3 ];
//       COMMENT( "Checking for small increase before decreasing standard deviation. In this case, do not consider previous decrease values.", 0 );
//       if( diff[ 3 ] < 0.0f )
// 	max_diff = 0.0f;
//       COMMENT( "prev_std: " << prev_std << ", std: " << std << ", max_diff: " << max_diff
// 	       << ", mean diff: " << diff[ 3 ] << ", diff: " << diff[ 2 ] << ", kappa: " << kappa << ", step: " << step, 3 );
//       //std::cout << "prev_std: " << prev_std << ", std: " << std << ", max_diff: " << max_diff << ", mean diff: " << diff[ 3 ] << ", diff: " << diff[ 2 ] << ", kappa: " << kappa << ", step: " << step << std::endl;
//       kappa += step;
//       prev_std = std;
//       if( ( ten_pc == false ) && ( diff[ 3 ] <= 0.1 * max_diff ) && ( diff[ 3 ] > 0.0f ) ) {
// 	//std::cout << "10 per cent flat std: " << prev_std << ", flat kappa: " << kappa << std::endl;
// 	ten_pc = true;
// 	ten_kappa = kappa;
//       }
//       if( ( five_pc == false ) && ( diff[ 3 ] < 0.05 * max_diff ) && ( diff[ 3 ] > 0.0f ) ) {
// 	//std::cout << "5 per cent flat std: " << prev_std << ", flat kappa: " << kappa << std::endl;
// 	five_pc = true;
// 	five_kappa = kappa;
//       }
//       if( ( ten_pc ) && ( !five_pc ) && ( 2.0f * ten_kappa < kappa ) ) {
// 	kappa = 2.0f * ten_kappa;
// 	break;
//       }
//       if( ( five_pc ) && ( 2.0f * five_kappa < kappa ) ) {
// 	kappa = 1.5f * five_kappa;
// 	break;
//       }
//     } while( ( ( diff[ 3 ] > 0.01 * max_diff ) || ( diff[ 3 ] < 0.0f ) ) && ( kappa < 1500.0f ) );
//     COMMENT( "flat std: " << prev_std << ", flat kappa: " << kappa, 0 );
//     return( kappa );
//   }
//   catch( std::bad_alloc &e ) {
//     std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
//     throw( std::runtime_error( msg ) );
//   }
//   catch( std::runtime_error &e ) {
//     std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
//     throw( std::runtime_error( msg ) );
//   }
//   catch( const std::out_of_range &e ) {
//     std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
//     throw( std::out_of_range( msg ) );
//   }
//   catch( const std::logic_error &e ) {
//     std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
//     throw( std::logic_error( msg ) );
//   }
// }

template< class D >
Image< D > AdaptiveNonLocalAnisotropicDiffusion( Image< D > img, const DiffusionFunction *diff_func,
						 float init_kappa, const Adjacency &adj,
						 const AdjacencyIterator &adj_itr, const Vector< double > &weight,
						 double integration_constant, const Vector< Image< D > > &map ) {
  try {
    COMMENT( "Avoid flitering flat image.", 1 );
    if( init_kappa < 10.0 )
      return( Image< D >( img ) );
    COMMENT( "Initial filtering with kappa: " << init_kappa, 1 );
    Image< D > res = QuickNonLocalAnisotropicDiffusion( img, diff_func, init_kappa, adj, adj_itr, weight,
							integration_constant, map );
    COMMENT( "Reducing kappa.", 1 );
    float kappa = init_kappa - reduction_factor * init_kappa; // diff_func->Reduction( kappa );
    COMMENT( "Filtering until low value of kappa is reached.", 1 );
    while( kappa >= 10.0 ) {
      COMMENT( "New kappa: " << kappa, 2 );
      std::swap( img, res );
      res = QuickNonLocalAnisotropicDiffusion( img, diff_func, kappa, adj, adj_itr, weight, integration_constant, map );
      kappa -= reduction_factor * kappa; // diff_func->Reduction( kappa );
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
Image< D > OptimalNonLocalAnisotropicDiffusion( Image< D > img, const DiffusionFunction *diff_func,
						float conservativeness, const Image< D > &canny, 
						const Image< D > &backg, const float search_radius,
						const float patch_radius, const float patch_distance,
						const size_t patch_adjacents ) {
  try {
    if( img.Dims( ) != canny.Dims( ) ) {
      std::string msg( BIAL_ERROR( "Input image and canny image dimensions do not match." ) );
      throw( std::logic_error( msg ) );
    }
    if( img.Dims( ) != backg.Dims( ) ) {
      std::string msg( BIAL_ERROR( "Input and background image dimensions do not match." ) );
      throw( std::logic_error( msg ) );
    }
    if( ( conservativeness < 0.0 ) || ( conservativeness > 1.0 ) ) {
      std::string msg( BIAL_ERROR( "Invalid conservativeness paramter. Expected: [0.0, 1.0]. Given: " +
				   std::to_string( conservativeness ) ) );
      throw( std::logic_error( msg ) );
    }
    COMMENT( "Computing the map of adjacent patches.", 0 );
    Vector< Image< D > > map; // Resulting map with a image for each patch adjacent.
    for( size_t patch = 0; patch < patch_adjacents; ++patch )
      map.push_back( Image< D >( img.Dim( ) ) );
    size_t frame_size = std::ceil( search_radius + patch_radius );
    Image< D > framed_img( ImageOp::AddFrame( img, frame_size ) );
    Vector< size_t > img_dim( img.Dim( ) );
    if( patch_adjacents > 0 ) {
      try {
	size_t total_threads = 12;
	Vector< std::thread > threads;
	for( size_t thd = 0; thd < total_threads; ++thd ) {
	  threads.push_back( std::thread( &PatchSimilarityMap< D >, std::ref( map ), std::ref( framed_img ),
					  std::ref( img_dim ), search_radius, patch_radius, patch_adjacents,
					  thd, total_threads ) );
	}
	for( size_t thd = 0; thd < total_threads; ++thd ) {
	  threads( thd ).join( );
	}
      }
      catch( std::exception &e ) {
	BIAL_WARNING( "Failed to run in multi-thread. Exception: " << e.what( ) );
	PatchSimilarityMap( map, framed_img, img.Dim( ), search_radius, patch_radius, patch_adjacents, 0, 1 );
      }
    }
    //std::cout << "Finished computing patches." << std::endl;
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
    float step;
    //float edge_kappa = NonLocalEdgeRegionKappa( img, canny, diff_func, weight, integration_constant, adj, adj_itr, step, map );
    Vector< double > local_weight( Filtering::WeightVector( adj, adj_size, img_dims ) );
    double local_integration_constant = Filtering::IntegrationConstant( adj, adj_size, img_dims );
    float edge_kappa = Filtering::EdgeRegionKappa( img, canny, diff_func, local_weight, local_integration_constant, adj, adj_itr, step );
    //std::cout << "edge_kappa: " << edge_kappa << std::endl;
    //float flat_kappa = NonLocalFlatRegionKappa( img, backg, diff_func, weight, integration_constant, adj, adj_itr, step, map );
    float flat_kappa = Filtering::FlatRegionKappa( img, backg, diff_func, local_weight, local_integration_constant, adj, adj_itr, step );
    //std::cout << "flat_kappa: " << flat_kappa << std::endl;
    float init_kappa = ( edge_kappa + flat_kappa ) / 2.0;
    if( edge_kappa < flat_kappa )
      init_kappa = edge_kappa + conservativeness * ( flat_kappa - edge_kappa );
    COMMENT( "edge_kappa: " << edge_kappa << ", flat_kappa: " << flat_kappa << ", init_kappa: " << init_kappa, 0 );
    std::cout << "edge_kappa: " << edge_kappa << ", flat_kappa: " << flat_kappa
	      << ", init_kappa: " << init_kappa << std::endl;
    COMMENT( "Returning adaptive filter based on initial kappa value.", 0 );
    return( AdaptiveNonLocalAnisotropicDiffusion( img, diff_func, init_kappa, adj, adj_itr, weight,
						  integration_constant, map ) );
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
Image< D > OptimalNonLocalAnisotropicDiffusion( Image< D > img, const DiffusionFunction *diff_func,
						float conservativeness, const float search_radius,
						const float patch_radius, const float patch_distance,
						const size_t patch_adjacents ) {
  try {
    if( ( conservativeness < 0.0 ) || ( conservativeness > 1.0 ) ) {
      std::string msg( BIAL_ERROR( "Invalid conservativeness paramter. Expected: [0.0, 1.0]. Given: " +
				   std::to_string( conservativeness ) ) );
      throw( std::logic_error( msg ) );
    }
    COMMENT( "Computing canny gradient with 0.8, 0.9 hysteresis.", 0 );
    Image< D > canny = Gradient::Canny( img, 0.8, 0.9 );
    COMMENT( "Segmenting the background for a planar region.", 0 );
    Image< D > backg = Segmentation::Background( img, canny );
    COMMENT( "Filtering with optimum anisotropic diffusion filter.", 0 );
    return( OptimalNonLocalAnisotropicDiffusion( img, diff_func, conservativeness, canny, backg,
						 search_radius, patch_radius, patch_distance, patch_adjacents ) );
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
void MultipleOptimalNonLocalAnisotropicDiffusion( Image< D > img, const DiffusionFunction *diff_func,
						  const std::string &inputname,
						  const std::string &basename, const Image< D > &canny, 
						  const Image< D > &backg, const float search_radius,
						  const float patch_radius, const float patch_distance,
						  const size_t patch_adjacents, const float reduction ) {
  try {
    reduction_factor = reduction;
    if( img.Dims( ) != canny.Dims( ) ) {
      std::string msg( BIAL_ERROR( "Input image and canny image dimensions do not match." ) );
      throw( std::logic_error( msg ) );
    }
    if( img.Dims( ) != backg.Dims( ) ) {
      std::string msg( BIAL_ERROR( "Input and background image dimensions do not match." ) );
      throw( std::logic_error( msg ) );
    }
    COMMENT( "Computing the map of adjacent patches.", 0 );
    Vector< Image< D > > map; // Resulting map with a image for each patch adjacent.
    for( size_t patch = 0; patch < patch_adjacents; ++patch )
      map.push_back( Image< D >( img.Dim( ) ) );
    if( patch_adjacents > 0 ) {
      size_t frame_size = std::ceil( search_radius + patch_radius );
      Image< D > framed_img( Filtering::Median( ImageOp::AddFrame( img, frame_size ), 1.8f ) );
      Vector< size_t > img_dim( img.Dim( ) );
      try {
	size_t total_threads = 12;
	Vector< std::thread > threads;
	for( size_t thd = 0; thd < total_threads; ++thd ) {
	  threads.push_back( std::thread( &PatchSimilarityMap< D >, std::ref( map ), std::ref( framed_img ),
					  std::ref( img_dim ), search_radius, patch_radius, patch_adjacents,
					  thd, total_threads ) );
	}
	for( size_t thd = 0; thd < total_threads; ++thd ) {
	  threads( thd ).join( );
	}
      }
      catch( std::exception &e ) {
	BIAL_WARNING( "Failed to run in multi-thread. Exception: " << e.what( ) );
	PatchSimilarityMap( map, framed_img, img.Dim( ), search_radius, patch_radius, patch_adjacents, 0, 1 );
      }
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
    float step = 0;
    //std::cout << "diff_func: " << diff_func << ", weight: " << weight << ", integration_constant: " << integration_constant << std::endl;
    //float edge_kappa = NonLocalEdgeRegionKappa( img, canny, diff_func, weight, integration_constant, adj, adj_itr, step, map );
    float flat_kappa = NonLocalFlatRegionKappa( img, backg, diff_func, weight, integration_constant, adj, adj_itr, step, map );
    //if( edge_kappa < 10.0 )
    //edge_kappa = 10.0001f;
    //COMMENT( "edge_kappa: " << edge_kappa << ", flat_kappa: " << flat_kappa, 0 );
    //std::cout << "edge_kappa: " << edge_kappa << ", flat_kappa: " << flat_kappa << std::endl;
    COMMENT( "Returning adaptive filter based on initial kappa value.", 0 );
    // if( flat_kappa < edge_kappa )
    //   std::swap( flat_kappa, edge_kappa );
    for( size_t cons = 0; cons < 5; ++cons ) {
      float init_kappa = flat_kappa * ( cons + 1.0 ) / 5.0;
      Image< D > res( AdaptiveNonLocalAnisotropicDiffusion( img, diff_func, init_kappa, adj, adj_itr, weight, integration_constant, map ) );
      Write( res, basename + "_" + std::to_string( cons ) + ".nii.gz", inputname );
      // D min = res.Minimum( );
      // if( min != 0 )
      // 	Write( res - min, basename + "_min_" + std::to_string( cons ) + ".nii.gz" );
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
