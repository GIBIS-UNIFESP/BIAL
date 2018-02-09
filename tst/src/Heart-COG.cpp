/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2012/Sep/21 */
/* Content: Test file. */
/* Description: Test with Heart class and COG function. */

#include "DrawBox.hpp"
#include "FileImage.hpp"
#include "HeartCOG.hpp"
#include "Image.hpp"
#include "Integral.hpp"
#include "NiftiHeader.hpp"
#include "Vector.hpp"

using namespace std;
using namespace Bial;


int main( int argc, char **argv ) {
  if( argc != 2 ) {
    cout << "usage: " << argv[ 0 ] << " <input>" << endl;
    cout << "input: .............. original image" << endl;
    return( 0 );
  }
  COMMENT( "Reading data contents.", 0 );
  Image< float > scn( Read< float >( argv[ 1 ] ) );
  NiftiHeader nii( argv[ 1 ] );

  COMMENT( "Getting image orientation", 0 );
  string orientation = nii.Orientation( );

  COMMENT( "Initializing output files", 0 );
  Vector< Box > best_rectangles;
  Image< float > similarity( scn );

  COMMENT( "Computing COG", 0 );
  Vector< size_t > COG = Heart::WeightedCOG( scn, orientation, best_rectangles, similarity );

  COMMENT( "Drawing regions to an image", 0 );
  Image< int > regions( scn );
  regions.Set( 0 );
  std::cout << "Rectangles: " << std::endl;
  for( size_t rtg = 0; rtg < best_rectangles.size( ); ++rtg ) {
    std::cout << rtg << ":" << std::endl;
    best_rectangles[ rtg ].Print( std::cout );
    best_rectangles[ rtg ].Draw( regions );
  }
  COMMENT( "Printing COG value", 0 );
  cout << COG[ 0 ] << " " << COG[ 1 ] << " " << COG[ 2 ] << endl;

  COMMENT( "Writting output images", 0 );
  Write( similarity, "dat/similarity.nii.gz", argv[ 1 ] );
  Write( regions, "dat/regions.nii.gz", argv[ 1 ] );

  return( 0 );
}
