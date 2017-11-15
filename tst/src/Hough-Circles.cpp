/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2012/Jun/27 */
/* Content: Test file. */
/* Description: Test with Filteirng class. */

#include "DrawCircle.hpp"
#include "FileImage.hpp"
#include "HoughCircle.hpp"
#include "Image.hpp"
#include "Matrix.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc != 11 ) {
    cout << "Usage: " << argv[ 0 ] << " <input image> <output image> <scale> <min dist> <min radius> <max radius> " <<
    "<canny threshold> <detection threshold> <multi scale canny levels> <clean edges>" << endl;
    cout << "\t<scale>: > 0.0. Compression scale for the image detection." << endl;
    cout << "\t<min dist>: > 0.0. Minimum distance between two detected circles." << endl;
    cout << "\t<min radius> <max radius>: max radius >= min radius > 0.0. Minimum and maximum radius of the expected "
         << "circles." << endl;
    cout << "\t<canny_threshold>: between 0.0 and 1.0. High hysteresis threshold for canny edge detection." << endl;
    cout << "\t<detection threshold>: > 0.0. A threshold to validate circles based on Hough accumulator. " <<
    "Suggested: 10 to 200." << endl;
    cout << "\t<multi scale canny levels>: 1 to 4. Higher values will generate more edges in heterogeneous brightness "
         << "regions." << endl;
    cout << "\t<clean edges>: 0 or 1. 1 to remove non-circular border segments." << endl;
    return( 0 );
  }
  /* reading parameters */
  Image< int > img( Read< int >( argv[ 1 ] ) );
  float scale = atof( argv[ 3 ] );
  float min_dist = atof( argv[ 4 ] );
  float min_radius = atof( argv[ 5 ] );
  float max_radius = atof( argv[ 6 ] );
  float canny_threshold = atof( argv[ 7 ] );
  float detection_threshold = atof( argv[ 8 ] );
  int canny_levels = atoi( argv[ 9 ] );
  int clean_edges = atoi( argv[ 10 ] );

  Vector< Circle > circles = Hough::HoughCircles( img, scale, min_dist, min_radius, max_radius, canny_threshold,
                                                  detection_threshold, canny_levels, clean_edges );

  /* img.Set( 0 ); */
  Image< int > res( img );
  for( size_t elm = 0; elm < circles.size( ); ++elm ) {
    circles[ elm ].Draw( res );
    circles[ elm ].Print( cout );
  }
  Write( res, argv[ 2 ], argv[ 1 ] );

  return( 0 );
}
