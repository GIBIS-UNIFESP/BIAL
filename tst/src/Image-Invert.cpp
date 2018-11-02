/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2018/Nov/02 */
/* Content: Test file. */
/* Description: Test with Image class. */

#include "FileImage.hpp"
#include "Image.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc < 3 ) {
    cout << "Usage: " << argv[ 0 ] << " <input image> <output image>" << endl;
    exit( -1 );
  }
  Image< int > img1( Read< int >( argv[ 1 ] ) );
  Image< int > res( img1 );
  int min = img1.Minimum( );
  int max = img1.Maximum( );
  for( size_t pxl = 0; pxl < img1.size( ); ++pxl )
    res[ pxl ] = max - res[ pxl ] - min;
  Write( res, argv[ 2 ], argv[ 1 ] );
  return( 0 );
}
