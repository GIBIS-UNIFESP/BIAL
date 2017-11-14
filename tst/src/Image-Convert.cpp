/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2013/Oct/28 */
/* Description: Test image conversion. */

#include "FileImage.hpp"
#include "Image.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc != 3 ) {
    cout << "Usage: " << argv[ 0 ] << " <input image> <output image>" << endl;
    exit( 1 );
  }
  {
    Image< float > img( Read< float >( argv[ 1 ] ) );
    for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {
      if( abs( img[ pxl ] - round( img[ pxl ] ) ) > 0.00001 ) {
        Write( img, argv[ 2 ] );
        return( 0 );
      }
    }
  }

  Image< int > img( Read< int >( argv[ 1 ] ) );

  Write( img, argv[ 2 ], argv[ 1 ] );

  return( 0 );
}
