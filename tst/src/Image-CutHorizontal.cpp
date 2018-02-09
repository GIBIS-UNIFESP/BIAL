/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2016/Feb/01 */
/* Content: Cuts the input image into upper and lower halves. */
/* Description: */

#include "FileImage.hpp"
#include "Image.hpp"
#include "ImageResize.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc != 3 ) {
    cout << "Usage: " << argv[ 0 ] << " <input image 1> <output basename>" << endl;
    exit( -1 );
  }
  Image< int > img( Read< int >( argv[ 1 ] ) );
  size_t hpixels = img.size( 0 ) - 1;
  size_t vpixels = img.size( 1 ) - 1;
  size_t uvpixels = img.size( 1 ) / 2 - 1;
  size_t lvpixels = img.size( 1 ) / 2;
  Image< int > higher( ImageOp::Resize( img, { 0, 0 }, { hpixels, uvpixels } ) );
  Image< int > lower( ImageOp::Resize( img, { 0, lvpixels }, { hpixels, vpixels } ) );

  Write( lower, string( argv[ 2 ] ) + "-low.pgm", argv[ 1 ] );
  Write( higher, string( argv[ 2 ] ) + "-hig.pgm", argv[ 1 ] );
  return( 0 );
}
