/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2013/Nov/05 */
/* Content: Test file. */
/* Description: Test with Lungs images. */

#include "AdjacencyRound.hpp"
#include "FileImage.hpp"
#include "Image.hpp"
#include "Integral.hpp"
#include "MorphologyDilation.hpp"
#include "SegmentationIntensityThreshold.hpp"
#include "SegmentationGeoStar.hpp"
#include "Vector.hpp"

using namespace std;
using namespace Bial;

size_t GetPosWindowMaxSum( Image< int > img, size_t window_x_size, size_t window_y_size ) {
  COMMENT( "Side of the square.", 0 );
  Image< double > integral = Integral::IntegralImage( img );
  size_t best_pos = 0;

  size_t max_sum = 0;
  for( size_t row = window_y_size - 1; row < img.size( 1 ); ++row ) {
    for( size_t col = window_x_size - 1; col < img.size( 0 ); ++col ) {
      double sum = Integral::WindowIntegralValue( integral, Vector< size_t >( { col, row } ),
                                                  Vector< size_t >( { window_x_size, window_y_size } ) );
      if( sum > max_sum ) {
        max_sum = sum;
        best_pos = col - window_x_size / 2 + ( row - window_y_size / 2 ) * img.size( 0 );
      }
    }
  }
  return( best_pos );
}

size_t Bisection( Image< int > img ) {
  COMMENT( "Finds the best plane of symmetry.", 0 );

  int best_bisec_col = 0;
  float min_diff = std::numeric_limits< int >::max( );
  for( size_t bisec_col = 0; bisec_col < img.size( 0 ); ++bisec_col ) {
    float diff = 0.0;
    for( size_t row = 0; row < img.size( 1 ); ++row ) {
      for( size_t lft_col = 0; lft_col < img.size( 0 ); ++lft_col ) {

        COMMENT( "Computing mirror column coordinate, with respect to bisection line.", 4 );
        size_t rgt_col = 2 * bisec_col - lft_col;

        COMMENT( "Updating difference between bipartition.", 4 );
        if( rgt_col < img.size( 0 ) ) {
          diff += std::abs( img( lft_col, row ) - img( rgt_col, row ) );
        }
        else {
          diff += img( lft_col, row );
        }
      }
    }
    COMMENT( "Getting the lowest bisection difference.", 4 );
    if( diff < min_diff ) {
      min_diff = diff;
      best_bisec_col = bisec_col;
    }
  }
  return( best_bisec_col );
}


