/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2013/Oct/28 */
/* Content: Test file. */
/* Description: Test with Dicom format file. */

#include "FileImage.hpp"
#include "Image.hpp"
#include "PNMHeader.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc < 3 ) {
    cout << "Usage: " << argv[ 0 ] << " <input dicom> <output pgm>" << endl;
    exit( -1 );
  }
  Image< double > img( Read< double >( argv[ 1 ] ) );

  Write( img, argv[ 2 ] );
  return( 0 );
}
