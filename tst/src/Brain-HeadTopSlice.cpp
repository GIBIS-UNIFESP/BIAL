/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2012/Sep/21 */
/* Content: Test file. */
/* Description: Test with Brain class. */

#include "BrainCOG.hpp"
#include "FileImage.hpp"
#include "Image.hpp"
#include "NiftiHeader.hpp"
#include "SegmentationOtsuThreshold.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  string orientation;
  if( argc < 2 ) {
    cout << "Usage: " << argv[ 0 ] << " <input image> [-v]" << endl;
    cout << "    -v: verbose mode." << endl;
    return( 0 );
  }

  COMMENT( "Reading image.", 0 );
  Image< int > scn( Read< int >( argv[ 1 ] ) );
  COMMENT( "Reading header.", 0 );
  NiftiHeader hdr( argv[ 1 ] );
  bool verbose = false;
  if( argc == 3 ) {
    verbose = true;
  }

  COMMENT( "Getting orientation.", 0 );
  orientation = hdr.Orientation( );
  size_t otsu_threshold = Segmentation::OtsuThreshold( scn );
  if( verbose ) {
    cout << "Orientation: " << orientation << endl;
    cout << "Otsu: " << otsu_threshold << endl;
    cout << "orientation.find_first_of( S )" << orientation.find_first_of( "S" ) << endl;
    cout << "orientation.find_first_of( I )" << orientation.find_first_of( "I" ) << endl;
  }

  COMMENT( "Computing uppercut.", 0 );
  size_t cut = Brain::HeadTopSlice( scn, orientation );

  cout << cut << endl;

  return( 0 );
}
