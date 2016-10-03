/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Author: Fábio Augusto Menocci Cappabianco */
/* Date: 2012/Jun/29 */
/* Version: 1.0.00 */
/* Content: Test file. */
/* Description: Test with Matrix class. */

#include "Matrix.hpp"

using namespace std;
using namespace Bial;

int main( int, char** ) {
  Matrix< double > M1( 5, 5, 5 );
  Matrix< double > M2( 10, 10 );
  double scalar;
  bool equals;

  M1.Set( 10.34 );

  scalar = 10.0;
  cout << "M1." << endl << M1 << endl;
  M2 = M1 * scalar;
  cout << "M2 = M1 * 10.0." << endl << M2 << endl;
  M1 *= scalar;
  cout << "M1 *= 10.0." << endl << M1 << endl;
  equals = M1.Equals( M2, 0.01 );
  cout << "M1 is equal to M2? " << equals << endl;
  M2( 0 ) = -100.1;
  M2( 0, 3, 2 ) = M2( 0 );
  M2( 1 ) = M2( 0, 3, 2 );
  cout << "M2 modified by operator () in indexes ( 0 ), ( 1 ), and ( 0, 3, 2 )." << endl << M2 << endl;
  equals = M1.Equals( M2, 0.01 );
  cout << "M1 is equal to M2? " << equals << endl;
  return( 0 );
}