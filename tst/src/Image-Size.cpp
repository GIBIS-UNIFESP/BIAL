/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2016/Nov/25 */
/* Content: Prints image dimensions. */
/* Description: */

#include "FileImage.hpp"
#include "Image.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc != 2 ) {
    cout << "Usage: " << argv[ 0 ] << " <input image>" << endl;
    exit( -1 );
  }
  Image< int > img1( Read< int >( argv[ 1 ] ) );

  cout << img1.Dim( );

  return( 0 );
}
