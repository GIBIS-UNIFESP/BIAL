/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2015/Jun/16 */
/* Content: Test file. */
/* Description: Test with Matrix class. */

#include "FileImage.hpp"
#include "Image.hpp"
#include "SegmentationOctants.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc != 3 ) {
    cout << "Usage: " << argv[ 0 ] << " <input image> <output basename>" << endl;
    return( 0 );
  }
  Image< int > image( Read< int >( argv[ 1 ] ) );

  Matrix< Image< int > > res_image( Segmentation::Octants( image ) );

  string extension = File::Extension( argv[ 1 ] );
  for( size_t img = 0; img < res_image.size( ); ++img ) {
    Write( res_image[ img ], argv[ 2 ] + string( "-" ) + to_string( img ) + extension );
  }
  return( 0 );
}
