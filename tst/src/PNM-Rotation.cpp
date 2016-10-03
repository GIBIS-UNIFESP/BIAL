/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Author: Fábio Augusto Menocci Cappabianco */
/* Date: 2013/Sep/29 */
/* Version: 1.0.00 */
/* Content: Test file. */
/* Description: Test PNM image geometric operation. */

#include "Color.hpp"
#include "FileImage.hpp"
#include "Image.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char *argv[] ) {
  if( argc != 4 ) {
    cout << "Usage: " << argv[ 0 ] << " <input image> <angle> <resulting image>"
         << endl;
    return( 0 );
  }
  Image< Color > img( Read< Color >( argv[ 1 ] ) );
  double angle = atof( argv[ 2 ] );

  Image< Color > res( img );
  for( size_t v = 0; v < res.size( 1 ); ++v ) {
    for( size_t u = 0; u < res.size( 0 ); ++u ) {
      double f = u * cos( angle ) - v*sin( angle );
      double g = u * sin( angle ) + v*cos( angle );
      size_t x = round( f );
      size_t y = round( g );
      /* Verifica se coordenadas x e y ficam fora do dominio da imagem. */
      if( ( f < 0.0 ) || ( x >= res.size( 0 ) ) || ( g < 0.0 ) || ( y >= res.size( 1 ) ) ) {
        res( u, v ).Value( 0 );
      }
      else {
        res( u, v ) = img( x, y );
      }
    }
  }
  Write( res, argv[ 3 ] );

  return( 0 );
}
