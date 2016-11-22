/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Author: Fábio Augusto Menocci Cappabianco */
/* Date: 2012/Jun/27 */
/* Version: 1.0.00 */
/* Content: Test file. */
/* Description: Test with Matrix class. */

#include "Matrix.hpp"

using namespace std;
using namespace Bial;

int main( int, char** ) {
  COMMENT( "Creating matrices.", 0 );
  Matrix< double > M1( 10, 8 );
  Matrix< double > M2( 2, 3 );
  Matrix< double > M3( 1, 2 );

  M2 = M1;
  for( size_t i = 0; i < M1.size( 1 ); ++i ) { /* static_cast< int > == ( int ) */
    for( size_t j = 0; j < M1.size( 0 ); ++j ) {
      M1[ i * M1.size( 0 ) + j ] = i + j;
      M2[ i * M2.size( 0 ) + j ] = i * j;
    }
  }
  cout << "M1." << endl << M1 << endl;
  cout << "M2." << endl << M2 << endl;
  M3 = M1 + M2;
  cout << "M3 - Sum of M1 and M2." << endl << M3 << endl;
  M1 += M2;
  cout << "M1 - In place sum of M1 and M2." << endl << M1 << endl;
  /* M1 = M2( "1:7, 3:10" ); */

  /*
   * cout << "M1 - Sub matrix:." << endl;
   * cout << M1 << endl;
   */

  return( 0 );
}
