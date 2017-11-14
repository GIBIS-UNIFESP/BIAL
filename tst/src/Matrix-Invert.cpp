/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2012/Jun/27 */
/* Content: Test file. */
/* Description: Test with Matrix class. */

#include "Common.hpp"
#include "Matrix.hpp"
#include "MatrixIdentity.hpp"
#include "MatrixInverse.hpp"

using namespace std;
using namespace Bial;

int main( int, char** ) {

  Common::Randomize( false );

  Matrix< double > M1( 10, 10 );
  M1[ 5 ] = 1.4;
  M1[ 6 ] = M1.at( 5 );
  M1.at( 7, 0 ) = M1.at( 5, 0 );
  M1.at( 7, 5 ) = M1.at( 7, 0 );
  Matrix< double > M2( M1 );
  Matrix< double > M3( 1, 15 );
  double diff;

  rand( );
  for( size_t i = 0; i < M1.size( 1 ); ++i ) {
    for( size_t j = 0; j < M1.size( 0 ); ++j ) {
      M1.at( i, j ) = -22 + ( 100. * rand( ) ) / RAND_MAX;
    }
  }
  cout << "M1." << endl;
  M1.Print( cout );
  cout << "M2 - Initial values of M1 copied to M2." << endl;
  M2.Print( cout );
  M2 = M1;
  cout << "M2 - just a copy of M1." << endl;
  M2.Print( cout );
  MatrixOp::Inverse( M1 );
  cout << "M1 - inverse of original." << endl;
  M1.Print( cout );
  M3 = M1 * M2;
  cout << "M3 - identity." << endl;
  M3.Print( cout );
  M1 *= M2;
  cout << "M1 - identity by *= operator." << endl;
  M1.Print( cout );
  diff = MatrixOp::CompareToIdentity( M3 );
  cout << "Max difference from identity: " << diff << endl;

  M1 = Matrix< double >( 3, 3 );
  M1( 0, 0 ) = 0;
  M1( 1, 0 ) = -1;
  M1( 2, 0 ) = 0;
  M1( 0, 1 ) = 0;
  M1( 1, 1 ) = 0;
  M1( 2, 1 ) = -1;
  M1( 0, 2 ) = -1;
  M1( 1, 2 ) = 0;
  M1( 2, 2 ) = 0;

  cout << "3x3 matrix:" << endl;
  M1.Print( cout );
  MatrixOp::Inverse( M1 );
  cout << endl << "3x3 matrix inverse:" << endl;
  M1.Print( cout );

  return( 0 );
}
