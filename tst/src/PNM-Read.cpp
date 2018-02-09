/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2013/Sep/26 */
/* Content: Test file. */
/* Description: Test reading functions for PNM files. */

#include "Color.hpp"
#include "FileImage.hpp"
#include "Image.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc != 2 ) {
    cout << "Usage: " << argv[ 0 ] << " <input file name>" << endl;
    exit( 0 );
  }
  COMMENT( "Reading file.", 0 );
  Image< int > img( Read< int >( argv[ 1 ] ) );

  COMMENT( "Printing data.", 0 );
  cout << img << endl;

  return( 0 );
}
