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
  if( argc < 4 ) {
    cout << "Creates a binary mask, setting non-zero values to 1." << endl;
    cout << "Usage: " << argv[ 0 ] << " <input image> <scalar> <output image>" << endl;
    exit( -1 );
  }
  Image< int > scn( Read< int >( argv[ 1 ] ) );
  int scalar = atoi( argv[ 2 ] );
  scn = scn + scalar;
  Write( scn, argv[ 3 ] );
  return( 0 );
}
