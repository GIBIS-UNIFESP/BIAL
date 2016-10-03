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
  if( argc != 4 ) {
    cout << "Usage: " << argv[ 0 ] << " <input image> <output image> <dimension>" << endl;
    cout << "\t\t<dimension>: 0=x, 1=y, 2=z" << endl;
    return( 0 );
  }
  Image< int > img( Read< int >( argv[ 1 ] ) );
  size_t x_size = img.size( 0 );
  size_t y_size = img.size( 1 );
  size_t dims = img.Dims( );
  size_t flip = atoi( argv[ 3 ] );
  if( dims == 2 ) {
    Image< int > res( x_size, y_size );
    for( size_t y = 0; y < y_size; ++y ) {
      for( size_t x = 0; x < x_size; ++x ) {
        if( flip == 0 )
          res( x_size - 1 - x, y ) = img( x, y );
        else
          res( x, y_size - 1 - y ) = img( x, y );
      }
    }
    Write( res, argv[ 2 ], argv[ 1 ] );
  }
  else {
    size_t z_size = img.size( 2 );
    Image< int > res( x_size, y_size, z_size );
    for( size_t z = 0; z < z_size; ++z ) {
      for( size_t y = 0; y < y_size; ++y ) {
        for( size_t x = 0; x < x_size; ++x ) {
          if( flip == 0 )
            res( x_size - 1 - x, y, z ) = img( x, y, z );
          else if( flip == 1 )
            res( x, y_size - 1 - y, z ) = img( x, y, z );
          else
            res( x, y, z_size - 1 - z ) = img( x, y, z );
        }
      }
    }
    Write( res, argv[ 2 ], argv[ 1 ] );
  }

  return( 0 );
}
