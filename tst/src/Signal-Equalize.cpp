/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2012/Sep/21 */
/* Content: Test file. */
/* Description: Test with signals. */

#include "FileImage.hpp"
#include "Histogram.hpp"
#include "Image.hpp"
#include "Signal.hpp"
#include "SignalEqualize.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc != 2 ) {
    cout << "Usage: " << argv[ 0 ] << " <input file>" << endl;
    return( 0 );
  }
  Image< int > img( Read< int >( argv[ 1 ] ) );
  Signal hist = SignalType::Histogram( img );
  SignalOp::Equalize( hist );
  cout << "Equalized histogram: " << endl;
  hist.Print( cout );

  return( 0 );
}
