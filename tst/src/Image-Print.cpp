/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Author: Fábio Cappabianco */
/* Date: 2015/Dec/22 */
/* Version: 1.0.00 */
/* Content: Test file. */
/* Description: Test . */

#include "FileImage.hpp"
#include "Image.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc != 2 ) {
    cout << "Usage: " << argv[ 0 ] << " <input image>" << endl;
    exit( 1 );
  }
  Image< int > img( Read< int >( argv[ 1 ] ) );
  
  cout << img << endl;

  return( 0 );
}
