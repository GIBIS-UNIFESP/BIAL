/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2015/May/03 */
/* Content: Test file. */
/* Description: Test with Filtering classes. */

#include "Common.hpp"
#include "FileImage.hpp"
#include "FilteringMean.hpp"
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
  Image< int > res( Filtering::Mean( img, radius ) );

  Write( res, argv[ 3 ], argv[ 1 ] );

  return( 0 );
}
