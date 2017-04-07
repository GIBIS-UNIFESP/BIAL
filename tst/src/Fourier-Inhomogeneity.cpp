/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Author: Fábio Augusto Menocci Cappabianco */
/* Date: 2017/Mar/28 */
/* Version: 1.0.00 */
/* Content: Tests fourier transform. */
/* Description: Test with Bit class. */

#include "Adjacency.hpp"
#include "AdjacencyIterator.hpp"
#include "AdjacencyRound.hpp"
#include "Fourier.hpp"
#include "FileImage.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char *argv[ ] ) {

  if( argc != 5 ) {
    cout << "Usage: " << argv[ 0 ] << " <input image> <output image> <min_radius> <max_radius>" << endl;
    return( 0 );
  }

  COMMENT( "Reading data.", 0 );
  Image< int > img( Read< int >( argv[ 1 ] ) );
  float min_rad = atof( argv[ 3 ] );
  float max_rad = atof( argv[ 4 ] );
  COMMENT( "Running FFT.", 0 );
  Image< bial_complex > c_img( img );
  for( size_t z = 0; z < c_img.size( 2 ); ++z ) {
    for( size_t y = 0; y < c_img.size( 1 ); ++y ) {
      for( size_t x = 0; x < c_img.size( 0 ); ++x ) {
        int pot = static_cast< int >( ( x + y + z ) % 2 ) * -2 + 1;
        c_img( x, y, z ).real( c_img( x, y, z ).real( ) * pot );
      }
    }
  }
  Image< bial_complex > f_img( FFT( c_img ) );
  COMMENT( "Filtering inhomogeneity.", 0 );
  Adjacency adj( AdjacencyType::HyperSpheric( max_rad, img.Dims( ) ) );
  Vector< size_t > src_pxl( 3, f_img.size( 2 ) / 2 );
  src_pxl[ 0 ] = f_img.size( 0 ) / 2;
  src_pxl[ 1 ] = f_img.size( 1 ) / 2;
  Vector< size_t > tgt_pxl( 3 );
  AdjacencyIterator itr( img, adj );
  for( size_t elm = 0; elm < adj.size( ); ++elm ) {
    if( ( adj.Displacement( elm ) >= min_rad ) && ( itr.AdjVct( src_pxl, elm, tgt_pxl ) ) )
      f_img( tgt_pxl ) = 0.0;
  }
  Image< llint > abs_img( f_img.Dim( ) );
  for( size_t pxl = 0; pxl < abs_img.size( ); ++pxl )
    abs_img[ pxl ] = static_cast< llint >( std::abs( f_img[ pxl ] ) );
  Write( abs_img * ( 255.0 / abs_img.Maximum( ) ), "abs_img.pgm" );
  COMMENT( "Running IFFT.", 0 );
  Image< bial_complex > r_img( FFT( f_img, false ) );
  Image< int > res_pad( r_img );
  Image< int > res( img.Dim( ) );
  ZeroUnPad( res_pad, res );
  for( size_t z = 0; z < c_img.size( 2 ); ++z ) {
    for( size_t y = 0; y < c_img.size( 1 ); ++y ) {
      for( size_t x = 0; x < c_img.size( 0 ); ++x ) {
        int pot = static_cast< int >( ( x + y + z ) % 2 ) * -2 + 1;
        res( x, y, z ) *= pot;
      }
    }
  }
  Write( res, argv[ 2 ], argv[ 1 ] );
  
  return( 0 );
}
