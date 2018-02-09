/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2013/Sep/29 */
/* Content: Test file. */
/* Description: Test PNM image basic operations. */

#include "Color.hpp"
#include "FileImage.hpp"
#include "Image.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char *argv[] ) {
  if( argc != 4 ) {
    cout << "Usage: " << argv[ 0 ] << "<image 1> <image 2> <resulting image>" << endl;
    return( 0 );
  }
  Image< Color > img1( Read< Color >( argv[ 1 ] ) );
  Image< Color > img2( Read< Color >( argv[ 2 ] ) );

  Image< Color > res( ( img1 / 2 ) + ( img2 / 2 ) );

  Write( res, argv[ 3 ] );

  return( 0 );
}
