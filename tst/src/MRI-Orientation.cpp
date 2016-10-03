/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Author: Fábio Augusto Menocci Cappabianco */
/* Date: 2012/Jun/27 */
/* Version: 1.0.00 */
/* Content: Test file. */
/* Description: Testing Nifti header orientation operations. */

#include "NiftiHeader.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc < 2 ) {
    cout << "Usage: " << argv[ 0 ] << " <input image>" << endl;
    exit( -1 );
  }
  NiftiHeader hdr( argv[ 1 ] );

  cout << hdr.Orientation( ) << endl;

  return( 0 );
}