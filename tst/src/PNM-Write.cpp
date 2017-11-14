/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Author: Fábio Augusto Menocci Cappabianco */
/* Date: 2013/Sep/26 */
/* Version: 1.0.00 */
/* Content: Test file. */
/* Description: Test writing functions for PNM files. */

#include "Color.hpp"
#include "FileImage.hpp"
#include "Image.hpp"
#include "PNMHeader.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc < 3 ) {
    cout << "Usage: " << argv[ 0 ] << " <input file name> <output file name>" << endl;
    exit( 1 );
  }
  Image< Color > img( Read< Color >( argv[ 1 ] ) );
  WritePNM( img, argv[ 2 ], PNMHeader( ), false );

  return( 0 );
}
