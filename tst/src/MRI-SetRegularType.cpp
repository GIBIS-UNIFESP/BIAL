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
  if( argc < 2 ) {
    cout << "Usage: " << argv[ 0 ] << " <input image> <output image>" << endl;
    exit( -1 );
  }
  Image< int > mri( Read< int >( argv[ 1 ] ) );
  NiftiHeader hdr( argv[ 1 ] );
  string reg( "r" );
  
  hdr.DataTypeString( reg );

  WriteNifti( mri, argv[ 2 ], hdr );

  return( 0 );
}
