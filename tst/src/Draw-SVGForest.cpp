/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2015/Aug/30 */
/* Content: Test file. */
/* Description: Test with directed graph writing into SVG file. */

#include "AdjacencyRound.hpp"
#include "DrawSVGGraph.hpp"
#include "EdgeRiverBed.hpp"
#include "FileImage.hpp"
#include "Image.hpp"

using namespace Bial;
using namespace std;

int main( ) {
  Image< > img( Read< int >( "res/river.pgm" ) );
  Vector< bool > seed( img.size( ) );
  Image< > label( img.Dim( ) );
  Image< > pred( img.Dim( ) );
  Image< > res( img.Dim( ) );
  seed( 0 ) = true;
  seed( 12 ) = true;
  seed( 24 ) = true;
  seed( 36 ) = true;
  label( 0 ) = 256;
  label( 12 ) = 256 << 8;
  label( 24 ) = 256 << 16;
  label( 36 ) = 256 + ( 256 << 8 );

  string filename( "dat/init_graph.svg" );
  Adjacency adj( AdjacencyType::Circular( 1.1f ) );
  SVGDGraph( img, seed, label, adj, filename );

  tie( res, pred ) = Edge::RiverBed( img, seed );

  filename = string( "dat/forest.svg" );
  SVGDForest( res, pred, label, filename );
  return( 0 );
}
