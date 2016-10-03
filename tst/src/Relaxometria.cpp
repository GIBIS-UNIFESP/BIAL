/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Author: Fábio Augusto Menocci Cappabianco */
/* Date: 2015/May/21 */
/* Version: 1.0.00 */
/* Content: Test file. */
/* Description: Implementation of multiclass labeling from segmentation result. */

#include "FileImage.hpp"
#include "Image.hpp"

using namespace Bial;
using namespace std;

int main( int argc, char *argv[] ) {
  if( argc != 4 ) {
    cout << "usage: " << argv[ 0 ] << " <input image> <image number> <output image>" << endl;
    cout << "\t\t<image number>: From 0 to 35." << endl;
    return( 0 );
  }
  COMMENT( "Read image from input file.", 0 );
  Image< int > img( Read< int >( argv[ 1 ] ) );
  size_t img_num = atoi( argv[ 2 ] );

  COMMENT( "Select the slices that contain a single sample of the data.", 0 );
  Image< int > res( Vector< size_t >( { img.size( 0 ), img.size( 1 ), 1296 / 36 } ),
                    { img.PixelSize( 0 ), img.PixelSize( 1 ), 3.0f } );
  for( size_t z = 0; z < res.size( 2 ); ++z ) {
    for( size_t y = 0; y < img.size( 1 ); ++y ) {
      for( size_t x = 0; x < img.size( 0 ); ++x ) {
        res( x, y, z ) = img( x, y, z * 36 + img_num );
      }
    }
  }
  COMMENT( "Write the result to output file.", 0 );
  Write( res, argv[ 3 ] );

  return( 0 );
}
