/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2018/Nov/07 */
/* Content: Test file. */
/* Description: Computes sets of Edge and Flat pixels with standard deviation different of zero in their adjacency. */

#include "AdjacencyIterator.hpp"
#include "AdjacencyRound.hpp"
#include "DiffusionFunction.hpp"
#include "FileImage.hpp"
#include "Filtering-NonLocalAnisotropicBasics.cpp"
#include "Histogram.hpp"
#include "Image.hpp"
#include "Signal.hpp"
#include "SignalAccumulate.hpp"
#include "SignalNormalize.hpp"

using namespace std;
using namespace Bial;

using namespace std;
using namespace Bial;

template< class D >
void RegionCurve( const Image< D > &source, const Image< D > &mask, 
		  const DiffusionFunction *diff_func, const Vector< double > &weight,
		  const double integration_constant, const Adjacency &adj,
		  const AdjacencyIterator &adj_itr, const Vector< Image< D > > &map ) {
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
    for( float kappa = 10; kappa < 1500; kappa += 5 ) {
      Image< D > filtered( QuickNonLocalAnisotropicDiffusion( source, mask_vector, diff_func, kappa, adj, adj_itr,
							      weight, integration_constant, map ) );
      for( size_t pxl = 0; pxl < mask_vector_size; ++pxl )
	edges[ pxl ] = filtered[ mask_vector[ pxl ] ];
      float std = Statistics::StandardDeviation( edges );
      std::cout << "kappa: " << kappa << ", std: " << std << std::endl;
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
void AnisotropicDiffusionCurves( Image< D > img, const DiffusionFunction *diff_func,
				 const Image< D > &canny, const Image< D > &backg, const float search_radius,
				 const float patch_radius, const float patch_distance, const size_t patch_adjacents ) {
  try {
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
    std::cout << "diff_func: " << diff_func << ", weight: " << weight << ", integration_constant: " << integration_constant << std::endl;
    RegionCurve( img, canny, diff_func, weight, integration_constant, adj, adj_itr, map );
    RegionCurve( img, backg, diff_func, weight, integration_constant, adj, adj_itr, map );
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
  try {
    if( argc < 2 ) {
      cout << "Usage: " << argv[ 0 ] << " <Input image>" << endl;
      return( 0 );
    }
    /* Reading inputs: */
    Image< int > src( Read< int >( argv[ 1 ] ) );
    Image< int > edge_img( src.Dim( ), src.PixelSize( ) );
    Image< int > flat_img( src.Dim( ), src.PixelSize( ) );
    if( !FlatEdgePixels( src, edge_img, flat_img ) )
      return( 0 );
    DiffusionFunction *diff_func;
    diff_func = new RobustDiffusionFunction;
    float search_radius = 3.0f;
    float patch_radius = 1.5f;
    float patch_distance = 1.0f;
    size_t patch_adjacents = 0;
    AnisotropicDiffusionCurves( src, diff_func, edge_img, flat_img, search_radius, patch_radius, patch_distance, patch_adjacents );
    delete ( dynamic_cast< RobustDiffusionFunction* >( diff_func ) );
    return( 0 );
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

