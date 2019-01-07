/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/**
 * @date 2013/Jul/12 
 * @brief Image filtering methods.
 */

#ifndef BIALFILTERINGOPTIMALANISOTROPICDIFFUSION_C
#define BIALFILTERINGOPTIMALANISOTROPICDIFFUSION_C

#include "FilteringOptimalAnisotropicDiffusion.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_FilteringOptimalAnisotropicDiffusion )
#define BIAL_EXPLICIT_FilteringOptimalAnisotropicDiffusion
#endif

#if defined ( BIAL_EXPLICIT_FilteringOptimalAnisotropicDiffusion ) || ( BIAL_IMPLICIT_BIN )

#include "Adjacency.hpp"
#include "AdjacencyRound.hpp"
#include "AdjacencyIterator.hpp"
#include "FilteringAnisotropicDiffusion.hpp"
#include "FilteringMean.hpp"
#include "DiffusionFunction.hpp"
#ifdef BIAL_DEBUG
#include "FileImage.hpp"
#endif
#include "GradientCanny.hpp"
#include "Image.hpp"
#include "SegmentationBackground.hpp"
#include "StatisticsStdDev.hpp"

namespace Bial {

  template< class D >
  float Filtering::EdgeRegionKappa( const Image< D > &source, const Image< D > &mask, 
                                    const DiffusionFunction *diff_func, const Vector< double > &weight,
				    const double integration_constant, const Adjacency &adj,
				    const AdjacencyIterator &adj_itr, float &step ) {
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
      std::cout << "base_std: " << base_std << std::endl;
      COMMENT( "Estimate best edge kappa based on the maximum standard deviation in a binary search.", 3 );
      float kappa = base_std / 50;
      std::cout << "init kappa and step size: " << kappa << std::endl;
      step = kappa;
      Image< D > filtered( Filtering::QuickAnisotropicDiffusion( source, mask_vector, diff_func, kappa, adj, adj_itr, weight, integration_constant ) );
      for( size_t pxl = 0; pxl < mask_vector_size; ++pxl )
	edges[ pxl ] = filtered[ mask_vector[ pxl ] ];
      float prev_std = Statistics::StandardDeviation( edges );
      Vector< float > kappas;
      Vector< float > std_diff;
      kappas.push_back( kappa );
      std_diff.push_back( 0.0f );
      float max_std_diff = 0.0f;
      do {
	filtered = Filtering::QuickAnisotropicDiffusion( source, mask_vector, diff_func, kappa + step, adj, adj_itr, weight, integration_constant );
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
      COMMENT( "max_std_diff: " << max_std_diff << "\nstd_diff: " << std_diff << "\nkappas: " << kappas, 3 );
      std::cout << "max_std_diff: " << max_std_diff << ", std_diff: " << std_diff << "kappas: " << kappas << std::endl;
      for( size_t idx = 1; idx < std_diff.size( ); ++idx ) {
	if( std_diff[ idx ] > 0.05 * max_std_diff ) {
	  COMMENT( "edge kappa: " << kappas[ idx ], 3 );
	  std::cout << "edge kappa index: " << idx << std::endl;
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
  float Filtering::FlatRegionKappa( const Image< D > &source, const Image< D > &mask, 
                                    const DiffusionFunction *diff_func, const Vector< double > &weight,
				    const double integration_constant, const Adjacency &adj,
				    const AdjacencyIterator &adj_itr, float step ) {
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
      std::cout << "init kappa and step size: " << kappa << std::endl;
      Vector< int > backg( mask_vector_size );
      COMMENT( "Getting standard deviation given by strong mean filter of input image.", 0 );
      Image< D > filtered( Filtering::QuickAnisotropicDiffusion( source, mask_vector, diff_func, kappa, adj, adj_itr,
								 weight, integration_constant ) );
      for( size_t pxl = 0; pxl < mask_vector_size; ++pxl )
	backg[ pxl ] = filtered[ mask_vector[ pxl ] ];
      float prev_std = Statistics::StandardDeviation( backg );
      COMMENT( "Estimate best kappa for flat region.", 0 );
      float max_diff = 0.0f;
      Vector< float > diff( 4, 0.0 );
      do {
	filtered = Filtering::QuickAnisotropicDiffusion( source, mask_vector, diff_func, kappa + step, adj, adj_itr,
							 weight, integration_constant );
	for( size_t pxl = 0; pxl < mask_vector_size; ++pxl )
	  backg[ pxl ] = filtered[ mask_vector[ pxl ] ];
	float std = Statistics::StandardDeviation( backg );
	COMMENT( "Computing average difference in standard deviation.", 3 );
	diff[ 0 ] = diff[ 1 ];
	diff[ 1 ] = diff[ 2 ];
	diff[ 2 ] = prev_std - std;
	diff[ 3 ] = diff[ 0 ] + diff[ 1 ] + diff[ 2 ];
	if( max_diff < diff[ 3 ] )
	  max_diff = diff[ 3 ];
	COMMENT( "Checking for small increase before decreasing standard deviation. In this case, do not consider previous decrease values.", 3 );
	if( diff[ 3 ] < 0.0f )
	  max_diff = 0.0f;
	COMMENT( "prev_std: " << prev_std << ", std: " << std << ", max_diff: " << max_diff
		 << ", mean diff: " << diff[ 3 ] << ", kappa: " << kappa << ", step: " << step, 3 );
	std::cout << "prev_std: " << prev_std << ", std: " << std << ", max_diff: " << max_diff
		  << ", mean diff: " << diff[ 3 ] << ", kappa: " << kappa << ", step: " << step << std::endl;
	kappa += step;
	prev_std = std;
      }
      while( ( diff[ 3 ] > 0.01f * max_diff ) || ( diff[ 3 ] <= 0.0f ) || ( kappa < 800.0f ) );
      std::cout << "Final flat kappa: " << kappa << std::endl;
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
  Image< D > Filtering::OptimalAnisotropicDiffusion( Image< D > img, const DiffusionFunction *diff_func, float radius,
                                                     float conservativeness ) {
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
      return( Filtering::OptimalAnisotropicDiffusion( img, diff_func, radius, conservativeness, canny, backg ) );
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
  Image< D > Filtering::OptimalAnisotropicDiffusion( Image< D > img, const DiffusionFunction *diff_func, float radius,
                                                     float conservativeness, const Image< D > &canny, 
                                                     const Image< D > &backg ) {
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
      COMMENT( "Creating initial data structures.", 0 );
      Adjacency adj( AdjacencyType::HyperSpheric( radius, img.Dims( ) ) );
      AdjacencyIterator adj_itr( img, adj );
      size_t adj_size = adj.size( );
      size_t img_dims = img.Dims( );
      COMMENT( "Creating weight applied to each adjacent pixel based to its relative position to the filtered pixel.", 2 );
      Vector< double > weight( Filtering::WeightVector( adj, adj_size, img_dims ) );
      COMMENT( "Computing integration constant.", 2 );
      double integration_constant = Filtering::IntegrationConstant( adj, adj_size, img_dims );
      
      COMMENT( "Computing initial kappa using edge and flat regions.", 0 );
      std::cout << "Computing initial kappa using edge and flat regions." << std::endl;
      float step;
      float edge_kappa = EdgeRegionKappa( img, canny, diff_func, weight, integration_constant, adj, adj_itr, step );
      std::cout << "edge_kappa: " << edge_kappa << std::endl;
      float flat_kappa = FlatRegionKappa( img, backg, diff_func, weight, integration_constant, adj, adj_itr, step );
      std::cout << "flat_kappa: " << flat_kappa << std::endl;
      float init_kappa = ( edge_kappa + flat_kappa ) / 2.0;
      if( edge_kappa < flat_kappa )
        init_kappa = edge_kappa + conservativeness * ( flat_kappa - edge_kappa );
      COMMENT( "edge_kappa: " << edge_kappa << ", flat_kappa: " << flat_kappa << ", init_kappa: " << init_kappa, 0 );
      std::cout << "edge_kappa: " << edge_kappa << ", flat_kappa: " << flat_kappa << ", init_kappa: " << init_kappa << std::endl;

      COMMENT( "Returning adaptive filter based on initial kappa value.", 0 );
      return( Filtering::AdaptiveAnisotropicDiffusion( img, diff_func, init_kappa, radius ) );
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

#ifdef BIAL_EXPLICIT_FilteringOptimalAnisotropicDiffusion

  template float Filtering::EdgeRegionKappa( const Image< int > &source, const Image< int > &mask,
                                             const DiffusionFunction *diff_func, const Vector< double > &weight,
					     const double integration_constant, const Adjacency &adj,
					     const AdjacencyIterator &adj_itr, float &step );
  template float Filtering::FlatRegionKappa( const Image< int > &source, const Image< int > &mask,
					     const DiffusionFunction *diff_func, const Vector< double > &weight,
					     const double integration_constant, const Adjacency &adj,
					     const AdjacencyIterator &adj_itr, float step );
  template Image< int > Filtering::OptimalAnisotropicDiffusion( Image< int > img, const DiffusionFunction *diff_func,
                                                   float radius, float conservativeness );
  template Image< int > Filtering::OptimalAnisotropicDiffusion( Image< int > img, const DiffusionFunction *diff_func,
                                                   float radius, float conservativeness,
                                                   const Image< int > &canny, const Image< int > &backg );

  template float Filtering::EdgeRegionKappa( const Image< llint > &source, const Image< llint > &mask,
                                             const DiffusionFunction *diff_func, const Vector< double > &weight,
					     const double integration_constant, const Adjacency &adj,
					     const AdjacencyIterator &adj_itr, float &step );
  template float Filtering::FlatRegionKappa( const Image< llint > &source, const Image< llint > &mask,
					     const DiffusionFunction *diff_func, const Vector< double > &weight,
					     const double integration_constant, const Adjacency &adj,
					     const AdjacencyIterator &adj_itr, float step );
  template Image< llint > Filtering::OptimalAnisotropicDiffusion( Image< llint > img, const DiffusionFunction *diff_func,
                                                   float radius, float conservativeness );
  template Image< llint > Filtering::OptimalAnisotropicDiffusion( Image< llint > img, const DiffusionFunction *diff_func,
                                                   float radius, float conservativeness,
                                                   const Image< llint > &canny, const Image< llint > &backg );

  template float Filtering::EdgeRegionKappa( const Image< float > &source, const Image< float > &mask,
                                             const DiffusionFunction *diff_func, const Vector< double > &weight,
					     const double integration_constant, const Adjacency &adj,
					     const AdjacencyIterator &adj_itr, float &step );
  template float Filtering::FlatRegionKappa( const Image< float > &source, const Image< float > &mask,
					     const DiffusionFunction *diff_func, const Vector< double > &weight,
					     const double integration_constant, const Adjacency &adj,
					     const AdjacencyIterator &adj_itr, float step );
  template Image< float > Filtering::OptimalAnisotropicDiffusion( Image< float > img, const DiffusionFunction *diff_func,
                                                   float radius, float conservativeness );
  template Image< float > Filtering::OptimalAnisotropicDiffusion( Image< float > img, const DiffusionFunction *diff_func,
                                                   float radius, float conservativeness,
                                                   const Image< float > &canny, const Image< float > &backg );

  template float Filtering::EdgeRegionKappa( const Image< double > &source, const Image< double > &mask,
                                             const DiffusionFunction *diff_func, const Vector< double > &weight,
					     const double integration_constant, const Adjacency &adj,
					     const AdjacencyIterator &adj_itr, float &step );
  template float Filtering::FlatRegionKappa( const Image< double > &source, const Image< double > &mask,
					     const DiffusionFunction *diff_func, const Vector< double > &weight,
					     const double integration_constant, const Adjacency &adj,
					     const AdjacencyIterator &adj_itr, float step );
  template Image< double > Filtering::OptimalAnisotropicDiffusion( Image< double > img, const DiffusionFunction *diff_func,
                                                   float radius, float conservativeness );
  template Image< double > Filtering::OptimalAnisotropicDiffusion( Image< double > img, const DiffusionFunction *diff_func,
                                                   float radius, float conservativeness,
                                                   const Image< double > &canny, const Image< double > &backg );

#endif

}


#endif

#endif
