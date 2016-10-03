/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Author: Fábio Augusto Menocci Cappabianco */
/* Date: 2016/Feb/04 */
/* Version: 1.0.00 */
/* Content: Rotates image in 90 degrees clockwise. */
/* Description: */

#include "Adjacency.hpp"
#include "FileImage.hpp"
#include "Image.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc != 3 ) {
    cout << "usage: " << argv[ 0 ] << " <input image> <output image>" << endl;
    return( 0 );
  }
  Image< int > img( Read< int >( argv[ 1 ] ) );
  size_t x_size = img.size( 0 );
  size_t y_size = img.size( 1 );
  size_t dims = img.Dims( );
  if( dims == 2 ) {
    Image< int > res( y_size, x_size );
    for( size_t y = 0; y < y_size; ++y ) {
      for( size_t x = 0; x < x_size; ++x ) {
        res( y, x_size - 1 - x ) = img( x, y );
      }
    }
    Write( res, argv[ 2 ], argv[ 1 ] );
  }
  else {
    size_t z_size = img.size( 2 );
    Image< int > res( y_size, x_size, z_size );
    for( size_t z = 0; z < z_size; ++z ) {
      for( size_t y = 0; y < y_size; ++y ) {
        for( size_t x = 0; x < x_size; ++x ) {
          res( y, x_size - 1 - x, z ) = img( x, y, z );
        }
      }
    }
    Write( res, argv[ 2 ], argv[ 1 ] );
  }

  return( 0 );
}
