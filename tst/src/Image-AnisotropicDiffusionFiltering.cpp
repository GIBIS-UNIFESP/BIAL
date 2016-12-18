/** 
 *   BIAL
 *   Method:     Nonlinear Anisotropic Diffusion
 */

#include "FileImage.hpp"
#include "Image.hpp"

#define INTEGRATION_CONSTANT 1.0

using namespace std;
using namespace Bial;

float gradient( const Image< float > &src, const size_t &x, const size_t &y, const size_t &z ) {
  float grad = 6.0 * src( x, y, z );
  grad -= src( x + 1, y, z );
  grad -= src( x - 1, y, z );
  grad -= src( x, y + 1, z );
  grad -= src( x, y - 1, z );
  grad -= src( x, y, z + 1 );
  grad -= src( x, y, z - 1 );
  return( grad );
}

float diffusion_function( const float &grad, const float &kappa ) {
  return( exp( pow( -( abs( grad ) / kappa ), 2 ) ) );
}

float flow( const Image< float > &src, const float &kappa, const size_t &x, const size_t &y, const size_t &z ) {
  float grad = gradient( src, x, y, z );
  float diffusion_strength = 0.0f;

  diffusion_strength += src( x + 1, y, z ) - 2.0 * src( x, y, z ) + src( x - 1, y, z );
  diffusion_strength += src( x, y + 1, z ) - 2.0 * src( x, y, z ) + src( x, y - 1, z );
  diffusion_strength += src( x, y, z + 1 ) - 2.0 * src( x, y, z ) + src( x, y, z - 1 );

  return( diffusion_strength * diffusion_function( grad, kappa ) );
}

void adf( Image< float > &src, const int iterations, const float kappa, Image< float > &tgt ) {
  for( int i = 0; i < iterations; ++i ) {
    /*Difusão*/
    for( size_t x = 1; x < src.size( 0 ) - 1; ++x ) {
      for( size_t y = 1; y < src.size( 1 ) - 1; ++y ) {
        for( size_t z = 1; z < src.size( 2 ) - 1; ++z ) {
          tgt( x, y, z ) = src( x, y, z ) + INTEGRATION_CONSTANT*flow( src, kappa, x, y, z );
        }
      }
    }
    /*Atualização*/
    swap( src, tgt );
  }
  swap( src, tgt );
}

int main( int argc, char **argv ) {
  if( argc != 5 ) {
    cout << "Usage: " << argv[ 0 ] << " <input image> <iterations> <kappa> <output image>" << endl;
    return( 0 );
  }
  else {
    Image< float > src( Read< float >( argv[ 1 ] ) );
    Image< float > tgt( src );
    float iterations = atoi( argv[ 2 ] );
    float kappa = atof( argv[ 3 ] );
    adf( src, iterations, kappa, tgt );
    Write( tgt, argv[ 4 ] );
    return( 0 );
  }
}
