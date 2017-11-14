/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2013/Sep/26 */
/* Content: Test file. */
/* Description: Test colorspace conversion functions. */

#include "Color.hpp"
#include "DrawCircle.hpp"
#include "FileImage.hpp"
#include "Image.hpp"
#include "Vector.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc < 2 ) {
    cout << "Usage: " << argv[ 0 ] << " <output file name> [<optional input file>] " << endl;
    return( 0 );
  }
  COMMENT( "Creating ARGB image ( 2d image with 4 channels )", 1 );
  Image< Color > img( 800, 600 );
  if( argc == 3 ) {
    img = Read< Color >( argv[ 2 ] );
  }
  COMMENT( "Creating circle colors.", 0 );
  Color red = { 128, 128, 0, 0 };
  Color green = { 255, 0, 255, 0 };
  Color blue = { 128, 0, 0, 128 };
  Color yellow = { 128, 255, 255, 0 };

  COMMENT( "Setting the center of the circle.", 0 );
  const Vector< float > dsp( 2, 10 );
  const Vector< float > ctr( { img.size( 0 ) / 2.0f, img.size( 1 ) / 2.0f } );

  COMMENT( "Initializing Filled Circle (c1) with radius 80 and red color", 1 );
  const Circle c1( { 0.0f, 0.0f }, 80, red, true );

  COMMENT( "Drawing c1 in img with color scale 128", 1 );
  c1.Draw( img, dsp );

  COMMENT( "Initializing Empty Circle (c2) with radius 90 and green color", 1 );
  const Circle c2( { 0.0f, 0.0f }, 90, green, false );

  COMMENT( "Drawing c2 in img with color scale 255, 0.5", 1 );
  c2.Draw( img, dsp );

  COMMENT( "Initializing Filled Circle (c3) with radius 50 and blue color", 1 );
  const Circle c3( ctr, 50, blue, true );

  COMMENT( "Drawing c4 in img with color scale 128", 1 );
  c3.Draw( img );

  COMMENT( "Initializing Empty Circle (c4) with radius 75 and yellow color", 1 );
  const Circle c4( ctr, 75, yellow, false );

  COMMENT( "Drawing c4 in img with color scale 255", 1 );
  c4.Draw( img );

  Write( img, argv[ 1 ] );
  return( 0 );
}
