/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Author: Fábio Augusto Menocci Cappabianco */
/* Date: 2012/Jun/27 */
/* Version: 1.0.00 */
/* Content: Test file. */
/* Description: Test with Filteirng class. */

#include "GradientCanny.hpp"
#include "FileImage.hpp"
#include "Image.hpp"
#include "Matrix.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc != 3 ) {
    cout << "Usage: " << argv[ 0 ] << " <input image> <output magnitude>" << endl;
    return( 0 );
  }
  /* reading parameters */
  Image< int > img( Read< int >( argv[ 1 ] ) );

  Image< int > res( Gradient::NonMaxSobelSuppression( img ) );

  Write( res, argv[ 2 ], argv[ 1 ] );

  return( 0 );
}
