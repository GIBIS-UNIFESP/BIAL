/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2018/Nov/29 */
/* Content: Test file. */
/* Description: Test with Image class. */

#include "FileImage.hpp"
#include "Image.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc < 1 ) {
    cout << "Creates a binary mask, setting non-zero values to 1." << endl;
    cout << "Usage: " << argv[ 0 ] << " <input image>" << endl;
    return( 0 );
  }
  Image< int > img( Read< int >( argv[ 1 ] ) );
  Vector< size_t > img_hist( img.Maximum( ) + 1 );
  for( size_t pxl = 0; pxl < img.size( ); ++pxl )
    ++img_hist[ img[ pxl ] ];
  for( size_t bin = 0; bin < img_hist.size( ); ++bin ) {
    if( img_hist[ bin ] > 0 )
      std::cout << bin << ": " << img_hist[ bin ] << std::endl;
  }
  return( 0 );
}
