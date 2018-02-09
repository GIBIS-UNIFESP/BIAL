/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2013/Nov/01 */
/* Content: Test file. */
/* Description: Segmentation of background of images for the optimized anisotropic filter. */

#include "Common.hpp"
#include "FileImage.hpp"
#include "Image.hpp"
#include "SegmentationBackground.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc != 3 ) {
    cout << "Usage: " << argv[ 0 ] << " <input image> <output image>" << endl;
    return( 0 );
  }
  /* Fixed number sequence */
  Common::Randomize( false );

  /* Reading input data. */
  Image< int > img( Read< int >( argv[ 1 ] ) );

  /* Segmenting background */
  Image< int > dark( Segmentation::Background( img ) );

  /* Writing results. */
  Write( dark, argv[ 2 ] );

  return( 0 );
}
