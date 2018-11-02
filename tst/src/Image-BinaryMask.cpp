/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2012/Sep/21 */
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
  Image< float > scn( Read< float >( argv[ 1 ] ) );
  size_t zeros = 0;
  size_t non_zeros = 0;
  for( size_t p = 0; p < scn.size( ); ++p ) {
    if( ( scn[ p ] >  -0.00001 ) && ( scn[ p ] <  0.00001 ) ) {
      scn[ p ] = 0;
      ++zeros;
    }
    else {
      scn[ p ] = 1;
      ++non_zeros;
    }
  }
  std::cout << "Zeros: " << zeros << ", non-zeros: " << non_zeros << std::endl;
  Write( scn, argv[ 2 ] );
  return( 0 );
}
