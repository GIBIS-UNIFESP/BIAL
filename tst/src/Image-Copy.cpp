/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2015/Apr/16 */
/* Content: Test file. */
/* Description: Test . */

#include "FileImage.hpp"
#include "Image.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc != 3 ) {
    cout << "Usage: " << argv[ 0 ] << " <input image> <output image>" << endl;
    exit( 1 );
  }
  Image< int > img;
  COMMENT( "Assigning img.", 0 );
  img = Read< int >( argv[ 1 ] );

  Write( img, argv[ 2 ], argv[ 1 ] );

  return( 0 );
}
