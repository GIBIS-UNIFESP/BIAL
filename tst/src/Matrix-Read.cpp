/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2015/Jun/09 */
/* Content: Test file. */
/* Description: Test with Matrix class to read from file. */

#include "FileImage.hpp"
#include "Matrix.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc != 2 ) {
    cout << "Usage: " << argv[ 0 ] << "<input bmat file>" << endl;
    return( 0 );
  }
  Matrix< double > M1( ReadMatrix< double >( argv[ 1 ] ) );

  cout << M1 << endl;
  return( 0 );
}
