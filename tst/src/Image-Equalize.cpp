/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2012/Sep/21 */
/* Content: Test file. */
/* Description: Test with Image and Signal classes. */

#include "FileImage.hpp"
#include "Histogram.hpp"
#include "Image.hpp"
#include "Signal.hpp"
#include "SignalEqualize.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc != 3 ) {
    cout << "usage: " << argv[ 0 ] << " <input> <output>" << endl;
    return( 0 );
  }
  Image< int > img( Read< int >( argv[ 1 ] ) );
  Signal eql = SignalType::Histogram( img );
  SignalOp::Equalize( eql );
  Image< int > res( img );
  for( size_t val = 0; val < eql.size( ); ++val ) {
    eql[ val ] = round( eql[ val ] );
  }
  for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {
    res[ pxl ] = static_cast< int >( eql[ img[ pxl ] ] );
  }
  Write( res, argv[ 2 ], argv[ 1 ] );

  return( 0 );
}
