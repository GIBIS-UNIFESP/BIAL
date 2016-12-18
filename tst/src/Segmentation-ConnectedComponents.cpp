/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Author: Fábio Augusto Menocci Cappabianco */
/* Date: 2012/Sep/21 */
/* Version: 1.0.00 */
/* Content: Test file. */
/* Description: Test with Connected components function. */

#include "AdjacencyRound.hpp"
#include "FileImage.hpp"
#include "Image.hpp"
#include "SegmentationConnectedComponents.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc != 4 ) {
    cout << "Usage: " << argv[ 0 ] << " <input label image> <radius> <output label image>" << endl;
    return( 0 );
  }
  Image< int > source( Read< int >( argv[ 1 ] ) );
  float radius = atof( argv[ 2 ] );
  Adjacency spheric( AdjacencyType::HyperSpheric( radius, source.Dims( ) ) );
  Image< int > label = Segmentation::ConnectedComponents( source, spheric );
  Write( label, argv[ 3 ], argv[ 1 ] );
  return( 0 );
}
