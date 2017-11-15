/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2012/Sep/21 */
/* Content: Test file. */
/* Description: Test with Transform class. */

#include "AdjacencyRound.hpp"
#include "GeometricsScale.hpp"
#include "FileImage.hpp"
#include "Image.hpp"
#include "SegmentationBorder.hpp"
#include "TransformEuclDistInv.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  string orientation;
  if( argc < 2 ) {
    cout << "Usage: " << argv[ 0 ] << " <binary image> <output image>" << endl;
    exit( -1 );
  }
  Image< int > scn( Read< int >( argv[ 1 ] ) );
  Image< int > scn2( Geometrics::Scale( scn, 0.5, true ) );
  Image< int > bdr( Segmentation::BorderPixels( scn2, AdjacencyType::HyperSpheric( 1.1, scn2.Dims( ) ) ) );
  DEBUG_WRITE( bdr, "dist-trans-border", 0 );
  Image< int > res( Geometrics::Scale( Transform::InverseEDT( bdr, scn2, 15 ), 2.0, true ) );
  size_t size = res.size( );
  for( size_t elm = 0; elm < size; ++elm ) {
    if( scn[ elm ] == 0 )
      res[ elm ] = 0;
  }
  Write( res, argv[ 2 ], argv[ 1 ] );
  return( 0 );
}
