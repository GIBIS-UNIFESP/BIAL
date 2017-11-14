/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2012/Sep/21 */
/* Content: Test file. */
/* Description: Test with Image class. */

#include "FileImage.hpp"
#include "Image.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc < 4 ) {
    cout << "Usage: " << argv[ 0 ] << " <input image 1> <scalar> <output image>" << endl;
    exit( 0 );
  }
  Image< int > img1( Read< int >( argv[ 1 ] ) );
  float scalar = atof( argv[ 2 ] );

  Image< int > res( img1 * scalar );

  Write( res, argv[ 3 ], argv[ 1 ] );
  return( 0 );
}
