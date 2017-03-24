/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2015/Sep/15 */
/* Content: Test file. */
/* Description: Test with superpixels. */

#include "FileImage.hpp"
#include "Image.hpp"
#include "Matrix.hpp"
#include "SegmentationIntensityThreshold.hpp"
#include "StatisticsDice.hpp"
#include "Superpixel.hpp"

using namespace Bial;
using namespace std;

int main( int argc, char *argv[ ] ) {
  
  if( argc != 5 ){
    cout << "usage: " << argv[ 0 ] << " <brain image> <result> <supervoxel_size> <compactness>" << endl;
    cout << "\t <supervoxel_size>... The desired size of supervoxel." << endl;
    cout << "\t <compactness>....... The relative importance between color similarity and spatial proximity." << endl;
    cout << "\t                      It can be in the range [1, 100]." << endl;
    exit( 0 );
  }

  COMMENT( "Reading parameters.", 0 );
  Image< int > img( Read< int >( argv[ 1 ] ) );
  int sup_pixel_size = atoi( argv[ 3 ] );
  double compactness = atof( argv[ 4 ] );

  COMMENT( "Running slic.", 0 );
  Image< int > label( Superpixel::SuperVoxel( img, sup_pixel_size, compactness ) );

  COMMENT( "Writing result.", 0 );
  Write( label, argv[ 2 ], argv[ 1 ] );

  return( 0 );
}
