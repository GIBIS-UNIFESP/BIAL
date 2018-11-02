/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2018/Nov/02 */
/* Content: Test file. */
/* Description: Test . */

#include "FileImage.hpp"
#include "StatisticsAverage.hpp"
#include "StatisticsStdDev.hpp"
#include "Image.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc != 2 ) {
    cout << "Usage: " << argv[ 0 ] << " <input image>" << endl;
    exit( 1 );
  }
  Image< int > img( Read< int >( argv[ 1 ] ) );
  std::cout << "Minimum: " << img.Minimum( ) << std::endl;
  std::cout << "Maximum: " << img.Maximum( ) << std::endl;
  std::cout << "Average: " << Statistics::Average( img ) << std::endl;
  std::cout << "Standard Deviation: " << Statistics::StandardDeviation( img ) << std::endl;
  
  return( 0 );
}
