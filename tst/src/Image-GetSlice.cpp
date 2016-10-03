/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Author: Fábio Augusto Menocci Cappabianco */
/* Date: 2016/Feb/04 */
/* Version: 1.0.00 */
/* Content: Gets 2D slice from 3D image. */
/* Description: */

#include "Adjacency.hpp"
#include "FileImage.hpp"
#include "Image.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc != 5 ) {
    cout << "usage: " << argv[ 0 ] << " <input 3D image> <output 2D image> <slice number> <direction>" << endl;
    cout << "\t<direction>: 0, 1, or 2." << endl;
    return( 0 );
  }
  Image< int > img( Read< int >( argv[ 1 ] ) );
  size_t slice = atoi( argv[ 3 ] );
  size_t dir = atoi( argv[ 4 ] );
  size_t x_size = img.size( 0 );
  size_t y_size = img.size( 1 );
  size_t z_size = img.size( 2 );
  if( dir == 0 ) {
    Image< int > res( y_size, z_size );
    for( size_t z = 0; z < z_size; ++z ) {
      for( size_t y = 0; y < y_size; ++y ) {
        res( y, z ) = img( slice, y, z );
      }
    }
    Write( res, argv[ 2 ], argv[ 1 ] );
  }
  else if( dir == 1 ) {
    Image< int > res( x_size, z_size );
    for( size_t z = 0; z < z_size; ++z ) {
      for( size_t x = 0; x < x_size; ++x ) {
        res( x, z ) = img( x, slice, z );
      }
    }
    Write( res, argv[ 2 ], argv[ 1 ] );
  }
  else {
    Image< int > res( x_size, y_size );
    for( size_t y = 0; y < y_size; ++y ) {
      for( size_t x = 0; x < x_size; ++x ) {
        res( x, y ) = img( x, y, slice );
      }
    }
    Write( res, argv[ 2 ], argv[ 1 ] );
  }

  return( 0 );
}
