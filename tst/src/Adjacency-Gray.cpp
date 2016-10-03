/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Author: Fábio Augusto Menocci Cappabianco */
/* Date: 2012/Jun/27 */
/* Version: 1.0.00 */
/* Content: Tests Gray adjacency. */
/* Description: Test with Adjacency class. */

#include "AdjacencyGrayCode.hpp"

using namespace std;
using namespace Bial;

int main( int, char** ) {
  Adjacency adj = AdjacencyType::Gray8( );
  for( size_t idx = 0; idx < adj.size( ); ++idx ) {
    cout << "idx: " << idx;
    for( size_t dms = 0; dms < adj.Dims( ); ++dms ) {
      cout << ", d" << dms << ": " << adj.Displacement( dms, idx );
    }
    cout << endl;
  }
  return( 0 );
}
