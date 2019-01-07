/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2018/Nov/27 */
/* Content: Test file. */
/* Description: Computes potential edge and noise pixels. */

#include "AdjacencyRound.hpp"
#include "AdjacencyIterator.hpp"
#include "FileImage.hpp"
#include "FilteringMedian.hpp"
#include "Filtering-NonLocalAnisotropicBasics.cpp"
#include "Image.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc < 4 ) {
    cout << "Usage: " << argv[ 0 ] << " <input image> <noise output mask> <edge output mask>" << endl;
    exit( -1 );
  }
  Image< int > img( Read< int >( argv[ 1 ] ) );
  Image< int > noise_mask( img.Dim( ), img.PixelSize( ) );
  Image< int > edges_mask( img.Dim( ), img.PixelSize( ) );
  FlatEdgePixels( img, edges_mask, noise_mask );
  Write( noise_mask, argv[ 2 ] );
  Write( edges_mask, argv[ 3 ] );
  return( 0 );
}
