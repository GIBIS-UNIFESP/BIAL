/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2012/Jun/27 */
/* Content: Test Gradient hole closing. */
/* Description: Test with Filteirng class. */

#include "FileImage.hpp"
#include "GradientPosOperations.hpp"
#include "GradientScaleCanny.hpp"
#include "Image.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc != 7 ) {
    cout << "Usage: " << argv[ 0 ] <<
    " <input image> <output image> <lower threshold> <higher threshold> <sigma> <scales>" << endl;
    cout << "\t\t<lower threshold>, <higher threshold>: Hysteresis thresholds From 0.0 to 1.0." << endl;
    cout << "\t\t<sigma>: Standard deviation of Gaussian filter. From 0.0 to +oo." << endl;
    return( 0 );
  }
  COMMENT( "Reading inputs.", 0 );
  Image< int > img( Read< int >( argv[ 1 ] ) );
  float low_thres = atof( argv[ 3 ] );
  float hig_thres = atof( argv[ 4 ] );
  float sigma = atof( argv[ 5 ] );
  size_t scales = atoi( argv[ 6 ] );

  COMMENT( "Runing multi-scale canny.", 0 );
  Image< int > grad( Gradient::MultiScaleCanny( img, low_thres, hig_thres, sigma, scales ) );
  COMMENT( "Setting all canny gradient levels to 1.", 0 );
  for( size_t pxl = 0; pxl < grad.size( ); ++pxl ) {
    if( grad( pxl ) != 0 )
      grad( pxl ) = 1;
  }
  COMMENT( "Closing holes.", 0 );
  Image< int > res( Gradient::CloseHoles( grad ) );

  Write( res, argv[ 2 ], argv[ 1 ] );

  return( 0 );
}
