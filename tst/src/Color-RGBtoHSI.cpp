/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2016/Jan/13 */
/* Content: Test file. */
/* Description: Test with ColorBasics class. */

#include "Color.hpp"
#include "ColorHSI.hpp"
#include "RealColor.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc != 4 ) {
    cout << "Usage: " << argv[ 0 ] << " <red> <green> <blue>" << endl;
    cout << "\tAll values between 0 and 255." << endl;
    return( 0 );
  }

  Color rgb;
  rgb( 1 ) = atoi( argv[ 1 ] );
  rgb( 2 ) = atoi( argv[ 2 ] );
  rgb( 3 ) = atoi( argv[ 3 ] );

  cout << "Input RGB: " << rgb << endl;

  RealColor hsi( ColorSpace::ARGBtoAHSI( rgb ) );

  cout << "HSI: " << hsi << endl;

  rgb = ColorSpace::AHSItoARGB( hsi );

  cout << "Final RGB: " << rgb << endl;

  return( 0 );
}
