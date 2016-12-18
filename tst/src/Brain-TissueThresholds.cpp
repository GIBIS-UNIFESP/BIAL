/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2012/Sep/21 */
/* Content: Test file. */
/* Description: Test with Brain class. */

#include "BrainIntensities.hpp"
#include "FileImage.hpp"
#include "Image.hpp"

using namespace std;
using namespace Bial;
using namespace Bial::Brain;

int main( int argc, char **argv ) {
  string orientation;
  if( argc < 2 ) {
    cout << "Usage: " << argv[ 0 ] << " <Skull stripped brain image>" << endl;
    exit( -1 );
  }
  Image< int > scn( Read< int >( argv[ 1 ] ) );

  BrainIntensities tissues( scn );

  cout << "minimun: " << tissues.minimum << ", first_peak: " << tissues.first_peak << ", first_valley: " <<
  tissues.first_valley << ", second_peak: " << tissues.second_peak
       << ", second_valley: " << tissues.second_valley << ", third_peak: " << tissues.third_peak << ", maximum: " <<
  tissues.maximum << "." << endl;
  return( 0 );
}
