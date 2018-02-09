#include "Common.hpp"
#include "Feature.hpp"
#include "FileFeature.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char *argv[] ) {
  if( argc != 2 ) {
    cout << "Usage: " << argv[ 0 ] << " <output opf file>" << endl;
    return( 0 );
  }
  Feature< float > feature = FileFeature::Read< float >( "res/features.opf" );

  FileFeature::Write( feature, argv[ 1 ] );

  return( 0 );
}
