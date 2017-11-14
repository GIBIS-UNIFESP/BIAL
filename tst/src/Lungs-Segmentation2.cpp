/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2013/Nov/05 */
/* Content: Test file. */
/* Description: Test with Lungs images. */

#include "FileImage.hpp"
#include "Image.hpp"
#include "ImageResize.hpp"
#include "Integral.hpp"
#include "SegmentationGeoStar.hpp"
#include "SortingSort.hpp"
#include "Vector.hpp"

using namespace std;
using namespace Bial;

/* Finds the best plane of symmetry. */
size_t Bisection( Image< int > img ) {
  try {
    int best_bisec_col = 0;
    float min_diff = std::numeric_limits< int >::max( );
    for( size_t bisec_col = 0; bisec_col < img.size( 0 ); ++bisec_col ) {
      float diff = 0.0;
      for( size_t row = 0; row < img.size( 1 ); ++row ) {
        for( size_t lft_col = 0; lft_col < img.size( 0 ); ++lft_col ) {

          /* Computing mirror column coordinate, with respect to bisection line. */
          size_t rgt_col = 2 * bisec_col - lft_col;
          /* Updating difference between bipartition. */
          if( rgt_col < img.size( 0 ) ) {
            diff += std::abs( img( lft_col, row ) - img( rgt_col, row ) );
          }
          else {
            diff += img( lft_col, row );
          }
        }
      }
      /* Getting the lowest bisection difference */
      if( diff < min_diff ) {
        min_diff = diff;
        best_bisec_col = bisec_col;
      }
    }
    return( best_bisec_col );
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

/* Tamanho do lado do quadrado.*/
size_t GetPosWindowMaxSum( Image< int > img, size_t window_x_size, size_t window_y_size ) {
  try {
    Image< double > integral( Integral::IntegralImage( img ) );
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

void ComputeSeeds( Image< int > img, Image< int > acc_img, Vector< size_t > &int_seed, Vector< size_t > &ext_seed ) {
  try {
    Image< int > seeds( img.size( 1 ), img.size( 0 ) );

    COMMENT( "Getting maximum intensity position with given window size.", 0 );
    size_t win_xsize = static_cast< size_t >( img.size( 0 ) * 0.35 + 0.5 );
    size_t win_ysize = win_xsize / 2;
    size_t pxl = GetPosWindowMaxSum( acc_img, win_xsize, win_ysize ) + 1;
    size_t col = pxl % img.size( 0 );
    size_t row = pxl / img.size( 0 );

    COMMENT( "Computing seeds that are far from the heart center.", 0 );
    size_t max_distance = static_cast< size_t >( img.size( 0 ) * 0.45 + 0.5 );
    max_distance *= max_distance;
    for( size_t adj_pxl = 0; adj_pxl < img.size( ); ++adj_pxl ) {
      size_t adj_col = adj_pxl % img.size( 0 );
      size_t adj_row = adj_pxl / img.size( 0 );
      size_t dist = ( adj_col - col ) * ( adj_col - col ) + ( adj_row - row ) * ( adj_row - row );
      if( dist >= max_distance ) {
        ext_seed.push_back( adj_pxl );
        seeds[ adj_pxl ] = 1;
      }
    }
    size_t lft = pxl - win_xsize / 2;
    size_t rgt = pxl + win_xsize / 2;
    size_t lft_col = lft % img.size( 0 );
    size_t lft_row = lft / img.size( 0 );
    size_t rgt_col = rgt % img.size( 0 );
    size_t rgt_row = rgt / img.size( 0 );

    COMMENT( "Image area: " << img.Dim( ) << ", Getting left lung area: " <<
             std::to_string( lft_col - win_xsize / 4 ) << " : " <<
             std::to_string( lft_col + win_xsize / 4 ) << " , " <<
             std::to_string( lft_row - win_xsize / 3 ) << " : " <<
             std::to_string( lft_row + win_xsize / 3 ), 0 );
    Image< int > left_lung( ImageOp::Resize( img, { lft_col - win_xsize / 4, lft_row - win_xsize / 3 }, 
                                             { lft_col + win_xsize / 4, lft_row + win_xsize / 3 } ) );
    COMMENT( "Sorting left lung seeds.", 0 );
    Vector< size_t > lft_order = Sorting::Sort( left_lung );
    COMMENT( "Inserting in the internal seed vector.", 0 );
    for( size_t elm = 0; elm <= left_lung.size( ) / 4; ++elm ) {
      size_t lung_pxl = lft_order[ elm ];
      size_t img_col = lft_col - win_xsize / 4 + lung_pxl % left_lung.size( 0 );
      size_t img_row = lft_row - win_xsize / 3 + lung_pxl / left_lung.size( 0 );
      size_t img_pxl = img_col + img_row*img.size( 0 );
      int_seed.push_back( img_pxl );
      seeds[ img_pxl ] = 2;
    }
    COMMENT( "Getting right lung seeds.", 0 );
    Image< int > right_lung( ImageOp::Resize( img, { rgt_col - win_xsize / 4, rgt_row - win_xsize / 3 },
                                              { rgt_col + win_xsize / 4, rgt_row + win_xsize / 3 } ) );

    Vector< size_t > rgt_order = Sorting::Sort( right_lung );
    for( size_t elm = 0; elm <= right_lung.size( ) / 4; ++elm ) {
      size_t lung_pxl = rgt_order[ elm ];
      size_t img_col = rgt_col - win_xsize / 4 + lung_pxl % left_lung.size( 0 );
      size_t img_row = rgt_row - win_xsize / 3 + lung_pxl / left_lung.size( 0 );
      size_t img_pxl = img_col + img_row*img.size( 0 );
      int_seed.push_back( img_pxl );
      seeds[ img_pxl ] = 2;
    }
    COMMENT( "Getting heart seeds", 0 );
    Image< int > heart( ImageOp::Resize( img, { col - win_xsize / 4, row - win_xsize / 4 },
                                         { col + win_xsize / 4, row + win_xsize / 4 } ) );

    Vector< size_t > ctr_order = Sorting::Sort( heart, false );
    for( size_t elm = 0; elm <= heart.size( ) / 4; ++elm ) {
      size_t heart_pxl = ctr_order[ elm ];
      size_t img_col = col - win_xsize / 4 + heart_pxl % heart.size( 0 );
      size_t img_row = row - win_xsize / 4 + heart_pxl / heart.size( 0 );
      size_t img_pxl = img_col + img_row*img.size( 0 );
      ext_seed.push_back( img_pxl );
      seeds[ img_pxl ] = 1;
    }
    COMMENT( "Seeting seeds in bisection.", 0 );
    size_t bisec_col = Bisection( img );
    for( size_t elm = 0; elm <= row; ++elm ) {
      ext_seed.push_back( bisec_col + elm * img.size( 0 ) );
      seeds( bisec_col, elm ) = 1;
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

int main( int argc, char *argv[] ) {
  if( ( argc < 3 ) || ( argc > 5 ) ) {
    cout << "Usage: " << argv[ 0 ] << " <input dir> <output basename> [<alpha> [<beta>]]" << endl;
    cout << "\t\talpha: Oriented edges strength. -1.0 (dark to bright) 0.0 (non-oriented) to 1.0 (bright to dark)." <<
    " Default: 0.5" << endl;
    cout << "\t\tbeta: Geodesic distance force. 0.0 (geodesic distance) to 4.0 (sum function). Default: 0.5" << endl;
    return( 0 );
  }
  try {
    Vector< Image< int > > img( ReadDir< int >( argv[ 1 ] ) );

    float alpha = 0.5;
    float beta = 0.5;
    if( argc > 3 ) {
      alpha = atof( argv[ 3 ] );
    }
    if( argc > 4 ) {
      beta = atof( argv[ 4 ] );
    }
    /* Image< int > sum( img[ 0 ].size( 0 ), img[ 0 ].size( 1 ) ); */
    Image< int > diff( img[ 0 ] );
    diff.Set( 0 );
    for( size_t idx = 1; idx < img.size( ); ++idx ) {
      Image< int > local_diff( img[ idx ] - img[ idx - 1 ] );
      for( size_t pxl = 0; pxl < local_diff.size( ); ++pxl ) {
        if( local_diff[ pxl ] <= 0 )
          local_diff[ pxl ] = -local_diff[ pxl ];
      }
      diff += local_diff;
      /* sum += img[ idx ]; */
    }
    for( size_t slice = 0; slice < img.size( ); ++slice ) {

      /* Getting seeds */
      Vector< size_t > int_seed;
      Vector< size_t > ext_seed;
      ComputeSeeds( img[ slice ], diff, int_seed, ext_seed );

      /* std::cout << "Segmentation... " << std::endl; */
      Image< int > lungs( Segmentation::OrientedGeodesicStar( img[ slice ], int_seed, ext_seed, alpha, beta ) );
      /* std::cout << "writing result... " << std::endl; */
      Write( lungs, argv[ 2 ] + string( "_" ) + to_string( slice + 1 ) + string( ".pgm" ) );

    }
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
