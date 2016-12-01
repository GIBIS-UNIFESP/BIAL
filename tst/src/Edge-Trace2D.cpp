/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2015/Jun/09 */
/* Content: Test file. */
/* Description: Test with Edge detection function. */

#include "FileImage.hpp"
#include "Image.hpp"
#include "Color.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc < 6 ) {
    cout << "Program to trace the edges of a path after running edge delineation algorithms such as live-wire." << endl;
cout << "Usage: " << argv[ 0 ] << " <input image> <predecessor map image> <output basename> <end point coordinates>" << endl;
    cout << "\t\t<end point coordinates>: Coordinates of the point that ends the path." << endl;
    return( 0 );
  }
  COMMENT( "Reading input image.", 0 );
  Image< int > pred( Read< int >( argv[ 2 ] ) );
  COMMENT( "Reading end coordinates.", 0 );
  Vector< int > coord( 3, 0 );
  coord[ 0 ] = atoi( argv[ 4 ] );
  coord[ 1 ] = atoi( argv[ 5 ] );

  COMMENT( "Creating resulting image.", 0 );
  Image< int > res( pred.Dim( ), pred.PixelSize( ) );
  Image< Color > cres( Read< Color >( argv[ 1 ] ) );
  
  COMMENT( "Running though the image backwards to the root.", 0 );
  res( coord ) = 1;
  cres( coord )[ 3 ] = 255;
  int leaf = static_cast< int >( pred.Position( coord ) );
  do {
    res[ leaf ] = 1;
    cres[ leaf ][ 1 ] = 128;
    cres[ leaf ][ 2 ] = 128;
    leaf = pred[ leaf ];
    cout << leaf << endl;
  } 
  while( ( pred[ leaf ] != leaf ) && ( pred[ leaf ] != -1 ) );
  res[ leaf ] = 1;
  cres[ leaf ][ 1 ] = 255;
  
  COMMENT( "Writing result.", 0 );
  Write( res, string( argv[ 3 ] ) + ".pgm", argv[ 2 ] );
  Write( cres, string( argv[ 3 ] ) + ".ppm", argv[ 1 ] );

  return( 0 );
}
