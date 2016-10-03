/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2012/Sep/21 */
/* Content: Test file. */
/* Description: Test with Brain and Intensity classes. */

#include "FileImage.hpp"
#include "Histogram.hpp"
#include "Image.hpp"
#include "Signal.hpp"
#include "SignalEmptyBins.hpp"
#include "SignalMedianFilter.hpp"
#include "SignalOtsuThreshold.hpp"
#include "Vector.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  string orientation;
  if( argc < 2 ) {
    cout << "Usage: " << argv[ 0 ] << " <input image>" << endl;
    exit( -1 );
  }
  Image< int > scn( Read< int >( argv[ 1 ] ) );
  Signal hist = SignalType::Histogram( scn );
  Signal nonzero_hist = SignalOp::RemoveEmptyBins( hist );
  Vector< size_t > hist_index = SignalOp::MapBins( nonzero_hist, hist );
  nonzero_hist[ 0 ] = 0; /* Remove background. */
  Signal median_hist = SignalOp::MedianFilter( static_cast< const Signal >( nonzero_hist ), 
                                               5 + ( nonzero_hist.Bins( ) / 1000 ) );
  nonzero_hist[ 0 ] = hist[ hist.Bin( nonzero_hist.MinimumFrequency( ) ) ]; /* Insert backgound. */
  /* Find left cut coordinates */
  size_t left_cut = SignalOp::OtsuThreshold( hist );
  cout << left_cut << " ";
  /* Compute amplitude. */
  int amplitude = median_hist.MaximumFrequency( left_cut );
  /* Compute right cut. */
  size_t it = median_hist.Bins( ) - 2;
  for( ; median_hist[ it ] <= 0.1 * amplitude; --it ) {
  }
  size_t right_cut = it;
  cout << hist_index[ right_cut ] << endl;

  return( 0 );
}
