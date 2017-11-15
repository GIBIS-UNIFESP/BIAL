/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2015/Jun/09 */
/* Content: Test file. */
/* Description: Test with Edge detection function. */

#include "FileImage.hpp"
#include "Image.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc < 5 ) {
    cout << "Program to trace the edges of a path after running edge delineation algorithms such as live-wire." << endl;
    cout << "Usage: " << argv[ 0 ] << " <predecessor map image> <output image> <end point coordinates>" << endl;
    cout << "\t\t<end point coordinates>: Coordinates of the point that ends the path." << endl;
    return( 0 );
  }
  COMMENT( "Reading input image.", 0 );
  Image< int > pred( Read< int >( argv[ 1 ] ) );
  COMMENT( "Reading end coordinates.", 0 );
  Vector< int > coord( 3, 0 );
  coord[ 0 ] = atoi( argv[ 3 ] );
  coord[ 1 ] = atoi( argv[ 4 ] );
  if( argc == 6 )
    coord[ 2 ] = atoi( argv[ 5 ] );

  COMMENT( "Creating resulting image.", 0 );
  Image< int > res( pred.Dim( ), pred.PixelSize( ) );
  
  COMMENT( "Running though the image backwards to the root.", 0 );
  int leaf = static_cast< int >( pred.Position( coord ) );
  do {
    res[ leaf ] = 1;
    leaf = pred[ leaf ];
  } while( pred[ leaf ] != leaf );
  
  COMMENT( "Writing result.", 0 );
  Write( res, argv[ 2 ], argv[ 1 ] );

  return( 0 );
}
