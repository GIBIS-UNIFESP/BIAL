/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2012/Sep/21 */
/* Content: Test file. */
/* Description: Test with Image class. */

#include "FileImage.hpp"
#include "Image.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc < 3 ) {
    cout << "Creates a binary mask, setting non-zero values to 1." << endl;
    cout << "Usage: " << argv[ 0 ] << " <input image> <output image>" << endl;
    exit( -1 );
  }
  Image< Color > scn( Read< Color >( argv[ 1 ] ) );
  Image< int > res( scn.Dim( ), scn.PixelSize( ) );
  for( size_t chl = 1; chl < 4; ++chl ) {
    for( size_t pxl = 0; pxl < scn.size( ); ++pxl ) {
      if( scn[ pxl ][ chl ] != 0 ) {
        res[ pxl ] = 1;
      }
    }
  }

  Write( res, argv[ 2 ], argv[ 1 ] );
  return( 0 );
}
