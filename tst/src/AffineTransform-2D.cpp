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
  if( argc < 5 ) {
    cout << "Usage: " << argv[ 0 ] <<
    " <input image> <output image1> <output image2> <output image3> <output image4>" << endl;
    return( 0 );
  }
  COMMENT( "Input image.", 0 );
  Image< int > img( Read< int >( argv[ 1 ] ) );

  COMMENT( "Resultant images.", 0 );
  Image< int > res1( img );
  Image< int > res2( img );
  Image< int > res3( img );
  Image< int > res4( img );

  COMMENT( "Test variables.", 0 );
  float angle = 0.5;
  int translate_x = 100;
  int translate_y = 50;
  float shear_x = 0.5;
  float shear_y = 0.0;

  COMMENT( "Testing affine based methods.", 0 );
  
  Matrix< float > transform( 4, 4 );
  transform = Geometrics::Translate( -static_cast< int >( img.Size( 0 ) ) / 2, 
                                     -static_cast< int >( img.Size( 1 ) ) / 2 );
  transform = Geometrics::Rotate( transform, angle );
  transform = Geometrics::Translate( transform, static_cast< int >( img.Size( 0 ) ) / 2, 
                                     static_cast< int >( img.Size( 1 ) ) / 2 );
  transform = Geometrics::Shear( transform, shear_x, 0.0 );

  COMMENT( "Testing other methods.", 0 );
  res1 = Geometrics::AffineTransform( img, transform );
  res1 = Geometrics::Scale( res1, 2.0, 1.0 );
  res2 = Geometrics::Rotate( img, angle );
  res3 = Geometrics::Shear( img, shear_x, shear_y );
  res4 = Geometrics::Translate( img, -translate_x, -translate_y );

  COMMENT( "Writing results to files.", 0 );
  Write( res1, argv[ 2 ], argv[ 1 ] );
  Write( res2, argv[ 3 ], argv[ 1 ] );
  Write( res3, argv[ 4 ], argv[ 1 ] );
  Write( res4, argv[ 5 ], argv[ 1 ] );

  return( 0 );
}
