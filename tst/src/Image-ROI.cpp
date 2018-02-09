/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2012/Sep/21 */
/* Content: Test file. */
/* Description: Test with Image class. */

#include "FileImage.hpp"
#include "Image.hpp"
#include "ImageResize.hpp"
#include "Vector.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc < 2 ) {
    cout << "Usage: " << argv[ 0 ] << " <input image> <output image> <dim_0 min> <dim_0 max> <dim_1 min> <dim_1 max>"
         << " <dim_2 min> <dim_2 max> ... " << endl;
    exit( -1 );
  }
  Image< int > img( Read< int >( argv[ 1 ] ) );
  Vector< size_t > mins( ( argc - 2 ) / 2 );
  Vector< size_t > maxs( ( argc - 2 ) / 2 );
  for( size_t dms = 0; dms < mins.size( ); ++dms ) {
    mins[ dms ] = atoi( argv[ 3 + dms * 2 ] );
    maxs[ dms ] = atoi( argv[ 4 + dms * 2 ] );
  }
  for( size_t dms = 0; dms < mins.size( ); ++dms ) {
    cout << "mins[ " << dms << " ]: " << mins[ dms ] << endl;
    cout << "maxs[ " << dms << " ]: " << maxs[ dms ] << endl;
  }
  Image< int > res( ImageOp::Resize( img, mins, maxs ) );

  Write( res, argv[ 2 ] );
  return( 0 );
}
