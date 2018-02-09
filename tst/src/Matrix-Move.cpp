/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2012/Jun/29 */
/* Content: Test file. */
/* Description: Test with Matrix class. */

#include "Matrix.hpp"

using namespace std;
using namespace Bial;

int main( int, char** ) {

  Matrix< double > M1( 2, 3, 4 );
  M1.Set( 10.34 );
  M1[ 0 ] = 100.2;
  cout << "M1." << endl << M1 << endl;
  Matrix< double > M2 = move( M1 );
  cout << "M2." << endl << M2 << endl;
  /*
   * try {
   *   cout << "M1." << endl << M1 << endl;
   * }
   * catch( logic_error &e ) {
   *   cout << e.what( );
   * }
   */
  return( 0 );
}
