/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Author: Fábio Augusto Menocci Cappabianco */
/* Date: 2015/Jun/09 */
/* Version: 1.0.00 */
/* Content: Test file. */
/* Description: Test with Matrix class for reading from and writing to file. */

#include "FileImage.hpp"
#include "Matrix.hpp"

using namespace std;
using namespace Bial;

int main( int, char** ) {
  Matrix< double > M1( 10, 8 );
  for( size_t i = 0; i < M1.size( 1 ); ++i ) {
    for( size_t j = 0; j < M1.size( 0 ); ++j ) {
      M1[ i * M1.size( 0 ) + j ] = i + j;
    }
  }
  Write( M1, "dat/mat.bmat" );

  Matrix< float > M2( ReadMatrix< float >( "dat/mat.bmat" ) );

  cout << M1 << endl;
  cout << M2 << endl;

  return( 0 );
}
