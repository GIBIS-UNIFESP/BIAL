/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2012/Jun/27 */
/* Content: Test file. */
/* Description: Test with Kernel class. */

#include "KernelSobel.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc != 3 ) {
    cout << "Usage: " << argv[ 0 ] << " <dimensions> <direction>" << endl;
    cout << "\t\t<direction> should be smaller than <dimensions>." << endl;
    return( 0 );
  }
  size_t dimensions = atoi( argv[ 1 ] );
  size_t direction = atoi( argv[ 2 ] );
  if( dimensions <= direction ) {
    cout << "Error: Dimensions should be greater than direction. Given dimensions: " << dimensions <<
    ", direction: " << direction << endl;
    return( 0 );
  }
  Kernel krn = KernelType::NormalizedSobel( dimensions, direction );

  cout << "Kenel dimensions: " << krn.Dims( ) << ", elements: " << krn.size( ) << endl;
  for( size_t idx = 0; idx < krn.size( ); ++idx ) {
    cout << "displacement: ";
    for( size_t dms = 0; dms < krn.Dims( ); ++dms )
      cout << "dim[ " << dms << " ]: " << krn( idx, dms ) << " ";
    cout << "coefficient: " << krn.Value( idx ) << endl;
  }
  return( 0 );
}
