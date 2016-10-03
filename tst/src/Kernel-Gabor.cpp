/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Author: Fábio Augusto Menocci Cappabianco */
/* Date: 2012/Jun/27 */
/* Version: 1.0.00 */
/* Content: Test file. */
/* Description: Test with Gabor Kernel class. */

#include "KernelGabor.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc != 4 ) {
    cout << "Usage: " << argv[ 0 ] << " <sigma> <dimensions> <direction>" << endl;
    cout << "\t\t<direction> should be smaller than <dimensions>." << endl;
    return( 0 );
  }
  float sigma = atof( argv[ 1 ] );
  size_t dimensions = atoi( argv[ 2 ] );
  size_t direction = atoi( argv[ 3 ] );
  if( dimensions <= direction ) {
    cout << "Error: Dimensions should be greater than direction. Given dimensions: " << dimensions <<
    ", direction: " << direction << endl;
    return( 0 );
  }
  Kernel krn = KernelType::NormalizedGabor( sigma, dimensions, direction );

  cout << "Kenel dimensions: " << krn.Dims( ) << ", elements: " << krn.size( ) << endl;
  for( size_t idx = 0; idx < krn.size( ); ++idx ) {
    cout << "displacement: ";
    for( size_t dms = 0; dms < krn.Dims( ); ++dms ) {
      cout << "dim[ " << dms << " ]: " << krn.Displacement( dms, idx ) << " ";
    }
    cout << "coefficient: " << krn.Value( idx ) << endl;
  }
  return( 0 );
}
