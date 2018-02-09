/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2012/Sep/21 */
/* Content: Test file. */
/* Description: Test with Brain class. */

#include "BrainInhomogeneity.hpp"
#include "FileImage.hpp"
#include "Image.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  string orientation;
  if( argc < 4 ) {
    cout << "Usage: " << argv[ 0 ] << " <MRI brain image> <binary brain mask> <resulting image> " <<
    "[ <radius> ]" << endl;
    cout << "radius: 7.0 to 28.0. Default: 15.5." << endl;
    exit( -1 );
  }
  Image< int > scn( Read< int >( argv[ 1 ] ) );
  Image< int > msk( Read< int >( argv[ 2 ] ) );

  float radius = 15.5;
  if( argc > 4 ) {
    radius = atof( argv[ 4 ] );
  }
  cout << "radius: " << radius << endl;

  Image< int > res( Brain::InhomogeneityCorrection( scn, msk, radius ) );

  Write( res, argv[ 3 ], argv[ 1 ] );

  return( 0 );
}
