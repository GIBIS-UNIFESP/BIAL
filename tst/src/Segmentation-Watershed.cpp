/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2012/Sep/21 */
/* Content: Test file. */
/* Description: Test with Segmentation class. */

#include "FileImage.hpp"
#include "Image.hpp"
#include "SegmentationWatershed.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc != 4 ) {
    cout << "Usage: " << argv[ 0 ] << " <input image> <radius> <output image>" << endl;
    return( 0 );
  }

  Image< int > gradient( Read< int >( argv[ 1 ] ) );
  float radius = atof( argv[ 2 ] );
  Image< int > label = Segmentation::Watershed( gradient, radius );
  Write( label, argv[ 3 ], argv[ 1 ] );

  return( 0 );
}
