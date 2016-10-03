/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2013/Sep/21 */
/* Content: Test file. */
/* Description: Test with OPF class. */

#include "Common.hpp"
#include "FileImage.hpp"
#include "Image.hpp"
#include "OPFClusterMatching.hpp"
#include "Vector.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char *argv[] ) {
  if( argc < 3 ) {
    cout << "Usage: " << argv[ 0 ] << " <input image 1> <input image 2> ..." << endl;
    return( 0 );
  }
  Vector< Image< int > > image;
  for( int img = 0; img < argc - 1; ++img ) {
    image.push_back( Read< int >( argv[ img + 1 ] ) );
  }
  float diff = OPF::ClusterMatchingError( image );

  cout << diff << endl;

  return( 0 );
}
