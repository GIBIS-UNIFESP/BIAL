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
  if( argc != 8 ) {
    cout << "Usage: " << argv[ 0 ] <<
    " <input image> <output image> <lower threshold> <higher threshold> <sigma> <scales> <fraction>" << endl;
    cout << "\t\t<lower threshold>, <higher threshold>: Hysteresis thresholds From 0.0 to 1.0." << endl;
    cout << "\t\t<sigma>: Standard deviation of Gaussian filter. From 0.0 to +oo." << endl;
    cout << "\t\t<fraction>: Fraction of the maximum edge size to be ignored. In (0.0, 1.0]." << endl;
    return( 0 );
  }
  /* reading parameters */
  Image< int > img( Read< int >( argv[ 1 ] ) );
  float low_thres = atof( argv[ 3 ] );
  float hig_thres = atof( argv[ 4 ] );
  float sigma = atof( argv[ 5 ] );
  size_t scales = atoi( argv[ 6 ] );
  float fraction = atof( argv[ 7 ] );

  Image< int > res( Gradient::MultiScaleSizeCanny( img, low_thres, hig_thres, sigma, scales, fraction ) );

  Write( res, argv[ 2 ], argv[ 1 ] );

  return( 0 );
}
