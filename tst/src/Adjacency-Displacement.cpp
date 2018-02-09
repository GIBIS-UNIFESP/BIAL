/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2012/Jun/27 */
/* Content: Test file. */
/* Description: Test with Adjacency class. */

#include "AdjacencyRound.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc != 3 ) {
    cout << "Usage: " << argv[ 0 ] << " <adjacency dimensions> <adjacency radius>" << endl;
    return( 0 );
  }
  size_t dimensions = atoi( argv[ 1 ] );
  float radius = atof( argv[ 2 ] );
  Adjacency adj( AdjacencyType::HyperSpheric( radius, dimensions ) );
  for( size_t idx = 0; idx < adj.size( ); ++idx ) {
    cout << "idx: " << idx;
    for( size_t dms = 0; dms < adj.Dims( ); ++dms )
      cout << ", d" << dms << ": " << adj( idx, dms );
    cout << endl;
  }
  return( 0 );
}
