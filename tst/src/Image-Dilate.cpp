/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2012/Sep/21 */
/* Content: Test file. */
/* Description: Test with Image and Morphology classes. */

#include "AdjacencyRound.hpp"
#include "FileImage.hpp"
#include "Image.hpp"
#include "MorphologyDilation.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc != 4 ) {
    cout << "usage: " << argv[ 0 ] << " <input> <radius> <output>" << endl;
    cout << "input: .............. original binary image" << endl;
    cout << "radius: ............. dilation mask radius" << endl;
    cout << "output: ............. dilated binary image" << endl;
    return( 0 );
  }
  Image< int > img( Read< int >( argv[ 1 ] ) );
  float radius = atof( argv[ 2 ] );
  Adjacency adj( AdjacencyType::HyperSpheric( radius, img.Dims( ) ) );
  Image< int > res( Morphology::Dilate( img, adj ) );

  Write( res, argv[ 3 ], argv[ 1 ] );

  return( 0 );
}
