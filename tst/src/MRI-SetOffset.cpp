/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Author: Fábio Augusto Menocci Cappabianco */
/* Date: 2012/Jun/27 */
/* Version: 1.0.00 */
/* Content: Test file. */
/* Description: Testing Nifti header orientation operations. */

#include "FileImage.hpp"
#include "Image.hpp"
#include "NiftiHeader.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc < 6 ) {
    cout << "Usage: " << argv[ 0 ] << " <input image> <xoffset> <yoffset> <zoffset> <output image>" << endl;
    exit( -1 );
  }
  Image< int > mri( Read< int >( argv[ 1 ] ) );
  NiftiHeader hdr( argv[ 1 ] );

  hdr.QuaternOffset( atof( argv[ 2 ] ), atof( argv[ 3 ] ), atof( argv[ 4 ] ) );
  hdr.QuaternToMat( mri.PixelSize( ) );

  WriteNifti( mri, argv[ 5 ], hdr );

  return( 0 );
}
