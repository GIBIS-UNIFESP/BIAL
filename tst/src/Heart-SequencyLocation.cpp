/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2013/Nov/05 */
/* Content: Test file. */
/* Description: Test with Lungs images. */

#include "AdjacencyRound.hpp"
#include "Color.hpp"
#include "ColorRGB.hpp"
#include "DrawBox.hpp"
#include "FileImage.hpp"
#include "GradientSobel.hpp"
#include "GrowingBucketQueue.hpp"
#include "Image.hpp"
#include "ImageIFT.hpp"
#include "ImageMerge.hpp"
#include "ImageResize.hpp"
#include "Integral.hpp"
#include "IntensityGlobals.hpp"
#include "SumPathFunction.hpp"

using namespace std;
using namespace Bial;

/* Tamanho do lado do quadrado.*/
size_t GetPosWindowMaxSum( Image< int > img, size_t window_x_size, size_t window_y_size ) {
  try {
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
  if( argc != 3 ) {
    cout << "Usage: " << argv[ 0 ] << " <input dir> <output file>" << endl;
    return( 0 );
  }
  try {
    Vector< Image< int > > img( ReadDir< int >( argv[ 1 ] ) );

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
    }
    COMMENT( "Getting maximum intensity position with given window size.", 0 );
    size_t win_xsize = static_cast< size_t >( diff.size( 0 ) * 0.35 + 0.5 );
    size_t win_ysize = win_xsize / 2;
    size_t pxl = GetPosWindowMaxSum( diff, win_xsize, win_ysize ) + 1;
    size_t col = pxl % diff.size( 0 );
    size_t row = pxl / diff.size( 0 );

    Box heart_box( { col - win_xsize / 2.0f, row - win_ysize / 2.0f },
                   { static_cast< float >( win_xsize ), static_cast< float >( win_ysize ) },
                   Color( 180, 255, 255, 0 ), false );


    COMMENT( "Computing merged image.", 0 );
    Image< int > scn( ImageOp::Merge( img ) );
    COMMENT( "Computing slice image. scn dims: " << scn.Dim( ), 0 );
    // BUG here. Must remove first dimension.
    Image< int > sgt_rot( ImageOp::Resize( scn, { col + win_xsize / 4, 0, 0 },
                                           { col + win_xsize / 4, scn.size( 1 ) - 1, scn.size( 2 ) - 1 } ) );
    COMMENT( "Computing subimage. sgt_rot dims: " << sgt_rot.Dim( ), 0 );
    Image< int > sgt( sgt_rot.size( 2 ), sgt_rot.size( 1 ) );
    for( size_t x = 0; x < sgt_rot.size( 2 ); ++x ) {
      for( size_t y = 0; y < sgt_rot.size( 1 ); ++y ) {
        sgt( x, y ) = sgt_rot( 0, y, x );
      }
    }
    COMMENT( "Writing sagittal.", 0 );
    Write( sgt, "dat/sagittal.pgm" );

    COMMENT( "Computing sobel.", 0 );
    Image< int > grad( sgt );
    /* Image< int > grad( Gradient::DirectionalSobel( sgt, 1 ) ); */
    Gradient::Sobel( sgt, &grad );

    COMMENT( "Writing grad.", 0 );
    Write( grad, "dat/sgt_grad.pgm" );

    Image< int > cgrad( grad );
    Intensity::Complement( cgrad );
    int max = cgrad.Maximum( );
    for( size_t x = 0; x < sgt.size( 0 ); ++x ) {
      for( size_t y = 0; y < sgt.size( 1 ); ++y ) {
        if( ( y <= row + win_ysize / 2 ) || ( y > sgt.size( 1 ) - 10 ) ) {
          cgrad( x, y ) = max;
        }
      }
    }
    COMMENT( "Writing grad complement.", 0 );
    Write( cgrad, "dat/sgt_cgrad.pgm" );

    COMMENT( "Creating seeds.", 0 );
    Vector< bool > seeds( grad.size( ), false );
    for( size_t y = 0; y < sgt.size( 1 ); ++y ) {
      if( ( y > row + win_ysize / 2 ) && ( y <= sgt.size( 1 ) - 10 ) ) {
        seeds( y * grad.size( 0 ) ) = true;
      }
    }
    Image< int > value( cgrad );
    Image< int > pred( value );
    Adjacency adj( AdjacencyType::Circular( 1.9 ) );
    SumPathFunction< Image, int > pf( value, nullptr, &pred, false, value );
    size_t size = value.size( );
    GrowingBucketQueue queue( size, 1, true, true );
    for( size_t elm = 0; elm < size; ++elm ) {
      if( seeds[ elm ] != 0 ) {
        value[ elm ] += 1;
        queue.Insert( elm, value[ elm ] );
      }
      else
        value[ elm ] = std::numeric_limits< int >::max( );
    }
    COMMENT( "Creating and running IFT. Value dims: " << value.Dim( ), 0 );
    ImageIFT< int > ift( value, adj, &pf, &queue );
    ift.Run( );
    COMMENT( "Finished IFT.", 0 );
    Write( value, "dat/ift_value.pgm" );
    Write( pred, "dat/ift_pred.pgm" );
    COMMENT( "Computing min_y.", 0 );
    size_t min_y = row + win_ysize / 2 + 1;
    for( size_t y = row + win_ysize / 2 + 2; y <= sgt.size( 1 ) - 10; ++y ) {
      if( value( value.size( 0 ) - 1, min_y ) > value( value.size( 0 ) - 1, y ) )
        min_y = y;
    }
    COMMENT( "min_y: " << min_y, 0 );
    size_t min_pxl = min_y * value.size( 0 ) + value.size( 0 ) - 1;
    COMMENT( "min_pxl: " << min_pxl, 0 );
    Image< int > curve( grad.Dim( ) );
    int prd = min_pxl;
    long long sum_y = 0;
    do {
      sum_y += prd / value.size( 0 );
      curve[ prd ] = 1;
      prd = pred[ prd ];
    } while( prd >= 0 );
    sum_y = ( sum_y + 0.5 ) / value.size( 0 );
    Write( curve, "dat/curve.pgm" );
    COMMENT( "Creating res.", 0 );
    Vector< Image< Color > > res;
    prd = min_pxl;
    for( size_t slc = 0; slc < img.size( ); ++slc ) {
      COMMENT( "Setting image.", 0 );
      res.push_back( ColorSpace::GraytoARGB( img[ slc ] ) );
      res[ slc ].SetRange( Color( 255, 0, 0, 0 ), Color( 255, 255, 255, 255 ) );
      COMMENT( "Drawing square.", 0 );
      float dy = sum_y - prd / ( float ) value.size( 0 );
      heart_box.Draw( res[ slc ], { 0.0f, dy } );
      prd = pred[ prd ];
      Write( res[ slc ], argv[ 2 ] + std::to_string( slc ) + ".ppm" );
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
