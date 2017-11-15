/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2016/Feb/04 */
/* Content: Difference the content of two images. */
/* Description: */

#include "FileImage.hpp"
#include "Image.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc < 3 ) {
    cout << "Usage: " << argv[ 0 ] << " <input image 1> <input image 2> [<print_values>]" << endl;
    cout << "\t<print_values>: Set to any value to print image intensity for different pixels." << endl;
    exit( -1 );
  }
  Image< int > img1( Read< int >( argv[ 1 ] ) );
  Image< int > img2( Read< int >( argv[ 2 ] ) );

  bool print_vals = false;
  if( argc == 4 )
    print_vals = true;

  for( size_t pxl = 0; pxl < img1.size( ); ++pxl ) {
    if( img1[ pxl ] != img2[ pxl ] ) {
      cout << pxl << " ";
      if( print_vals )
        cout << img1[ pxl ] << ", " << img2[ pxl ];
      cout << endl;
    }
  }
  
  return( 0 );
}
