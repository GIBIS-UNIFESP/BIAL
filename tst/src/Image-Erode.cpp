/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Author: Fábio Augusto Menocci Cappabianco */
/* Date: 2016/Feb/04 */
/* Version: 1.0.00 */
/* Content: Erodes an image with given radius. */
/* Description: */

#include "AdjacencyRound.hpp"
#include "FileImage.hpp"
#include "Image.hpp"
#include "MorphologyErosion.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc != 4 ) {
    cout << "usage: " << argv[ 0 ] << " <input> <radius> <output>" << endl;
    cout << "input: .............. original binary image" << endl;
    cout << "radius: ............. erosion mask radius" << endl;
    cout << "output: ............. eroded binary image" << endl;
    return( 0 );
  }
  Image< int > img( Read< int >( argv[ 1 ] ) );
  float radius = atof( argv[ 2 ] );
  Adjacency adj( AdjacencyType::HyperSpheric( radius, img.Dims( ) ) );
  Image< int > res( Morphology::Erode( img, adj ) );

  Write( res, argv[ 3 ], argv[ 1 ] );

  return( 0 );
}