int main( int argc, char *argv[] ) {
  if( ( argc < 3 ) || ( argc > 5 ) ) {
    cout << "Usage: " << argv[ 0 ] << " <input dir> <output basename> [<alpha> [<beta>]]" << endl;
    cout << "\t\talpha: Oriented edges strength. -1.0 (dark to bright) 0.0 (non-oriented) to 1.0 (bright to dark)." <<
    " Default: 0.5" << endl;
    cout << "\t\tbeta: Geodesic distance force. 0.0 (geodesic distance) to 4.0 (sum function). Default: 0.5" << endl;
    return( 0 );
  }
  Vector< Image< int > > img( ReadDir< int >( argv[ 1 ] ) );

  float alpha = 0.5;
  float beta = 0.5;
  if( argc > 3 ) {
    alpha = atof( argv[ 3 ] );
  }
  if( argc > 4 ) {
    beta = atof( argv[ 4 ] );
  }
  Vector< size_t > obj_seeds; /* = diff_max * sum_min; */
  Vector< size_t > bkg_seeds; /* = sum_max; */

  Image< int > sum( img[ 0 ] );
  Image< int > diff( img[ 0 ] );
  Vector< bool > bkg_seed_tbl( sum.size( ), false );
  diff.Set( 0 );
  for( size_t slice = 1; slice < img.size( ); ++slice ) {
    Image< int > local_diff( img[ slice ] - img[ slice - 1 ] );
    for( size_t pxl = 0; pxl < local_diff.size( ); ++pxl ) {
      if( local_diff[ pxl ] <= 0 )
        local_diff[ pxl ] = -local_diff[ pxl ];
    }
    diff += local_diff;
    sum += img[ slice ];
  }
  int maxval_sum = sum.Maximum( );
  int maxval_diff = diff.Maximum( );
  Image< int > sum_min( sum );
  Segmentation::IntensityThreshold( sum_min, 0,
                                    static_cast< int >( 0.1 * maxval_sum ) );
  Image< int > sum_max( sum );
  Segmentation::IntensityThreshold( sum_max, static_cast< int >( 0.3 * maxval_sum ),
                                    static_cast< int >( maxval_sum ) );
  Image< int > diff_max( diff );
  Segmentation::IntensityThreshold( diff_max, static_cast< int >( 0.3 * maxval_diff ),
                                    static_cast< int >( maxval_diff ) );
  diff_max = Morphology::Dilate( diff_max, AdjacencyType::HyperSpheric( 2.0, sum.Dims( ) ) );

  diff_max *= sum_min;
  for( size_t pxl = 0; pxl < diff_max.size( ); ++pxl ) {
    if( diff_max[ pxl ] != 0 ) {
      obj_seeds.push_back( pxl );
    }
    if( sum_max[ pxl ] != 0 ) {
      bkg_seeds.push_back( pxl );
      bkg_seed_tbl[ pxl ] = true;
    }
  }
  COMMENT( "Getting maximum intensity position with given window size.", 0 );
  size_t win_xsize = static_cast< size_t >( sum.size( 0 ) * 0.35 + 0.5 );
  size_t win_ysize = win_xsize / 2;
  size_t pxl = GetPosWindowMaxSum( diff, win_xsize, win_ysize );
  size_t col = pxl % sum.size( 0 );
  size_t row = pxl / sum.size( 0 );

  COMMENT( "Computing maximum distance that a seed could be located from the maximum intensity position.", 0 );
  size_t max_distance = static_cast< size_t >( sum.size( 0 ) * 0.45 + 0.5 );
  max_distance *= max_distance;
  for( size_t adj_pxl = 0; adj_pxl < sum.size( ); ++adj_pxl ) {
    size_t adj_col = adj_pxl % sum.size( 0 );
    size_t adj_row = adj_pxl / sum.size( 0 );
    size_t dist = ( adj_col - col ) * ( adj_col - col ) + ( adj_row - row ) * ( adj_row - row );
    if( ( dist >= max_distance ) && ( bkg_seed_tbl[ adj_pxl ] == false ) ) {
      bkg_seeds.push_back( adj_pxl );
      bkg_seed_tbl[ adj_pxl ] = true;
    }
  }
  COMMENT( "Seeting seeds in bisection.", 0 );
  size_t bisec_col = Bisection( sum );
  for( size_t elm = 0; elm <= row; ++elm ) {
    if( bkg_seed_tbl[ bisec_col + elm * sum.size( 0 ) ] == false ) {
      bkg_seeds.push_back( bisec_col + elm * sum.size( 0 ) );
      bkg_seed_tbl[ bisec_col + elm * sum.size( 0 ) ] = true;
    }
  }

  COMMENT( "obj seeds: " << obj_seeds.size( ), 0 );
  COMMENT( "bkg seeds: " << bkg_seeds.size( ), 0 );
  for( size_t slice = 0; slice < img.size( ); ++slice ) {
    COMMENT( "Oriented segmentation. Slice:" << slice, 4 );
    Image< int > res( Segmentation::OrientedGeodesicStar( img[ slice ], obj_seeds, bkg_seeds, alpha, beta ) );
    Write( res, argv[ 2 ] + string( "_" ) + to_string( slice + 1 ) + string( ".pgm" ) );
  }
  return( 0 );
}
