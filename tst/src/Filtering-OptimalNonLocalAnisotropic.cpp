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


template< class D >
float NonLocalEdgeRegionKappa( const Image< D > &source, const Image< D > &mask, 
			       const DiffusionFunction *diff_func, const Vector< Image< D > > &map ) {
  try {
    float best_proportion = 1.01;
    COMMENT( "Get initial kappa, minimum standard deviation of flat region, and maximum standard deviation of edge "
	     << "region.", 3 );
    Vector< int > edges;
    for( size_t pxl = 0; pxl < mask.size( ); ++pxl ) {
      if( mask[ pxl ] != 0 )
	edges.push_back( source[ pxl ] );
    }
    float max_std = Statistics::StandardDeviation( edges );
    COMMENT( "Estimate best edge kappa based on the maximum standard deviation in a binary search.", 3 );
    float kappa = 2 * max_std;
    float step = kappa / 2.0;
    Image< D > filtered = NonLocalAnisotropicDiffusion( source, diff_func, kappa, 1, map );
    size_t elm = 0;
    for( size_t pxl = 0; pxl < mask.size( ); ++pxl ) {
      if( mask[ pxl ] != 0 ) {
	edges( elm ) = filtered[ pxl ];
	++elm;
      }
    }
    float best_std = Statistics::StandardDeviation( edges );
    COMMENT( "Binary search for the best kappa.", 3 );
    while( step > 2.0 ) {
      COMMENT( "Searching to the right.", 4 );
      filtered = NonLocalAnisotropicDiffusion( source, diff_func, kappa + step, 1, map );
      for( size_t pxl = 0, elm = 0; pxl < mask.size( ); ++pxl ) {
	if( mask[ pxl ] != 0 ) {
	  edges( elm ) = filtered[ pxl ];
	  ++elm;
	}
      }
      float std = Statistics::StandardDeviation( edges );
      if( std::fabs( std - max_std / best_proportion ) <= std::fabs( best_std - max_std / best_proportion ) ) {
	best_std = std;
	kappa = kappa + step;
      }
      else {
	COMMENT( "Searching to the left.", 4 );
	filtered = NonLocalAnisotropicDiffusion( source, diff_func, kappa - step, 1, map );
	for( size_t pxl = 0, elm = 0; pxl < mask.size( ); ++pxl ) {
	  if( mask[ pxl ] != 0 ) {
	    edges( elm ) = filtered[ pxl ];
	    ++elm;
	  }
	}
	std = Statistics::StandardDeviation( edges );
	if( std::fabs( std - max_std / best_proportion ) <= std::fabs( best_std - max_std / best_proportion ) ) {
	  best_std = std;
	  kappa = kappa - step;
	}
      }
      step /= 2.0;
    }
    COMMENT( "edge std: " << best_std << ", edge kappa: " << kappa, 3 );
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
float NonLocalFlatRegionKappa( const Image< D > &source, const Image< D > &mask, 
			       const DiffusionFunction *diff_func, float kappa,
			       const Vector< Image< D > > &map ) {
  try {
    COMMENT( "Get initial kappa, minimum standard deviation of flat region, and maximum standard deviation of edge region.", 0 );
    COMMENT( "Initial edge kappa: " << kappa, 0 );
    Vector< int > backg;
    for( size_t pxl = 0; pxl < mask.size( ); ++pxl ) {
      if( mask[ pxl ] != 0 )
	backg.push_back( source[ pxl ] );
    }
    COMMENT( "Getting first standard deviation given by input image.", 0 );
    float base_std = Statistics::StandardDeviation( backg );
    COMMENT( "Getting standard deviation given by strong mean filter of input image.", 0 );
    Image< D > filtered( NonLocalAnisotropicDiffusion( source, diff_func, kappa, 1, map ) );
    
    for( size_t pxl = 0, elm = 0; pxl < mask.size( ); ++pxl ) {
      if( mask[ pxl ] != 0 ) {
	backg[ elm ] = filtered[ pxl ];
	++elm;
      }
    }
    float best_std = Statistics::StandardDeviation( backg );
    COMMENT( "base_std: " << base_std << ". best_std: " << best_std, 0 );
    COMMENT( "Base standard deviation: " << base_std << ".", 0 );
    COMMENT( "Estimate best kappa for flat region based on the base and smooth standard deviation values in a binary search.", 0 );
    COMMENT( "Start searching for the kappa which gives the lowest standard deviation. Looking for the point in " <<
	     "which the standard deviaction varies less than 1% and is close to smooth standard deviation value.", 0 );
    float step = kappa / 2;
    bool change_dir = false;
    while( step > 2.0 ) {
      COMMENT( "Searching to the right of current kappa.", 3 );
      filtered = NonLocalAnisotropicDiffusion( source, diff_func, kappa + step, 1, map );
      
      for( size_t pxl = 0, elm = 0; pxl < mask.size( ); ++pxl ) {
	if( mask[ pxl ] != 0 ) {
	  backg( elm ) = filtered[ pxl ];
	  ++elm;
	}
      }
      float std = Statistics::StandardDeviation( backg );
      COMMENT( "Searching right with kappa: " << kappa + step << " and std: " << std, 0 );
      COMMENT( "Checking if standard deviation varies in more than 1%.", 3 );
      if( ( std > 0.8f * base_std ) && ( std > 0.99f * best_std ) ) {
	best_std = std;
	kappa = kappa + step;
	COMMENT( "New kappa: " << kappa, 0 );
	if( change_dir )
	  step /= 2.0;
      }
      else {
	COMMENT( "Searching to the left of current kappa.", 3 );
	filtered = NonLocalAnisotropicDiffusion( source, diff_func, kappa - step, 1, map );
	
	for( size_t pxl = 0, elm = 0; pxl < mask.size( ); ++pxl ) {
	  if( mask[ pxl ] != 0 ) {
	    backg( elm ) = filtered[ pxl ];
	    ++elm;
	  }
	}
	std = Statistics::StandardDeviation( backg );
	COMMENT( "Searching left with kappa: " << kappa - step << " with std: " << std, 0 );
	COMMENT( "Checking if standard deviation varies in more than 1%.", 3 );
	if( std < best_std * 1.01f ) {
	  best_std = std;
	  kappa = kappa - step;
	  COMMENT( "New kappa: " << kappa, 0 );
	  change_dir = true;
	  step /= 2.0;
	}
	else {
	  COMMENT( "No update.", 0 );
	  change_dir = true;
	  step /= 2.0;
	}
      }
    }
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
						 float init_kappa, const Vector< Image< D > > &map ) {
  try {
    COMMENT( "Avoid flitering flat image.", 1 );
    if( init_kappa <= 50.0 ) {
      return( Image< D >( img ) );
    }
    COMMENT( "Initial filtering with kappa: " << init_kappa, 1 );



    // Parei aqui! Tirar calculo de locais de dentro de nonlocal....

    
    
    Image< D > res = NonLocalAnisotropicDiffusion( img, diff_func, init_kappa, 1, map );
    COMMENT( "Reducing kappa.", 1 );
    float kappa = init_kappa - diff_func->Reduction( init_kappa );
    COMMENT( "Filtering until low value of kappa is reached.", 1 );
    while( kappa > 50.0 ) {
      COMMENT( "New kappa: " << kappa, 2 );
      std::swap( img, res );
      res = NonLocalAnisotropicDiffusion( img, diff_func, kappa, 1, map );
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
						const float patch_radius, const size_t patch_adjacents ) {
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
    COMMENT( "Computing initial kappa using edge and flat regions.", 0 );
    std::cout << "Computing edge kappa." << std::endl;
    float edge_kappa = NonLocalEdgeRegionKappa( img, canny, diff_func, map );
    std::cout << "Computing flat kappa." << std::endl;
    float flat_kappa = NonLocalFlatRegionKappa( img, backg, diff_func, edge_kappa, map );
    float init_kappa = ( edge_kappa + flat_kappa ) / 2.0;
    if( edge_kappa < flat_kappa ) {
      init_kappa = edge_kappa + conservativeness * ( flat_kappa - edge_kappa );
    }
    COMMENT( "edge_kappa: " << edge_kappa << ", flat_kappa: " << flat_kappa << ", init_kappa: " << init_kappa, 0 );
    std::cout << "edge_kappa: " << edge_kappa << ", flat_kappa: " << flat_kappa << ", init_kappa: " << init_kappa << std::endl;
    COMMENT( "Returning adaptive filter based on initial kappa value.", 0 );
    return( AdaptiveNonLocalAnisotropicDiffusion( img, diff_func, init_kappa, map ) );
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
						float conservativeness, const float search_radius, const float patch_radius,
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
						 search_radius, patch_radius, patch_adjacents ) );
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
  if( ( argc < 4 ) || ( argc == 6 ) || ( argc > 10 ) ) {
    cout << "Usage: " << argv[ 0 ] << " <Input image> <output image> <conservativeness> [<diffusion function> "
         << "[<edge_region> <flat_region> [<non-local search adjacency radius> [<non-local patch adjacency radius> [<non-local patch adjacents>]]]]]" << endl;
    cout << "\t\t<conservativeness>: (conserve all edges) 0.0 to 1.0 (remove all noise)." << endl;
    cout << "\t\t<diffusion function>: 0: Power(1.0); 1: Power(2.0); 2: Gaussian; 3: Robust."
         << "Default: 3." << endl;
    cout << "\t\t<edge_region>: Canny edge detection." << endl;
    cout << "\t\t<flat_region>: Background segmentation." << endl;
    cout << "\t\t<non-local search adjacency radius>: 0.0 to 10.0. Default: 5.00." << endl;
    cout << "\t\t<non-local patch adjacency radius>: 1.0 to 5.0. Default: 3.00." << endl;
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
  size_t patch_adjacents = 1;
  if( argc > 9 ) {
    patch_adjacents = atoi( argv[ 9 ] );
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
							   search_radius, patch_radius, patch_adjacents ) );
    Write( res, argv[ 2 ] );
  }
  else {
    std::cout << "Computing flat and edge region images." << std::endl;
    Image< int > res( OptimalNonLocalAnisotropicDiffusion( src, diff_func, conservativeness,
							   search_radius, patch_radius, patch_adjacents ) );
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

