/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Author: Fábio Augusto Menocci Cappabianco */
/* Date: 2012/Sep/21 */
/* Version: 1.0.00 */
/* Content: Test file. */
/* Description: Test with Image class. */

#include "FileImage.hpp"
#include "Image.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc < 4 ) {
    cout << "Usage: " << argv[ 0 ] << " <input image> <binary mask image> <output image>" << endl;
    exit( -1 );
  }
  Image< Color > img( Read< Color >( argv[ 1 ] ) );
  Image< int > mask( Read< int >( argv[ 2 ] ) );
  if( img.Dims( ) != mask.Dims( ) ) {
    cout << "Error: Input images must have the same dimensions." << endl;
    return( 0 );
  }

  for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {
    if( mask[ pxl ] == 0 ) {
      for( size_t ch = 1; ch < 4; ++ch ) {
        img[ pxl ][ ch ] = 0;
      }
    }
  }

  Write( img, argv[ 3 ], argv[ 1 ] );
  return( 0 );
}
