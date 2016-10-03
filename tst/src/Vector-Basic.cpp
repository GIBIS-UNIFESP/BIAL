/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Author: Fábio Augusto Menocci Cappabianco */
/* Date: 2015/May/28 */
/* Version: 1.0.00 */
/* Content: Test file. */
/* Description: Test with Vector class. */

#include "Vector.hpp"

using namespace std;
using namespace Bial;

int main( int, char** ) {
  Vector< double > V1( { 1.2, 2.1, 3.6, 6.2, 7.0, 8.87 } );
  Vector< double > V2( 6, 3.0 );
  Vector< double > V3 = { 1.2, 2.1, 3.6, 6.2, 7.0, 8.87 };

  cout << "V1." << endl << V1 << endl;
  cout << "V2." << endl << V2 << endl;
  cout << "V3." << endl << V3 << endl;
  V3 = V1 + V2;
  cout << "V3 - Sum of V1 and V2." << endl << V3 << endl;

  return( 0 );
}