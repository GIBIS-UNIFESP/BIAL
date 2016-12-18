/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2012/Sep/21 */
/* Content: Test file. */
/* Description: Test with Morphology class. */

#include "AdjacencyRound.hpp"
#include "FileImage.hpp"
#include "GradientMorphological.hpp"
#include "Image.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc != 3 ) {
    cout << "Usage: " << argv[ 0 ] << " <input image> <output image>" << endl;
    exit( 1 );
  }
  Image< int > img( Read< int >( argv[ 1 ] ) );

  Adjacency spheric( AdjacencyType::HyperSpheric( 1.0, img.Dims( ) ) );
  Image< int > gradient( Image< int >( Gradient::Morphological( img, spheric ) ) );

  Write( gradient, argv[ 2 ], argv[ 1 ] );

  return( 0 );
}
