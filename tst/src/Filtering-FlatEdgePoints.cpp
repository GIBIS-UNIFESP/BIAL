/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2018/Nov/07 */
/* Content: Test file. */
/* Description: Computes sets of Edge and Flat pixels with standard deviation different of zero in their adjacency. */

// #include "AdjacencyIterator.hpp"
// #include "AdjacencyRound.hpp"
#include "DiffusionFunction.hpp"
#include "FileImage.hpp"
#include "Filtering-NonLocalAnisotropicBasics.cpp"
// #include "Histogram.hpp"
#include "Image.hpp"
// #include "Signal.hpp"
// #include "SignalAccumulate.hpp"
// #include "SignalNormalize.hpp"

using namespace std;
using namespace Bial;

// template< class D >
// bool FlatEdgePixels( const Image< D > &img, Image< D > &edge_img, Image< D > &flat_img ) {
//   size_t img_size = img.size( );
//   if( img_size < 2000 ) {
//     std::cout << "Input image is too small. Minimum required: 2000 pixels." << std::endl;
//     return( false );
//   }
//   Adjacency adj( AdjacencyType::HyperSpheric( 1.8, img.Dims( ) ) );
//   AdjacencyIterator adj_itr( img, adj );
//   size_t adj_size = adj.size( );
//   COMMENT( "Computing non-zero standard deviation adjacency.", 0 );
//   Image< float > std_img( img.Dim( ) );
//   size_t non_zero_std_pixels = 0;
//   for( size_t src_pxl = 0; src_pxl < img_size; ++src_pxl ) {
//     size_t adj_pxl;
//     float mean = 0.0f;
//     size_t adjs = 0;
//     for( size_t adj_idx = 0; adj_idx < adj_size; ++adj_idx ) {
//       if( adj_itr.AdjIdx( src_pxl, adj_idx, adj_pxl ) ) {
// 	mean += std::abs( img[ src_pxl ] - img[ adj_pxl ] );
// 	++adjs;
//       }
//     }
//     mean /= adjs;
//     float std_dev = 0.0f;
//     for( size_t adj_idx = 0; adj_idx < adj_size; ++adj_idx ) {
//       if( adj_itr.AdjIdx( src_pxl, adj_idx, adj_pxl ) )
// 	std_dev += ( mean - img[ adj_pxl ] ) * ( mean - img[ adj_pxl ] );
//     }
//     std_dev = std::sqrt( std_dev / ( adjs - 1 ) );
//     if( std_dev != 0.0f ) {
//       std_img[ src_pxl ] = std_dev;
//       ++non_zero_std_pixels;
//     }
//   }
//   COMMENT( "Avoiding low standard deviation variation.", 0 );
//   float min_std = std::numeric_limits< float >::max( );
//   float max_std = 0;
//   for( size_t pxl = 0; pxl < img_size; ++pxl ) {
//     if( ( min_std > std_img[ pxl ] ) && ( std_img[ pxl ] > 0.0f ) )
//       min_std = std_img[ pxl ];
//     if( max_std < std_img[ pxl ] )
//       max_std = std_img[ pxl ];
//   }
//   std::cout << "Image min_std: " << min_std << ", max_std: " << max_std << std::endl;
//   if( max_std - min_std <= adj_size ) {
//     std::cout << "Input image is almost flat. No filtering required." << std::endl;
//     return( false );
//   }
//   std::cout << "non_zero_std_pixels: " << non_zero_std_pixels << std::endl;
//   if( non_zero_std_pixels < 2000 )  {
//     std::cout << "Input image is almost flat with less than 2000 pixels with non-zero adjacents. No filtering required."
// 	      << std::endl;
//     return( false );
//   }
//   COMMENT( "Computing the histogram of standard deviation.", 0 );
//   Signal std_hist( SignalType::Histogram( std_img, ( max_std - min_std + 1.0 ) / 100.0f ) );
//   std_hist[ 0 ] = 0.0;
//   SignalOp::Normalize( std_hist );
//   SignalOp::Accumulate( std_hist );
//   COMMENT( "std_hist: " << std_hist, 2 );
//   size_t std_hist_size = std_hist.size( );
//   COMMENT( "Histogram size: " << std_hist_size << ". Finding the 1/n sample bins.", 0 );
//   std::cout << "Histogram size: " << std_hist_size << ". Finding the 1/n sample bins." << std::endl;
//   COMMENT( "Histogram: " << std_hist, 4 );
//   size_t bins = 6;
//   Vector< size_t > tenth_bins( bins );
//   size_t src_bin = 0;
//   for( size_t bin = 0; bin < bins; ++bin ) {
//     while( std_hist[ src_bin ] < ( bin + 1.0 ) / bins )
//       ++src_bin;
//     tenth_bins[ bin ] = src_bin;
//   }
//   //std::cout << "tenth_bins: " << tenth_bins << std::endl;
//   //std::cout << "tenth_bins[ 0 ].freq: " << std_hist[ tenth_bins[ 0 ] ] << ", tenth_bins[ " << bins - 2 << " ].freq: " << std_hist[ tenth_bins[ bins - 2 ] ] << std::endl;
//   //std::cout << "tenth_bins[ 0 ].data: " << std_hist.Data( tenth_bins[ 0 ] ) << ", tenth_bins[ " << bins - 2 << " ].data: " << std_hist.Data( tenth_bins[ bins - 2 ] ) << std::endl;
//   COMMENT( "Computing the number of flat and edge samples according to non-zero standard deviation pixels.", 0 );
//   size_t total_samples = std::min( std::max( static_cast< size_t >( 10000 ), non_zero_std_pixels / 1000 ),
// 				   non_zero_std_pixels / 2 );
//   std::cout << "total_samples: " << total_samples << std::endl;
//   COMMENT( "Sampling pixels.", 0 );
//   Vector< bool > chosen( img_size, false );
//   size_t curr_samples = 0;
//   size_t edge_samples = 0;
//   size_t flat_samples = 0;
//   while( curr_samples < total_samples ) {
//     COMMENT( "Selecting unused sample with standard deviation different of zero.", 3 );
//     size_t sample = rand() % img_size;
//     while( ( std_img[ sample ] == 0.0f ) || ( chosen[ sample ] == true ) )
//       sample = ( sample + 1 ) % img_size;
//     if( std_img[ sample ] < std_hist.Data( tenth_bins[ 1 ] ) ) {
//       size_t adj_sample;
//       for( size_t adj_idx = 0; adj_idx < adj_size; ++adj_idx ) {
// 	if( adj_itr.AdjIdx( sample, adj_idx, adj_sample ) )
// 	  flat_img[ adj_sample ] = 1;
//       }
//       ++flat_samples;
//     }
//     if( std_img[ sample ] > std_hist.Data( tenth_bins[ bins - 2 ] ) ) {
//       edge_img[ sample ] = 1;
//       ++edge_samples;
//     }
//     ++curr_samples;
//   }
//   COMMENT( "Outputing results.", 0 );
//   std::cout << "flat samples: " << flat_samples << ", edge samples: " << edge_samples << std::endl;
//   if( ( flat_samples == 0 ) || ( edge_samples == 0 ) ) {
//     std::cout << "Error. Could not find any edge or flat pixels for this image." << std::endl;
//     exit( 0 );
//   }
//   return( true );
// }

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc < 3 ) {
    cout << "Usage: " << argv[ 0 ] << " <Input image> <output basename> [<diffusion function> "
         << "[<patch search adjacency radius> [<patch adjacency radius> "
	 << "[<patch distance> [<non-local patch adjacents> [ <kappa reduction factor> ]]]]]" << endl;
    cout << "\t\t<diffusion function>: 0: Power(1.0); 1: Power(2.0); 2: Gaussian; 3: Robust."
         << "Default: 3." << endl;
    cout << "\t\t<patch search adjacency radius>: 0.0 to 5.0. Default: 3.0." << endl;
    cout << "\t\t<patch adjacency radius>: 1.0 to 1.9. Default: 1.5." << endl;
    cout << "\t\t<patch distance>: 0.01 to 2.0. Default: 0.01." << endl;
    cout << "\t\t<non-local patch adjacents>: 0 to 4. Default: 1." << endl;
    cout << "\t\t<kappa reduction factor>: 0.1 to 1.0. Default: 0.64." << endl;
    return( 0 );
  }
  srand( time( NULL ) );
  /* Reading inputs: */
  Image< int > src( Read< int >( argv[ 1 ] ) );
  DiffusionFunction *diff_func;
  if( argc < 4 ) {
    diff_func = new RobustDiffusionFunction;
  }
  else {
    int func = atof( argv[ 3 ] );
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
  float search_radius = 3.0f;
  if( argc > 4 ) {
    search_radius = atof( argv[ 4 ] );
    if( ( search_radius < 0.0f ) || ( search_radius > 5.0f ) ) {
      cout << "Error: Invalid search adjacency radius. Expected: 0.0 to 5.0. Found: " << search_radius << endl;
      return( 0 );
    }
  }
  float patch_radius = 1.5f;
  if( argc > 5 ) {
    patch_radius = atof( argv[ 5 ] );
    if( ( patch_radius < 1.0f ) || ( patch_radius > 1.9f ) ) {
      cout << "Error: Invalid patch adjacency radius. Expected: 1.0 to 1.9. Found: " << patch_radius << endl;
      return( 0 );
    }
  }
  float patch_distance = 0.01f;
  if( argc > 6 ) {
    patch_distance = atof( argv[ 6 ] );
    if( ( patch_distance < 0.009f ) || ( patch_distance > 2.0f ) ) {
      cout << "Error: Invalid patch distance. Expected: 0.01 to 2.0. Found: " << patch_distance << endl;
      return( 0 );
    }
  }
  size_t patch_adjacents = 1;
  if( argc > 7 ) {
    patch_adjacents = atoi( argv[ 7 ] );
    if( ( patch_adjacents < 0 ) || ( patch_adjacents > 4 ) ) {
      cout << "Error: Invalid patch adjacents. Expected: 0 to 4. Found: " << patch_adjacents << endl;
      return( 0 );
    }
  }
  float reduction = 1.0f;
  if( argc > 8 ) {
    reduction = atof( argv[ 8 ] );
    if( ( reduction < 0.1 ) || ( reduction > 1.0 ) ) {
      cout << "Error: Invalid kappa reduction. Expected: 0.1 to 1.0. Found: " << reduction << endl;
      return( 0 );
    }
  }
  std::cout << "search_radius: " << search_radius << ", patch_radius: " << patch_radius << ", patch_distance: " << patch_distance << ", patch_adjacents: " << patch_adjacents << ", reduction: " << reduction << std::endl;
  Image< int > edge_img( src.Dim( ), src.PixelSize( ) );
  Image< int > flat_img( src.Dim( ), src.PixelSize( ) );
  if( !NoisyPixels( src, flat_img ) )
    return( 0 );
  MultipleOptimalNonLocalAnisotropicDiffusion( src, diff_func, std::string( argv[ 1 ] ), std::string( argv[ 2 ] ),
					       edge_img, flat_img, search_radius, patch_radius, patch_distance,
					       patch_adjacents, reduction );
  if( argc < 4 ) {
    delete ( dynamic_cast< RobustDiffusionFunction* >( diff_func ) );
  }
  else {
    int func = atof( argv[ 3 ] );
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

