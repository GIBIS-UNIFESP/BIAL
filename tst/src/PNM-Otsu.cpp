/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2014/Oct/23 */
/* Content: Test file. */
/* Description: Test with Color class. */

#include "Color.hpp"
#include "ColorChannel.hpp"
#include "FileImage.hpp"
#include "Image.hpp"
#include "SegmentationOtsuThreshold.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc != 3 ) {
    cout << "Usage: " << argv[ 0 ] << " <input file name> <output file name> " << endl;
    exit( 0 );
  }
  Image< Color > img( Read< Color >( argv[ 1 ] ) );
  Vector< Image< int > > channel( ColorSpace::Split< int >( img ) );
  
  for( size_t chl = 1; chl < 4; ++chl )
    channel( chl ) = Segmentation::Otsu( channel[ chl ] );
  Image< Color > res( ColorSpace::Merge( channel ) );
  Write( res, argv[ 2 ] );
  return( 0 );
}
