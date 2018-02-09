/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Author: Lucas Santana Lellis */
/* Date: 2013/Sep/26 */
/* Version: 1.0.00 */
/* Content: Test file. */
/* Description: Program to draw figures into images. */

#include "DrawBox.hpp"
#include "DrawCircle.hpp"
#include "DrawLine.hpp"
#include "DrawPoint.hpp"
#include "FileImage.hpp"
#include "Image.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc < 7 ) {
    cout << "Usage: " << argv[ 0 ] << " <input file> <output file> <figure options> " << endl;
    cout << "\t\t<figure options>: Empty box: 0 <x0> <y0> [<z0>]  <xn> <yn> [<zn>] <alpha> <red or gray> " << 
      "[<green> <blue>]" << endl;
    cout << "\t\t                  Filled box: 1 <x0> <y0> [<z0>]  <xn> <yn> [<zn>] <alpha> <red or gray> " <<
      "[<green> <blue>]" << endl;
    cout << "\t\t                  Line: 2 <x0> <y0> [<z0>]  <xn> <yn> [<zn>] <alpha> <red or gray> [<green> <blue>]" 
         << endl;
    cout << "\t\t                  Empty circle: 3 <cx> <cy> <radius> <alpha> <red or gray> [<green> <blue>]" << endl;
    cout << "\t\t                  Full circle: 4 <cx> <cy> <radius> <alpha> <red or gray> [<green> <blue>]" << endl;
    cout << "\t\t                  Point: 5 <x> <y> [<z>] <alpha> <red or gray> [<green> <blue>]" << endl;
    cout << "\t\t<alpha>,<colors>: 0.0 to 1.0." << endl;
    return( 0 );
  }

  Image< Color > img( Read< Color >( argv[ 1 ] ) );
  bool full = true;
  switch( atoi( argv[ 3 ] ) ) {
    
  case 0: {
    COMMENT( "Empty box.", 0 );
    full = false;
  }
  case 1: {
    if( atoi( argv[ 3 ] ) == 1 ) {
      COMMENT( "Full box.", 0 );
    }
    COMMENT( "Reading data.", 1 );
    Vector< float > low_coord( img.Dims( ) );
    Vector< float > high_coord( img.Dims( ) );
    Vector< float > size( img.Dims( ) );
    for( size_t dms = 0; dms < img.Dims( ); ++dms ) {
      low_coord[ dms ] = atof( argv[ dms + 4 ] );
      high_coord[ dms ] = atof( argv[ dms + 4 + img.Dims( ) ] );
      size[ dms ] = std::abs( low_coord[ dms ] - high_coord[ dms ] );
    }

    COMMENT( "Reading color.", 1 );
    Color color;
    color( 0 ) = atof( argv[ 4 + 2 * img.Dims( ) ] ); /* alpha */
    for( size_t clr = 0; clr < argc - 5u - 2u * img.Dims( ); ++clr )
      color( clr + 1 ) = atof( argv[ clr + 5 + 2 * img.Dims( ) ] );
    COMMENT( "Creating box with coordinates: " << low_coord << "-" << high_coord << ", color: " << color << ".", 1 );
    Box box( low_coord, size, color, full );
    COMMENT( "Writing to image.", 1 );
    box.Draw( img );
    break;
  }

  case 2: {
    COMMENT( "Line.", 0 );
    COMMENT( "Reading data.", 1 );
    Vector< float > low_coord( img.Dims( ) );
    Vector< float > high_coord( img.Dims( ) );
    for( size_t dms = 0; dms < img.Dims( ); ++dms ) {
      low_coord[ dms ] = atof( argv[ dms + 4 ] );
      high_coord[ dms ] = atof( argv[ dms + 4 + img.Dims( ) ] );
    }
    COMMENT( "Reading color.", 1 );
    Color color;
    color( 0 ) = atof( argv[ 4 + 2 * img.Dims( ) ] ); /* alpha */
    for( size_t clr = 0; clr < argc - 5u - 2u * img.Dims( ); ++clr )
      color( clr + 1 ) = atof( argv[ clr + 5 + 2 * img.Dims( ) ] );
    COMMENT( "Creating line with coordinates: " << low_coord << "-" << high_coord << ", color: " << color << ".", 1 );
    Line line( low_coord, high_coord, color );
    line.Draw( img );
    break;
  }

  case 3: {
    COMMENT( "Empty circle.", 0 );
    full = false;
  }

  case 4: {
    if( atoi( argv[ 3 ] ) == 5 ) {
      COMMENT( "Full circle.", 0 );
    }
    COMMENT( "Reading data.", 1 );
    Vector< float > center( 2 );
    for( size_t dms = 0; dms < 2; ++dms ) {
      center[ dms ] = atof( argv[ dms + 4 ] );
    }
    float radius = atof( argv[ 6 ] );
    COMMENT( "Reading color.", 1 );
    Color color;
    color( 0 ) = atof( argv[ 7 ] ); /* alpha */
    for( size_t clr = 0; clr < argc - 8u; ++clr )
      color( clr + 1 ) = atof( argv[ clr + 8 ] );
    COMMENT( "Creating line with center: " << center << ", radius" << radius << ", color: " << color << ".", 1 );
    Circle circle( center, radius, color, full );
    circle.Draw( img );
    break;
  }

  case 5: {
    COMMENT( "Point.", 0 );
    COMMENT( "Reading data.", 1 );
    Vector< float > coord( img.Dims( ) );
    for( size_t dms = 0; dms < img.Dims( ); ++dms )
      coord[ dms ] = atof( argv[ dms + 4 ] );
    COMMENT( "Reading color.", 1 );
    Color color;
    color( 0 ) = atof( argv[ img.Dims( ) + 4 ] ); /* alpha */
    for( size_t clr = 0; clr < argc - img.Dims( ) - 5u; ++clr )
      color( clr + 1 ) = atof( argv[ clr + img.Dims( ) + 5 ] );
    COMMENT( "Creating point with coordinates: " << coord << ", color: " << color << ".", 1 );
    Point point( coord, color );
    point.Draw( img );
    break;
  }

  default:
    cout << "Invalid Option." << endl;
  }
  Write( img, argv[ 2 ], argv[ 1 ] );

  return( 0 );
}
