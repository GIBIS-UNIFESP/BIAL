/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Author: Fábio Augusto Menocci Cappabianco */
/* Date: 2012/Jun/27 */
/* Version: 1.0.00 */
/* Content: Test file. */
/* Description: Test with Filteirng class. */

#include "FileImage.hpp"
#include "FilteringGaussian.hpp"
#include "Image.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( ( argc < 3 ) || ( argc > 5 ) ) {
    cout << "Usage: " << argv[ 0 ] << " <input image> <output image> [<radius> [ <stamdard deviation> ] ]"
         << endl;
    cout << "\t\t<radius>: [1.0, 20.0]. Default: 1.9" << endl;
    cout << "\t\t<standard deviation>: (0.0,+oo). Default: 2.0" << endl;
    return( 0 );
  }

  /* reading parameters */
  float radius = 1.9;
  if( argc > 3 ) {
    radius = atof( argv[ 3 ] );
  }
  float std_dev = 2.0;
  if( argc > 4 ) {
    std_dev = atof( argv[ 4 ] );
  }
  if( ( radius < 1.0 ) || ( radius > 20.0 ) ) {
    cout << "Error: radius range: [1.0, 20.0]. Given: " << radius << endl;
    return( 0 );
  }
  if( std_dev <= 0.0 ) {
    cout << "Error: Standard deviation range: (0.0, +oo). Given: " << std_dev << endl;
    return( 0 );
  }
  Image< int > img( Read< int >( argv[ 1 ] ) );
  Image< int > res( Filtering::Gaussian( img, radius, std_dev ) );

  Write( res, argv[ 2 ], argv[ 1 ] );

  return( 0 );
}
