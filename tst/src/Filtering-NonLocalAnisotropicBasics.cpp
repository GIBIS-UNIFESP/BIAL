/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2012/Sep/21 */
/* Content: Test file. */
/* Description: Test with filtering. */

#include "DiffusionFunction.hpp"
#include "FileImage.hpp"
#include "FilteringAnisotropicDiffusion.hpp"
#include "Image.hpp"

#include "AdjacencyIterator.hpp"
#include "AdjacencyRound.hpp"

using namespace std;
using namespace Bial;

template< class D >
void PatchSimilarityMap( Vector< Image< D > > &map, const Image< D > &img, const float search_radius,
			 const float patch_radius, const size_t patch_adjacents,
			 const size_t thread, const size_t total_threads ) {
  COMMENT( "Dealing with thread limits.", 3 );
  size_t img_size = img.Size( );
  size_t img_dims = img.Dims( );
  size_t min_index = thread * img_size / total_threads;
  size_t max_index = ( thread + 1 ) * img_size / total_threads;
  COMMENT( "Basic variables.", 3 );
  Adjacency search_adj( AdjacencyType::HyperSpheric( search_radius, img_dims ) ); // adjacency with size given by the radius for searching similar patches.
  Adjacency patch_adj( AdjacencyType::HyperSpheric( patch_radius, img_dims ) ); // adjacency with size given by the radius which defines the size of the patch.
  size_t search_adj_size = search_adj.size( );
  size_t patch_adj_size = patch_adj.size( );
  AdjacencyIterator search_adj_itr( img, search_adj );
  AdjacencyIterator patch_adj_itr( img, patch_adj );
  size_t target_pxl;
  size_t target_adj_pxl;
  size_t source_adj_pxl;
  Vector< int > target_patch_dist( search_adj_size, 0 );
  Vector< size_t > target_patch_index( search_adj_size, 0 );
  Vector< size_t > target_patch_common_elements( search_adj_size, 0 );
  for( size_t source_pxl = min_index; source_pxl < max_index; ++source_pxl ) {
    target_patch_dist = 0;
    target_patch_index = 0;
    target_patch_common_elements = 0;
    for( size_t search_adj_idx = 1; search_adj_idx < search_adj_size; ++search_adj_idx ) {
      COMMENT( "For each valid position, compare source and target patches.", 4 );
      if( search_adj_itr.AdjIdx( source_pxl, search_adj_idx, target_pxl ) ) {
	target_patch_index[ 0 ] = target_pxl;
	for( size_t patch_adj_idx = 0; patch_adj_idx < patch_adj_size; ++patch_adj_idx ) {
	  if( patch_adj_itr.AdjIdx( source_pxl, patch_adj_idx, source_adj_pxl ) &&
	      ( patch_adj_itr.AdjIdx( target_pxl, patch_adj_idx, target_adj_pxl ) ) ) {
	    target_patch_dist[ 0 ] += std::abs( img[ source_adj_pxl ] - img[ target_adj_pxl ] );
	    ++target_patch_common_elements[ 0 ];
	  }
	}
      }
      COMMENT( "Sorting target patches by number of common elements (inc) followed by distance (dec).", 4 );
      for( size_t sort_adj_idx = 1; sort_adj_idx < search_adj_idx; ++sort_adj_idx ) {
	if( ( target_patch_common_elements[ 0 ] < target_patch_common_elements[ sort_adj_idx ] ) ||
	    ( ( target_patch_common_elements[ 0 ] == target_patch_common_elements[ sort_adj_idx ] ) &&
	      ( target_patch_dist[ 0 ] > target_patch_dist[ sort_adj_idx ] ) ) ) {
	  std::swap( target_patch_common_elements[ 0 ], target_patch_common_elements[ sort_adj_idx ] );
	  std::swap( target_patch_dist[ 0 ], target_patch_dist[ sort_adj_idx ] );
	  std::swap( target_patch_index[ 0 ], target_patch_index[ sort_adj_idx ] );
	}
      }
      COMMENT( "Assigning last patch data to last position.", 4 );
      std::swap( target_patch_common_elements[ 0 ], target_patch_common_elements[ search_adj_idx ] );
      std::swap( target_patch_dist[ 0 ], target_patch_dist[ search_adj_idx ] );
      std::swap( target_patch_index[ 0 ], target_patch_index[ search_adj_idx ] );
    }
    COMMENT( "Assigning the index of the best patches.", 3 );
    for( size_t adj_index = 0; adj_index < patch_adjacents; ++adj_index )
      map[ adj_index ][ source_pxl ] = target_patch_index[ search_adj_size - 1 - adj_index ];
    // if( ( source_pxl > 5794237 ) && ( source_pxl < 5794242 ) ) {
    //   std::cout << "pixel: " << source_pxl << ". map[ 0 ]: " << map[ 0 ][ source_pxl ] << ". map[ 1 ]: " << map[ 1 ][ source_pxl ] << std::endl;
    //   std::cout << "target_patch_common_elements: " << target_patch_common_elements << std::endl;
    //   std::cout << "target_patch_dist: " << target_patch_dist << std::endl;
    //   std::cout << "target_patch_index: " << target_patch_index << std::endl;
    //   for( size_t patch_adj_idx = 0; patch_adj_idx < patch_adj_size; ++patch_adj_idx ) {
    // 	if( patch_adj_itr.AdjIdx( source_pxl, patch_adj_idx, source_adj_pxl ) &&
    // 	    ( patch_adj_itr.AdjIdx( map[ 0 ][ source_pxl ], patch_adj_idx, target_adj_pxl ) ) ) {
    // 	  std::cout << patch_adj_idx << ": src img: " << img[ source_adj_pxl ] << ", tgt img: " << img[ target_adj_pxl ] << std::endl;
    // 	}
    //   }
    //   for( size_t patch_adj_idx = 0; patch_adj_idx < patch_adj_size; ++patch_adj_idx ) {
    // 	if( patch_adj_itr.AdjIdx( source_pxl, patch_adj_idx, source_adj_pxl ) &&
    // 	    ( patch_adj_itr.AdjIdx( map[ 1 ][ source_pxl ], patch_adj_idx, target_adj_pxl ) ) ) {
    // 	  std::cout << patch_adj_idx << ": src img: " << img[ source_adj_pxl ] << ", tgt img: " << img[ target_adj_pxl ] << std::endl;
    // 	}
    //   }
    // }
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
    try {
      size_t total_threads = 12;
      Vector< std::thread > threads;
      for( size_t thd = 0; thd < total_threads; ++thd ) {
	threads.push_back( std::thread( &PatchSimilarityMap< D >, std::ref( map ), std::ref( img ),
					search_radius, patch_radius, patch_adjacents,
					thd, total_threads ) );
      }
      for( size_t thd = 0; thd < total_threads; ++thd ) {
	threads( thd ).join( );
      }
    }
    catch( std::exception &e ) {
      BIAL_WARNING( "Failed to run in multi-thread. Exception: " << e.what( ) );
      PatchSimilarityMap( map, img, search_radius, patch_radius, patch_adjacents, 0, 1 );
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
