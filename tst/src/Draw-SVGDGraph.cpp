/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Author: Fábio Augusto Menocci Cappabianco */
/* Date: 2015/Aug/30 */
/* Version: 1.0.00 */
/* Content: Test file. */
/* Description: Test with directed graph writing into SVG file. */

#include "AdjacencyRound.hpp"
#include "DrawSVGGraph.hpp"
#include "FileImage.hpp"
#include "Image.hpp"

using namespace Bial;
using namespace std;

int main( ) {
  Image< > img( Read< int >( "res/100x100.pgm.gz" ) );
  Vector< bool > seed( img.size( ) );
  Image< > label( img.Dim( ) );
  label( 100 ) = 255;
  label( 200 ) = 255 << 8;
  label( 210 ) = 255 << 16;
  seed( 100 ) = true;
  seed( 200 ) = true;
  Adjacency adj = AdjacencyType::Circular( );
  string filename( "dat/dgraph.svg" );
  SVGDGraph( img, seed, label, adj, filename );
  return( 0 );
}
