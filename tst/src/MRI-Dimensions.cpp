/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Author: Fábio Augusto Menocci Cappabianco */
/* Date: 2012/Jun/27 */
/* Version: 1.0.00 */
/* Content: Test file. */
/* Description: Testing 3D image header functions. */

#include "FileImage.hpp"
#include "Image.hpp"
#include "NiftiHeader.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc != 2 ) {
    cout << "Uso: " << argv[ 0 ] << "<Nifti file>" << endl;
    return( 0 );
  }
  Image< int > mri( Read< int >( argv[ 1 ] ) );
  mri.PrintDimensions( cout );

  return( 0 );
}
