/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2016/Jan/12 */
/* Content: Program to insert inhomogeneity to 2D image. */
/* Description: It inserts a vertical inhomogeneity.  The top line remains the same, while the bottom line suffers the
   strongest effect. */

#include "Color.hpp"
#include "ColorChannel.hpp"
#include "ColorHSI.hpp"
#include "FileImage.hpp"
#include "Image.hpp"
#include "RealColor.hpp"

using namespace std;
using namespace Bial;

template< class D >
Image< D > InsertInhomogeneity( const Image< D > &img, float intensity ) {
  Image< D > res( img );
  size_t lines = img.size( 1 );
  size_t rows = img.size( 0 );
  const size_t GD_SZ = 10;
  
  COMMENT( "Slow decrement of intensity.", 0 );
  for( size_t y = lines / 2 - GD_SZ; y < lines / 2 + GD_SZ + 1; ++y ) {
    float line_intensity = 1.0f - ( 1.0f - intensity ) * 
      ( ( y + GD_SZ + 1 - lines / 2 ) / ( 2.0f * ( GD_SZ + 1.0f ) ) );
    //cout << "line: " << y << ", intensity: " << line_intensity << endl;
    for( size_t x = 0; x < rows; ++x ) {
      res( x, y ) *= line_intensity;
    }
  }
  COMMENT( "Shade region.", 0 );
  for( size_t y = lines / 2 + GD_SZ + 1; y < lines; ++y ) {
    //cout << "line: " << y << ", intensity: " << intensity << endl;
    for( size_t x = 0; x < rows; ++x ) {
      res( x, y ) *= intensity;
    }
  }
  
  return( res );
}


int main( int argc, char** argv ) {
  if( argc != 4 ) {
    cout << "Usage: " << argv[ 0 ] << " <input image> <output image> <inhomogeneity intensity>" << endl;
    cout << "\t<inhomogeneity intensity>: 0.0 to 1.0. The lower, the stronger is the inhomogeneity." << endl;
    return( 0 );
  }

  string ext = File::ToLowerExtension( argv[ 1 ] );
  float inhomogeneity = atof( argv[ 3 ] );
  if( ext == ".ppm" ) {
    Image< Color > img( Read< Color >( argv[ 1 ] ) );
    Image< RealColor > hsi( ColorSpace::ARGBtoAHSI( img ) );
    Image< float > intensity( ColorSpace::Channel< float >( hsi, 3 ) );
    Image< float > res( InsertInhomogeneity( intensity, inhomogeneity ) );
    for( size_t pxl = 0; pxl < img.size( ); ++pxl )
      hsi( pxl )( 3 ) = res( pxl );
    img = ColorSpace::AHSItoARGB( hsi );
    Write( img, argv[ 2 ] );
    return( 0 );
  }
  if( ext == ".pgm" ) {
    Image< int > img( Read< int >( argv[ 1 ] ) );
    Image< int > res( InsertInhomogeneity( img, inhomogeneity ) );
    Write( res, argv[ 2 ] );
    return( 0 );
  }
  cout << "Image formats: pgm or ppm." << endl;  
  return( 0 );
}
