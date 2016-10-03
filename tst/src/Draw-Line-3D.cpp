/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Author: Lucas Santana Lellis */
/* Date: 2013/Sep/26 */
/* Version: 1.0.00 */
/* Content: Test file. */
/* Description: Test colorspace conversion functions. */

#include "Color.hpp"
#include "DrawLine.hpp"
#include "FileImage.hpp"
#include "Image.hpp"
#include "Vector.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc != 2 ) {
    cout << "Usage: " << argv[ 0 ] << " <output file name> " << endl;
    return( 0 );
  }
  /* Spatial dimensions of the image */
  const Vector< size_t > spc_dim = { 800, 600, 300 };
  COMMENT( "Creating gray image ( 3d image )", 1 );
  Image< int > img( spc_dim );
  Line l1( { 0, 0, 0 }, { 799, 599, 299 }, Color( 255, 255, 255, 255 ) );
  l1.Draw( img );
  l1 = Line( { 0, 0, 0 }, { 799, 599, 299 }, Color( 255, 128, 128, 128 ) );
  l1.Draw( img, { 12, 22, 34 } );
  l1 = Line( { 0, 0, 0 }, { 799, 599, 299 }, Color( 255, 64, 64, 64 ) );
  l1.Draw( img, { 54, -85, -52 } );

  Line l2( { 400, 151, 420 }, { -201, 804, 299 }, Color( 255, 255, 255, 255 ) );
  l2.Draw( img );
  l2 = Line( { 400, 151, 420 }, { -201, 804, 299 }, Color( 255, 128, 128, 128 ) );
  l2.Draw( img, { 21, 54, 35 } );
  l2 = Line( { 400, 151, 420 }, { -201, 804, 299 }, Color( 255, 64, 64, 64 ) );
  l2.Draw( img, { 52, -84, -56 } );

  Line l3( { 158, 11, 68 }, { -45, 356, -85 }, Color( 255, 255, 255, 255 ) );
  l3.Draw( img );
  l3 = Line( { 158, 11, 68 }, { -45, 356, -85 }, Color( 255, 128, 128, 128 ) );
  l3.Draw( img, { 32, -22, 34 } );
  l3 = Line( { 158, 11, 68 }, { -45, 356, -85 }, Color( 255, 64, 64, 64 ) );
  l3.Draw( img, { 44, -65, -75 } );

  Write( img, argv[ 1 ] );
  return( 0 );
}
