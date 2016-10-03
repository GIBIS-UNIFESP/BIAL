/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2016/Sep/15 */
/* Description: Test image interpolation. */

#include "FileImage.hpp"
#include "Image.hpp"
#include "ImageInterpolation.hpp"
#include "PixelInterpolation.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc != 4 ) {
    cout << "Usage: " << argv[ 0 ] << " <input image> <output image> <pixel size>" << endl;
    exit( 1 );
  }
  
  Image< int > img( Read< int >( argv[ 1 ] ) );
  float pixel_size = atof( argv[ 3 ] );
  if( pixel_size < 0.01 ) {
    cout << "Error: Pixel size must be greater than 0." << endl;
    return( 0 );
  }
  LinearInterpolation interp;

  if( img.Dims( ) == 2 ) {
    Image< int > res( ImageInterpolation::Interpolation( img, pixel_size, pixel_size, interp ) );
    Write( res, argv[ 2 ], argv[ 1 ] );
  }
  else {
    Image< int > res( ImageInterpolation::Interpolation( img, pixel_size, pixel_size, pixel_size, interp ) );
    Write( res, argv[ 2 ], argv[ 1 ] );
  }

  return( 0 );
}
