/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2018/Oct/10 */
/* Content: Test file. */
/* Description: Testing otimized non-local anisotropic filter. */

#include "Common.hpp"
#include "DiffusionFunction.hpp"
#include "FileImage.hpp"
#include "Filtering-NonLocalAnisotropicBasics.cpp"
#include "DiffusionFunction.hpp"
#include "GradientCanny.hpp"
#include "SegmentationBackground.hpp"
#include "StatisticsStdDev.hpp"

using namespace std;
using namespace Bial;


Vector< double > NonLocalWeightVector( const Adjacency &adj, const size_t adj_size, const size_t img_dims ) {
  try {
    COMMENT( "Creating weight applied to each adjacent pixel based to its relative position to the filtered pixel.", 2 );
    Vector< double > weight( 2 * adj_size, 0 );
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
      double distance = 0.0;
      for( size_t dim = 0; dim < img_dims; ++dim )
	distance += ( 1 + adj( adj_idx, dim ) ) * ( 1 + adj( adj_idx, dim ) );
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
					       const size_t patch_adjacents ) {
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
      double distance = 0.0;
      for( size_t dim = 0; dim < img_dims; ++dim )
	distance += 1 + std::abs( adj( idx, dim ) );
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
    COMMENT( "Get initial kappa, minimum standard deviation of flat region, and maximum standard deviation of edge "
	     << "region.", 3 );
    COMMENT( "Computing mask vector.", 3 );
    Vector< size_t > mask_vector;
    for( size_t pxl = 0; pxl < mask.size( ); ++pxl ) {
      if( mask[ pxl ] != 0 )
	mask_vector.push_back( pxl );
    }
    size_t mask_vector_size = mask_vector.size( );
    COMMENT( "edges pixel vector.", 3 );
    Vector< int > edges( mask_vector_size );
    for( size_t pxl = 0; pxl < mask_vector_size; ++pxl )
      edges[ pxl ] = source[ mask_vector[ pxl ] ];
    float base_std = Statistics::StandardDeviation( edges );

    COMMENT( "Estimate best edge kappa based on the maximum standard deviation in a binary search.", 3 );
    float kappa = base_std / 50;
    step = kappa;
    Image< D > filtered( QuickNonLocalAnisotropicDiffusion( source, mask_vector, diff_func, kappa, adj,
							    adj_itr, weight, integration_constant, map ) );
    for( size_t pxl = 0; pxl < mask_vector_size; ++pxl )
      edges[ pxl ] = filtered[ mask_vector[ pxl ] ];
    float prev_std = Statistics::StandardDeviation( edges );
    Vector< float > kappas;
    Vector< float > std_diff;
    kappas.push_back( kappa );
    std_diff.push_back( 0.0f );
    float max_std_diff = 0.0f;
    do {
      filtered = QuickNonLocalAnisotropicDiffusion( source, mask_vector, diff_func, kappa + step, adj, adj_itr,
						    weight, integration_constant, map );
      for( size_t pxl = 0; pxl < mask_vector_size; ++pxl )
	edges[ pxl ] = filtered[ mask_vector[ pxl ] ];
      float std = Statistics::StandardDeviation( edges );
      kappas.push_back( kappa + step );
      std_diff.push_back( prev_std - std );
      if( max_std_diff < prev_std - std )
	max_std_diff = prev_std - std;
      prev_std = std;
      kappa += step;
    } while( ( kappa < base_std ) || ( std_diff[ std_diff.size( ) - 2 ] > std_diff[ std_diff.size( ) - 1 ] ) );
    COMMENT( "max_std_diff: " << max_std_diff << ", std_diff: " << std_diff << "kappas: " << kappas, 3 );
    for( size_t idx = 1; idx < std_diff.size( ); ++idx ) {
      if( std_diff[ idx ] > 0.01 * max_std_diff ) {
	COMMENT( "edge kappa: " << kappas[ idx ], 3 );
	return( kappas[ idx ] );
      }
    }
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
    COMMENT( "Get initial kappa, minimum standard deviation of flat region, and maximum standard deviation of edge region.", 0 );
    float kappa = step;
    Vector< int > backg( mask_vector_size );
    COMMENT( "Getting standard deviation given by strong mean filter of input image.", 0 );
    Image< D > filtered( QuickNonLocalAnisotropicDiffusion( source, mask_vector, diff_func, kappa, adj, adj_itr,
							    weight, integration_constant, map ) );
    for( size_t pxl = 0; pxl < mask_vector_size; ++pxl )
      backg[ pxl ] = filtered[ mask_vector[ pxl ] ];
    float prev_std = Statistics::StandardDeviation( backg );
    COMMENT( "Estimate best kappa for flat region.", 0 );
    float max_diff = 0.0f;
    float diff;
    do {
      filtered = QuickNonLocalAnisotropicDiffusion( source, mask_vector, diff_func, kappa + step, adj, adj_itr,
						    weight, integration_constant, map );
      for( size_t pxl = 0; pxl < mask_vector_size; ++pxl )
	backg[ pxl ] = filtered[ mask_vector[ pxl ] ];
      float std = Statistics::StandardDeviation( backg );
      diff = prev_std - std;
      if( max_diff < diff )
	max_diff = diff;
      COMMENT( "prev_std: " << prev_std << ", std: " << std << ", max_diff: " << max_diff
	       << ", diff: " << diff << ", kappa: " << kappa << ", step: " << step, 3 );
      kappa += step;
      prev_std = std;
    } while( diff > 0.01 * max_diff );
    COMMENT( "flat std: " << best_std << ", flat kappa: " << kappa, 0 );
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

template< class D >
Image< D > AdaptiveNonLocalAnisotropicDiffusion( Image< D > img, const DiffusionFunction *diff_func,
						 float init_kappa, const Adjacency &adj,
						 const AdjacencyIterator &adj_itr, const Vector< double > &weight,
						 double integration_constant, const Vector< Image< D > > &map ) {
  try {
    COMMENT( "Avoid flitering flat image.", 1 );
    if( init_kappa <= 10.0 )
      return( Image< D >( img ) );
    COMMENT( "Initial filtering with kappa: " << init_kappa, 1 );
    Image< D > res = QuickNonLocalAnisotropicDiffusion( img, diff_func, init_kappa, adj, adj_itr, weight,
							integration_constant, map );
    COMMENT( "Reducing kappa.", 1 );
    float kappa = init_kappa - diff_func->Reduction( init_kappa );
    COMMENT( "Filtering until low value of kappa is reached.", 1 );
    while( kappa > 10.0 ) {
      COMMENT( "New kappa: " << kappa, 2 );
      std::swap( img, res );
      res = QuickNonLocalAnisotropicDiffusion( img, diff_func, kappa, adj, adj_itr, weight, integration_constant, map );
      kappa -= diff_func->Reduction( kappa );
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
						const float patch_radius, const float flow_radius,
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
    Vector< Image< D > > map( patch_adjacents, Image< D >( img.Dim( ) ) ); // Resulting map with a image for each patch adjacent.
    try {
      size_t total_threads = 12;
      Vector< std::thread > threads;
      for( size_t thd = 0; thd < total_threads; ++thd ) {
	threads.push_back( std::thread( &PatchSimilarityMap< D >, std::ref( map ), std::ref( img ),
					search_radius, patch_radius, patch_adjacents, thd, total_threads ) );
      }
      for( size_t thd = 0; thd < total_threads; ++thd ) {
	threads( thd ).join( );
      }
    }
    catch( std::exception &e ) {
      BIAL_WARNING( "Failed to run in multi-thread. Exception: " << e.what( ) );
      PatchSimilarityMap( map, img, search_radius, patch_radius, patch_adjacents, 0, 1 );
    }
    std::cout << "Finished computing patches." << std::endl;
    COMMENT( "Creating initial data structures.", 0 );
    Adjacency adj( AdjacencyType::HyperSpheric( flow_radius, img.Dims( ) ) );
    AdjacencyIterator adj_itr( img, adj );
    size_t adj_size = adj.size( );
    size_t img_dims = img.Dims( );
    COMMENT( "Creating weight applied to each adjacent pixel based to its relative position to the filtered pixel.", 2 );
    Vector< double > weight( NonLocalWeightVector( adj, adj_size, img_dims ) );
    COMMENT( "Computing integration constant.", 2 );
    double integration_constant = NonLocalIntegrationConstant( adj, adj_size, img_dims, patch_adjacents );
    COMMENT( "Computing initial kappa using edge and flat regions.", 0 );
    float step;
    float edge_kappa = NonLocalEdgeRegionKappa( img, canny, diff_func, weight, integration_constant, adj, adj_itr,
						step, map );
    std::cout << "edge_kappa: " << edge_kappa << std::endl;
    float flat_kappa = NonLocalFlatRegionKappa( img, backg, diff_func, weight, integration_constant, adj, adj_itr,
						step, map );
    std::cout << "flat_kappa: " << flat_kappa << std::endl;
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
						const float patch_radius, const float flow_radius,
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
						 search_radius, patch_radius, flow_radius, patch_adjacents ) );
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

int main( int argc, char **argv ) {
  if( ( argc < 4 ) || ( argc == 6 ) || ( argc > 11 ) ) {
    cout << "Usage: " << argv[ 0 ] << " <Input image> <output image> <conservativeness> [<diffusion function> "
         << "[<edge_region> <flat_region> [<non-local search adjacency radius> [<non-local patch adjacency radius> "
	 << "[<flow adjacency radius> [<non-local patch adjacents>]]]]]]" << endl;
    cout << "\t\t<conservativeness>: (conserve all edges) 0.0 to 1.0 (remove all noise)." << endl;
    cout << "\t\t<diffusion function>: 0: Power(1.0); 1: Power(2.0); 2: Gaussian; 3: Robust."
         << "Default: 3." << endl;
    cout << "\t\t<edge_region>: Canny edge detection." << endl;
    cout << "\t\t<flat_region>: Background segmentation." << endl;
    cout << "\t\t<non-local search adjacency radius>: 0.0 to 10.0. Default: 5.00." << endl;
    cout << "\t\t<non-local patch adjacency radius>: 1.0 to 5.0. Default: 3.00." << endl;
    cout << "\t\t<flow adjacency radius>: 1.01 to 2.0. Default: 1.01." << endl;
    cout << "\t\t<non-local patch adjacents>: 0 to 4. Default: 1." << endl;
    return( 0 );
  }

  /* Reading inputs: */
  Image< int > src( Read< int >( argv[ 1 ] ) );
  float conservativeness = atof( argv[ 3 ] );
  if( ( conservativeness < 0.0 ) || ( conservativeness > 1.0 ) ) {
    cout << "Error: Invalid conservativeness parameter. Expected: 0.0 to 1.0. Found: " << conservativeness << endl;
    return( 0 );
  }
  DiffusionFunction *diff_func;
  if( argc < 5 ) {
    diff_func = new RobustDiffusionFunction;
  }
  else {
    int func = atof( argv[ 4 ] );
    if( func == 0 ) {
      diff_func = new PowerDiffusionFunction( 1.0 );
    }
    else if( func == 1 ) {
      diff_func = new PowerDiffusionFunction( 2.0 );
    }
    else if( func == 2 ) {
      diff_func = new GaussianDiffusionFunction;
    }
    else if( func == 3 ) {
      diff_func = new RobustDiffusionFunction;
    }
    else {
      cout << "Error: Invalid diffusion function option. Expected: 0 to 4. Found: " << func << endl;
      return( 0 );
    }
  }
  float search_radius = 5.00;
  if( argc > 7 ) {
    search_radius = atof( argv[ 7 ] );
    if( ( search_radius < 0.0 ) || ( search_radius > 10.0 ) ) {
      cout << "Error: Invalid search adjacency radius. Expected: 0.0 to 10.0. Found: " << search_radius << endl;
      return( 0 );
    }
  }
  float patch_radius = 3.00;
  if( argc > 8 ) {
    patch_radius = atof( argv[ 8 ] );
    if( ( patch_radius < 1.0 ) || ( search_radius > 5.0 ) ) {
      cout << "Error: Invalid patch adjacency radius. Expected: 1.0 to 5.0. Found: " << patch_radius << endl;
      return( 0 );
    }
  }
  float flow_radius = 1.01;
  if( argc > 9 ) {
    flow_radius = atof( argv[ 9 ] );
    if( ( flow_radius < 1.0 ) || ( flow_radius > 2.0 ) ) {
      cout << "Error: Invalid flow adjacency radius. Expected: 1.01 to 2.0. Found: " << flow_radius << endl;
      return( 0 );
    }
  }
  size_t patch_adjacents = 1;
  if( argc > 10 ) {
    patch_adjacents = atoi( argv[ 10 ] );
    if( ( patch_adjacents < 0 ) || ( patch_adjacents > 4 ) ) {
      cout << "Error: Invalid patch adjacents. Expected: 0 to 4. Found: " << patch_adjacents << endl;
      return( 0 );
    }
  }
  if( argc > 5 ) {
    std::cout << "Using input images for flat and edge regions." << std::endl;
    Image< int > edge_region( Read< int >( argv[ 5 ] ) );
    Image< int > flat_region( Read< int >( argv[ 6 ] ) );
    Image< int > res( OptimalNonLocalAnisotropicDiffusion( src, diff_func, conservativeness, edge_region, flat_region,
							   search_radius, patch_radius, flow_radius, patch_adjacents ) );
    Write( res, argv[ 2 ] );
  }
  else {
    std::cout << "Computing flat and edge region images." << std::endl;
    Image< int > res( OptimalNonLocalAnisotropicDiffusion( src, diff_func, conservativeness, search_radius,
							   patch_radius, flow_radius, patch_adjacents ) );
    Write( res, argv[ 2 ] );
  }
  if( argc < 5 ) {
    delete ( dynamic_cast< RobustDiffusionFunction* >( diff_func ) );
  }
  else {
    int func = atof( argv[ 4 ] );
    if( func == 0 ) {
      delete ( dynamic_cast< PowerDiffusionFunction* >( diff_func ) );
    }
    else if( func == 1 ) {
      delete ( dynamic_cast< PowerDiffusionFunction* >( diff_func ) );
    }
    else if( func == 2 ) {
      delete ( dynamic_cast< GaussianDiffusionFunction* >( diff_func ) );
    }
    else if( func == 3 ) {
      delete ( dynamic_cast< RobustDiffusionFunction* >( diff_func ) );
    }
    else { /* if( func == 4 ) */
      delete ( dynamic_cast< RobustDiffusionFunction* >( diff_func ) );
    }
  }
  return( 0 );
}

