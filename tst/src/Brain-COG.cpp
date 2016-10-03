/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2012/Sep/21 */
/* Content: Test file. */
/* Description: Test with Brain class. */

#include "BrainCOG.hpp"
#include "FileImage.hpp"
#include "Image.hpp"
#include "Integral.hpp"
#include "IntensityGlobals.hpp"
#include "MRIModality.hpp"
#include "NiftiHeader.hpp"
#include "SegmentationIntensityThreshold.hpp"
#include "SegmentationOtsuThreshold.hpp"
#include "Vector.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc != 4 ) {
    cout << "usage: " << argv[ 0 ] << " <input> <cog_type> <modality>" << endl;
    cout << "input: .............. original image" << endl;
    cout << "cog_type: ........... 0 - simple mask centroid" << endl;
    cout << "          ........... 1 - weighted mean centroid" << endl;
    cout << "          ........... 2 - considers top of the head and its height" << endl;
    cout << "          ........... 3 - top of the head followed by haar filters" << endl;
    cout << "modality: ........... 0 - T1 (Default)" << endl;
    cout << "          ........... 1 - T2" << endl;
    cout << "          ........... 2 - PD" << endl;
    cout << "output: ............. stripped mask" << endl;
    return( 0 );
  }
  Image< float > scn( Read< float >( argv[ 1 ] ) );
  int cog_type = atoi( argv[ 2 ] );
  MRIModality modality = static_cast< MRIModality >( atoi( argv[ 3 ] ) + 1 );
  if( modality != MRIModality::T1 ) {
    float T1 = Segmentation::OtsuThreshold( scn );
    float T2 = scn.Maximum( );
    Image< float > bin( scn );
    Segmentation::IntensityThreshold( bin, T1, T2 );
    Intensity::Complement( scn, bin );
  }
  Vector< size_t > COG;
  switch( cog_type ) {
      case 0: {
        COG = Brain::COGbyIntensityThreshold( scn );
      break;
    }
      case 1: {
        COG = Brain::COGbyWeightedIntensity( scn );
      break;
    }
      case 2: {
        NiftiHeader nii( argv[ 1 ] );
        string orientation = nii.Orientation( );
        COG = Brain::COGbyHeadTopDistance( scn, orientation );
        break;
    }
      default: { /* case 3: */
        NiftiHeader nii( argv[ 1 ] );
        string orientation = nii.Orientation( );
        COG = Brain::COGbyIntegralImage( scn, orientation, modality );
      break;
    }
  }
  cout << COG[ 0 ] << " " << COG[ 1 ] << " " << COG[ 2 ] << endl;

  return( 0 );
}
