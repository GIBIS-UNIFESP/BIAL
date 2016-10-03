/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2012/Sep/21 */
/* Content: Test file. */
/* Description: Test with IFT class. */

#include "FileImage.hpp"
#include "Image.hpp"
#include "SegmentationWatershed.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc != 3 ) {
    cout << "Usage: " << argv[ 0 ] << " <input image> <output image>" << endl;
    return( 0 );
  }
  Image< int > gradient( Read< int >( argv[ 1 ] ) );
  Image< int > label( Segmentation::Watershed( gradient ) );

  Write( label, argv[ 2 ], argv[ 1 ] );

  return( 0 );
}
