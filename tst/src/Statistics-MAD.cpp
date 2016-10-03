/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Author: Fábio Augusto Menocci Cappabianco */
/* Date: 2012/Sep/21 */
/* Version: 1.0.00 */
/* Content: Test file. */
/* Description: Test with statistics. */

#include "FileImage.hpp"
#include "Image.hpp"
#include "StatisticsMAD.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc != 2 ) {
    cout << "Usage: " << argv[ 0 ] << " <input file>" << endl;
    return( 0 );
  }
  Image< int > img( Read< int >( argv[ 1 ] ) );
  float mad = Statistics::MedianAbsoluteDeviation( img );
  cout << "Median absolute deviation: " << mad << endl;
  cout << "Expected standard deviation: " << 1.4826 * mad << endl;

  return( 0 );
}
