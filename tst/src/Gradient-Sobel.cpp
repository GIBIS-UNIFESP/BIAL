/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2012/Jun/27 */
/* Content: Test file. */
/* Description: Test with Filteirng class. */

#include "FileImage.hpp"
#include "GradientSobel.hpp"
#include "Image.hpp"
#include "Matrix.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( ( argc < 3 ) || ( argc > 4 ) ) {
    cout << "Usage: " << argv[ 0 ] << " <input image> <output magnitude> [<output direction>]" << endl;
    return( 0 );
  }
  /* reading parameters */
  Image< int > img( Read< int >( argv[ 1 ] ) );

  Image< int > mag( img );
  if( argc == 4 ) {
    Image< int > dir( img );
    Gradient::Sobel( img, &mag, &dir );

    Write( mag, argv[ 2 ], argv[ 1 ] );
    Write( dir, argv[ 3 ], argv[ 1 ] );
  }
  else {
    Gradient::Sobel( img, &mag, nullptr );
    Write( mag, argv[ 2 ], argv[ 1 ] );
  }

  return( 0 );
}
