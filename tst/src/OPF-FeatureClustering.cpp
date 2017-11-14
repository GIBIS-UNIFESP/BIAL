/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Author: Fábio Augusto Menocci Cappabianco */
/* Date: 2013/Sep/21 */
/* Version: 1.0.00 */
/* Content: Test file. */
/* Description: Test with Transform class. */

#include "Common.hpp"
#include "Feature.hpp"
#include "FileFeature.hpp"
#include "OPFSpectralClustering.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char *argv[] ) {
  if( argc != 3 ) {
    cout << "Usage: " << argv[ 0 ] << " <input file> <expected number of clusters>" << endl;
    cout << "\t<maximum error>: In [0.0, 1.0]. 0.0 for perfect segmentation and 1.0 for no error detection." << endl;
    return( 0 );
  }
  /* Fixed number sequence */
  Common::Randomize( true );

  Feature< int > feature = FileFeature::Read< int >( argv[ 1 ] );
  size_t clusters = atoi( argv[ 2 ] );

  size_t nlabels = OPF::SpectralClustering( feature, clusters );

  cout << "Labels: " << nlabels << endl;
  cout << feature.Label( ) << endl;

  return( 0 );
}
