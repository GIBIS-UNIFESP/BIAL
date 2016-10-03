/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Author: Fábio Augusto Menocci Cappabianco */
/* Date: 2012/Jun/27 */
/* Version: 1.0.00 */
/* Content: Test file. */
/* Description: Test with Filteirng class. */

#include "FileImage.hpp"
#include "GradientSobel.hpp"
#include "Image.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc != 4 ) {
    cout << "Usage: " << argv[ 0 ] << " <input image> <output image> <direction> ]"
         << endl;
    cout << "\t\t<direction>: Lower than the number of dimensions of the input image." << endl;
    return( 0 );
  }
  /* reading parameters */
  Image< int > img( Read< int >( argv[ 1 ] ) );
  size_t direction = atoi( argv[ 3 ] );
  if( direction >= img.Dims( ) ) {
    cout << "Error: Invalid direction. Given: " << direction << ". Image dimensions: " << img.Dims( ) << endl;
    return( 0 );
  }
  Image< int > res( Gradient::DirectionalSobel( img, direction ) );

  Write( res, argv[ 2 ], argv[ 1 ] );

  return( 0 );
}
