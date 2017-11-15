/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2012/Sep/13 */
/* Content: Test file. */
/* Description: Test with RandomQueue and Filtering classes. */

#include "Common.hpp"
#include "FileImage.hpp"
#include "FilteringMedian.hpp"
#include "Image.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc != 4 ) {
    cout << "Usage: " << argv[ 0 ] << " <input image> <filtering radius> <output image>" << endl;
    return( 0 );
  }
  Image< int > img( Read< int >( argv[ 1 ] ) );
  float radius = atof( argv[ 2 ] );
  Image< int > res( Filtering::Median( img, radius ) );
  Write( res, argv[ 3 ], argv[ 1 ] );
  return( 0 );
}
