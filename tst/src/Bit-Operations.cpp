/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2015/Apr/10 */
/* Content: Tests bit operations. */
/* Description: Test with Bit class. */

#include "Bit.hpp"

using namespace std;
using namespace Bial;

int main( ) {
  int num1 = 10, neg1a, neg1b;
  int set1a, set1b;
  int reset1a, reset1b;

  neg1a = Bit::Invert( num1, 1 );
  neg1b = Bit::Invert( num1, 0 );

  cout << "Init val: " << num1 << endl;
  cout << "invert 1: " << neg1a << endl;
  cout << "invert 0: " << neg1b << endl;

  set1a = Bit::Preset( num1, 1 );
  set1b = Bit::Preset( num1, 0 );

  cout << "set 1: " << set1a << endl;
  cout << "set 0: " << set1b << endl;

  reset1a = Bit::Reset( num1, 1 );
  reset1b = Bit::Reset( num1, 0 );

  cout << "reset 1: " << reset1a << endl;
  cout << "reset 0: " << reset1b << endl;

  cout << "Value 1: " <<
  Bit::Value( num1, 1 ) << endl;
  cout << "Value 0: " <<
  Bit::Value( num1, 0 ) << endl;

  cout << "IsOne 1: " <<
  Bit::IsOne( num1, 1 ) << endl;
  cout << "IsOne 0: " <<
  Bit::IsOne( num1, 0 ) << endl;

  cout << "IsZero 1: " <<
  Bit::IsZero( num1, 1 ) << endl;
  cout << "IsZero 0: " <<
  Bit::IsZero( num1, 0 ) << endl;

  return( 0 );
}
