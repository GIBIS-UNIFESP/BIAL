/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2013/Nov/01 */
/* Content: Test file. */
/* Description: Test with Segmentation class. */

#include "FileImage.hpp"
#include "Image.hpp"
#include "SegmentationIntensityThreshold.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc != 5 ) {
    cout << "Usage: " << argv[ 0 ] << " <input image> <output image> <minimum> <maximum>]" << endl;
    cout << "\t\t 0.0 <= minimum <= maximum <= 1.0." << endl;
    return( 0 );
  }
  /* Reading input data. */
  float minimum = atof( argv[ 3 ] );
  float maximum = atof( argv[ 4 ] );
  Image< int > img( Read< int >( argv[ 1 ] ) );
  if( ( minimum > maximum ) || ( minimum < 0.0 ) || ( maximum > 1.0 ) ) {
    cout << "Error: 0.0 <= minimum <= maximum <= 1.0." << endl;
    return( 0 );
  }
  int img_min = img.Minimum( );
  int img_max = img.Maximum( );

  int min_thres = static_cast< int >( round( img_min + ( img_max - img_min ) * minimum ) );
  int max_thres = static_cast< int >( round( img_min + ( img_max - img_min ) * maximum ) );

  /* Running segmentation */
  Segmentation::IntensityThreshold( img, min_thres, max_thres );

  /* Writing results. */
  Write( img, argv[ 2 ], argv[ 1 ] );

  return( 0 );
}
