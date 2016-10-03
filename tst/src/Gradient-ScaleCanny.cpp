/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Author: Fábio Augusto Menocci Cappabianco */
/* Date: 2012/Jun/27 */
/* Version: 1.0.00 */
/* Content: Test file. */
/* Description: Test with Filteirng class. */

#include "FileImage.hpp"
#include "GradientScaleCanny.hpp"
#include "Image.hpp"
#include "Matrix.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc != 6 ) {
    cout << "Usage: " << argv[ 0 ] <<
    " <input image> <output image> <window side> <lower threshold> <higher threshold>" << endl;
    cout << "\t\t<lower threshold>, <higher threshold>: From 0.0 to 1.0." << endl;
    cout << "\t\t<window side>: From 1 to window maximum side." << endl;
    return( 0 );
  }
  /* reading parameters */
  Image< int > img( Read< int >( argv[ 1 ] ) );
  size_t window_side = atoi( argv[ 3 ] );
  float low_thres = atof( argv[ 4 ] );
  float hig_thres = atof( argv[ 5 ] );

  Image< int > res( Gradient::ScaleCanny( img, window_side, low_thres, hig_thres ) );

  Write( res, argv[ 2 ], argv[ 1 ] );

  return( 0 );
}
