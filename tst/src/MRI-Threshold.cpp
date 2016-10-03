/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Author: Fábio Augusto Menocci Cappabianco */
/* Date: 2012/Jul/26 */
/* Version: 1.0.00 */
/* Content: Test file. */
/* Description: Basic test with MRI image class. */

#include "FileImage.hpp"
#include "Image.hpp"
#include "NiftiHeader.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc != 3 ) {
    cout << "Usage: " << argv[ 0 ] << " <input_image> <output_image>" << endl;
  }
  Image< int > img( Read< int >( argv[ 1 ] ) );
  NiftiHeader hdr( argv[ 1 ] );

  Image< int > res( img );

  int max = img.Maximum( );
  for( size_t elm = 0; elm < img.Size( ); ++elm ) {
    if( res( elm ) < max / 4 ) {
      res( elm ) = 0;
    }
    else {
      res( elm ) = 1;
    }
  }
  WriteNifti( res, argv[ 2 ], hdr );

  return( 0 );
}
