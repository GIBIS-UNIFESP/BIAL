/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2012/Jun/27 */
/* Content: Application. */
/* Description: Copies Nifti header information from an image to other. Keeps only pixel dimensions and number of
 *pixels. */

#include "FileImage.hpp"
#include "Image.hpp"
#include "NiftiHeader.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc != 4 ) {
    cout << "Usage: " << argv[ 0 ] << " <source image> <target image> <output image>" << endl;
    return( 0 );
  }
  Image< double > img( Read< double >( argv[ 2 ] ) );

  Write( img, argv[ 3 ], argv[ 1 ] );

  return( 0 );
}
