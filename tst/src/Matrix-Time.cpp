/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2014/Oct/14 */
/* Content: Test file. */
/* Description: Test with Matrix class. */

#include "Matrix.hpp"

using namespace std;
using namespace Bial;

int main( int, char** ) {
  Matrix< double > M1( 40000, 40000 );
  Matrix< double > M2( 40000, 40000 );
  for( size_t i = 0; i < M1.size( 1 ); ++i ) { /* static_cast< int > == ( int ) */
    for( size_t j = 0; j < M1.size( 0 ); ++j ) {
      M1[ i * M1.size( 0 ) + j ] = i + j;
      M2[ i * M1.size( 0 ) + j ] = i - j;
    }
  }
  double total = 0.0;
  for( size_t p = 0; p < M1.size( ); ++p ) {
    total += M1[ p ] - M2[ p ];
  }
  cout << total << endl;

  return( 0 );
}
