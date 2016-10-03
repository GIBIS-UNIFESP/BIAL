/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Author: Fábio Augusto Menocci Cappabianco */
/* Date: 2012/Jun/27 */
/* Version: 1.0.00 */
/* Content: Test file. */
/* Description: Test with Filteirng class. */

#include "FileImage.hpp"
#include "GradientGabor.hpp"
#include "Image.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( ( argc < 4 ) || ( argc > 5 ) ) {
    cout << "Usage: " << argv[ 0 ] << " <input image> <standard deviation> <output magnitude> [<output direction> ]" <<
    endl;
    return( 0 );
  }

  /* reading parameters */
  Image< int > img( Read< int >( argv[ 1 ] ) );
  Image< int > mag( img );
  Image< int > dir( img );
  float sigma = atof( argv[ 2 ] );

  Gradient::Gabor( img, sigma, &mag, &dir );

  Write( mag, argv[ 3 ], argv[ 1 ] );
  if( argc == 5 ) {
    Write( dir, argv[ 4 ], argv[ 1 ] );
  }
  return( 0 );
}
