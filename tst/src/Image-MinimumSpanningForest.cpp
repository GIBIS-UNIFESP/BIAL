/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2012/Sep/21 */
/* Content: Test file. */
/* Description: Test with Segmentation class. */

#include "Feature.hpp"
#include "FileImage.hpp"
#include "Image.hpp"
#include "SegmentationMST.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc != 4 ) {
    cout << "Usage: " << argv[ 0 ] << " <input image> <classes> <output image>" << endl;
    return( 0 );
  }
  Image< int > img( Read< int >( argv[ 1 ] ) );
  size_t classes = atoi( argv[ 2 ] );
  Feature< int > feature( img.size( ), 1 );
  for( size_t elm = 0; elm < feature.Elements( ); ++elm ) {
    feature( elm, 0 ) = img[ elm ];
  }
  Image< int > label( Segmentation::MinimumSpanningForest( img, feature, classes ) );

  Write( label, argv[ 3 ], argv[ 1 ] );

  return( 0 );
}
