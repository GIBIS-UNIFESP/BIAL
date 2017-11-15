/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2015/Aug/30 */
/* Content: Test file. */
/* Description: Test with directed graph writing into SVG file. */

#include "AdjacencyRound.hpp"
#include "DrawSVGGraph.hpp"
#include "FileImage.hpp"
#include "Image.hpp"

using namespace Bial;
using namespace std;

int main( ) {
  COMMENT( "Reading images.", 0 );
  Image< > img( Read< int >( "res/100x100.pgm.gz" ) );
  Vector< bool > seed( img.size( ) );
  Image< > label( img.Dim( ) );
  COMMENT( "Setting labels.", 0 );
  label( 100 ) = 255;
  label( 200 ) = 255 << 8;
  label( 210 ) = 255 << 16;
  seed( 100 ) = true;
  seed( 200 ) = true;
  COMMENT( "Creating adjacency.", 0 );
  Adjacency adj( AdjacencyType::Circular( ) );
  string filename( "dat/dgraph.svg" );
  COMMENT( "Writing graph.", 0 );
  SVGDGraph( img, adj, filename, &seed, &label );
  return( 0 );
}
