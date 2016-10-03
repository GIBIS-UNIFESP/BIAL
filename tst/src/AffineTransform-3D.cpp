/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Author: Fábio Augusto Menocci Cappabianco */
/* Date: 2014/Jun/11 */
/* Version: 1.0.00 */
/* Content: Test file. */
/* Description: Test with Geometric class. */

#include "FileImage.hpp"
#include "GeometricsAffine.hpp"
#include "GeometricsRotate.hpp"
#include "GeometricsScale.hpp"
#include "GeometricsShear.hpp"
#include "GeometricsTranslate.hpp"
#include "Image.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  /* Verifies the input */
  if( argc != 5 ) {
    cout << "Usage: " << argv[ 0 ] << " <input image> <output 1> <output 2> <output 3>" << endl;
    return( 0 );
  }
  /* Input image */
  Image< int > img( Read< int >( argv[ 1 ] ) );

  Image< int > resx( Geometrics::Rotate( img, 0.5, 0 ) );
  Image< int > resy( Geometrics::Shear( img, 0, 0, 0, 0, 0, 0.2 ) );

  Matrix< float > transform( 4, 4 );
  transform = Geometrics::Translate( -static_cast< int >( img.Size( 0 ) ) / 2, 
                                     -static_cast< int >( img.Size( 1 ) ) / 2,
                                     -static_cast< int >( img.Size( 2 ) ) / 2 );
  transform = Geometrics::Rotate( transform, 0.5, 0 );
  transform = Geometrics::Translate( transform, static_cast< double >( img.Size( 0 ) ) / 2, 
                                     static_cast< double >( img.Size( 1 ) ) / 2,
                                     static_cast< double >( img.Size( 2 ) ) / 2 );
  transform = Geometrics::Scale( transform, 0.8, 0.8, 0.8 );

  Image< int > resz( Geometrics::AffineTransform( img, transform ) );

  Write( resx, argv[ 2 ], argv[ 1 ] );
  Write( resy, argv[ 3 ], argv[ 1 ] );
  Write( resz, argv[ 4 ], argv[ 1 ] );

  return( 0 );
}
