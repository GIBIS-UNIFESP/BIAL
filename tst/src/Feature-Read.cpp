/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2014/Jun/09 */
/* Content: Test file. */
/* Description: Test with feature vectors. */

#include "Common.hpp"
#include "Feature.hpp"
#include "FileFeature.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char *argv[] ) {
  if( argc != 2 ) {
    cout << "Usage: " << argv[ 0 ] << " <input opf file>" << endl;
    return( 0 );
  }
  Feature< float > feature = FileFeature::Read< float >( argv[ 1 ] );

  cout << "Features: " << endl;
  cout << feature << endl;

  return( 0 );
}
