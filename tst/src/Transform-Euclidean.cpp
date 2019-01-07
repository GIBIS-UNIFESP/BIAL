/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2016/Feb/01 */
/* Content: Euclidean distance transform application. */
/* Description: Input must be a binary image. */

#include "AdjacencyRound.hpp"
#include "FileImage.hpp"
#include "Image.hpp"
#include "SegmentationBorder.hpp"
#include "TransformEuclDist.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc < 3 ) {
    cout << "Usage: " << argv[ 0 ] << " <binary image> <output image>" << endl;
    return( 0 );
  }

  Image< int > scn( Read< int >( argv[ 1 ] ) );
  Image< int > bdr( Segmentation::BorderPixels( scn, AdjacencyType::HyperSpheric( 1.1, scn.Dims( ) ) ) );
  Image< int > res( Transform::EDT( bdr ) );

  Write( res, argv[ 2 ], argv[ 1 ] );
  return( 0 );
}
