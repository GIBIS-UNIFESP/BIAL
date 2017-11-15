/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2013/Sep/26 */
/* Content: Test file. */
/* Description: Test colorspace conversion functions. */

#include "Color.hpp"
#include "ColorLab.hpp"
#include "FileImage.hpp"
#include "Image.hpp"
#include "RealColor.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc != 3 ) {
    cout << "Usage: " << argv[ 0 ] << " <input file name> <output file name> " << endl;
    return( 0 );
  }
  Image< Color > img( Read< Color >( argv[ 1 ] ) );

  Image< RealColor > trans( ColorSpace::ARGBtoXYZ( img ) );
  trans = ColorSpace::XYZtoCIELab( trans );
  trans = ColorSpace::CIELabtoXYZ( trans );
  img = ColorSpace::XYZtoARGB( trans );

  WritePNM( img, argv[ 2 ], PNMHeader( ), false );

  return( 0 );
}
