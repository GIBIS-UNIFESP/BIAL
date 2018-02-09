/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2016/Feb/01 */
/* Content: Sums the content of two images. */
/* Description: */

#include "FileImage.hpp"
#include "Image.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc < 4 ) {
    cout << "Usage: " << argv[ 0 ] << " <input image 1> <input image 2> <output image>" << endl;
    exit( -1 );
  }
  Image< int > img1( Read< int >( argv[ 1 ] ) );
  Image< int > img2( Read< int >( argv[ 2 ] ) );
  Image< int > res( img1 + img2 );

  Write( res, argv[ 3 ], argv[ 1 ] );
  return( 0 );
}
