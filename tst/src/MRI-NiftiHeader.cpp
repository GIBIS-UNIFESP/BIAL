/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2012/Jun/27 */
/* Content: Test file. */
/* Description: Testing Nifti header orientation operations. */

#include "Image.hpp"
#include "NiftiHeader.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc < 2 ) {
    cout << "Usage: " << argv[ 0 ] << " <input image>" << endl;
    exit( -1 );
  }
  NiftiHeader hdr( argv[ 1 ] );

  hdr.Print( cout );

  return( 0 );
}
