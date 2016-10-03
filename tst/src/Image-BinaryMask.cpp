/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Author: Fábio Augusto Menocci Cappabianco */
/* Date: 2012/Sep/21 */
/* Version: 1.0.00 */
/* Content: Test file. */
/* Description: Test with Image class. */

#include "FileImage.hpp"
#include "Image.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc < 3 ) {
    cout << "Creates a binary mask, setting non-zero values to 1." << endl;
    cout << "Usage: " << argv[ 0 ] << " <input image> <output image>" << endl;
    exit( -1 );
  }
  Image< int > scn( Read< int >( argv[ 1 ] ) );
  for( size_t p = 0; p < scn.size( ); ++p ) {
    if( scn[ p ] != 0 ) {
      scn[ p ] = 1;
    }
  }
  Write( scn, argv[ 2 ], argv[ 1 ] );

  return( 0 );
}
