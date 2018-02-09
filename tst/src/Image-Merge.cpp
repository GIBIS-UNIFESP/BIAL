/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2013/Oct/31 */
/* Content: Test file. */
/* Description: Test Dicom, Directories and Merging process. */

#include "FileImage.hpp"
#include "Image.hpp"
#include "ImageMerge.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc != 3 ) {
    cout << "Usage: " << argv[ 0 ] << " <input dicom dir> <output nifti or scene file>" << endl;
    return( 0 );
  }
  Vector< Image< int > > slices( ReadDir< int >( argv[ 1 ] ) );
  Image< int > merge( ImageOp::Merge( slices ) );

  Write( merge, argv[ 2 ] );

  return( 0 );
}
