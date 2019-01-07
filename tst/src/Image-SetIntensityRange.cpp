/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2018/Nov/06 */
/* Content: Sets image intensity range to desired values. */
/* Description: */

#include "FileImage.hpp"
#include "Image.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc < 5 ) {
    cout << "Usage: " << argv[ 0 ] << " <input image> <output image> <lowest intensity> <highest intensity>" << endl;
    exit( -1 );
  }
  Image< float > img1( Read< float >( argv[ 1 ] ) );
  int new_min = atoi( argv[ 3 ] );
  int new_max = atoi( argv[ 4 ] );
  double old_min = img1.Minimum( );
  double old_max = img1.Maximum( );
  Image< int > res( img1 );

  std::cout << "old_min: " << old_min << ", old_max: " << old_max << std::endl;
  for( size_t pxl = 0; pxl < img1.size( ); ++pxl ) {
    double norm = ( img1[ pxl ] - old_min ) / ( old_max - old_min );
    res[ pxl ] = norm * ( new_max - new_min ) + new_min;
  }
  
  Write( res, argv[ 2 ] );
  return( 0 );
}
