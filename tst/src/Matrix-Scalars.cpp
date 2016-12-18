/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2012/Jun/28 */
/* Content: Test file. */
/* Description: Test with Matrix class. */

#include "Common.hpp"
#include "Matrix.hpp"

using namespace std;
using namespace Bial;

int main( int, char** ) {

  Common::Randomize( false );

  Matrix< double > M1( 10, 10 );
  Matrix< double > M2( 2, 10 );
  double scalar;

  rand( );
  for( size_t i = 0; i < M1.size( 1 ); ++i ) {
    for( size_t j = 0; j < M1.size( 0 ); ++j ) {
      M1.at( i, j ) = -22 + ( 100. * rand( ) ) / RAND_MAX;
    }
  }
  scalar = 10.0;
  cout << "M1." << endl << M1 << endl;
  M2 = M1 * scalar;
  cout << "M2 = M1 * 10.0." << endl << M2 << endl;
  M1 *= scalar;
  cout << "M1 *= 10.0." << endl << M1 << endl;
  M2( 0 ) = -100.1;
  M2( 0, 3 ) = M2( 0 );
  M2( 1 ) = M2( 0, 3 );
  cout << "M2 modified by operator () in indexes ( 0 ), ( 1 ), and ( 0, 3 )." << endl << M2 << endl;
  return( 0 );
}
