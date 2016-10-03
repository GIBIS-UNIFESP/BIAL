/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Author: Fábio Augusto Menocci Cappabianco */
/* Date: 2015/Apr/10 */
/* Version: 1.0.00 */
/* Content: Tests bit inversion operator. */
/* Description: Test with Bit class. */

#include "Bit.hpp"

using namespace std;
using namespace Bial;

int main( ) {
  int num1 = 10, neg1a, neg1b;

  neg1a = Bit::Invert( num1, 1 );
  neg1b = Bit::Invert( num1, 0 );

  cout << "Init val: " << num1 << endl;
  cout << "invert 1: " << neg1a << endl;
  cout << "invert 0: " << neg1b << endl;

  return( 0 );
}
