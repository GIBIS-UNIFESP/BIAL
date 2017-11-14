/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2012/Sep/21 */
/* Content: Test file. */
/* Description: Test with Image class. */

#include "Image.hpp"
#include "ImageEquals.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc < 3 ) {
    cout << "Compares two images. Returns 0 if they are equal, or returns 1 if images have distinct sizes or data."
         << endl;
    cout << "Usage: " << argv[ 0 ] << " <image 1> <image 2>" << endl;
    exit( -1 );
  }
  if( !ImageOp::EqualImages( argv[ 1 ], argv[ 2 ] ) ) {
    cout << 1 << endl;
    return( 0 );
  }
  cout << 0 << endl;
  return( 0 );
}
