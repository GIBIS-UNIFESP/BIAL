/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2013/Nov/01 */
/* Content: Test file. */
/* Description: Test with Segmentation class. */

#include "FileImage.hpp"
#include "Image.hpp"
#include "SegmentationGeoStar.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( ( argc < 4 ) || ( argc > 6 ) ) {
    cout << "Usage: " << argv[ 0 ] << " <input image> <output image> <seeds> [<alpha> <beta>]" << endl;
    cout << "\t\talpha: Oriented edges strength. 0.0 (non-oriented) to 1.0 (extremely oriented). Default: 0.5" << endl;
    cout << "\t\tbeta: Geodesic distance force. 0.0 (geodesic distance) to 4.0 (sum function). Default: 0.5" << endl;
    return( 0 );
  }
  /* Reading input data. */
  float alpha = 0.5;
  float beta = 0.5;
  if( argc > 4 ) {
    alpha = atof( argv[ 4 ] );
  }
  if( argc > 5 ) {
    beta = atof( argv[ 5 ] );
  }
  Image< int > img( Read< int >( argv[ 1 ] ) );
  Image< int > seed_img( Read< int >( argv[ 3 ] ) );
  if( img.Dims( ) != seed_img.size( 0 ) - 1 ) {
    cout << "Error: seed and image dimensions do not match." << endl;
    return( 0 );
  }
  /* Defining object and background seeds */
  Vector< size_t > obj_seeds;
  Vector< size_t > bkg_seeds;
  size_t seed_dimensions = img.Dims( );
  for( size_t idx = 0; idx < seed_img.size( ); idx += ( seed_dimensions + 1 ) ) {
    size_t factor = 1.0;
    size_t pxl = seed_img[ idx ];
    for( size_t dms = 1; dms < seed_dimensions; ++dms ) {
      factor *= img.size( dms - 1 );
      pxl += seed_img[ idx + dms ] * factor;
    }
    if( seed_img[ idx + seed_dimensions ] == 0 ) {
      bkg_seeds.push_back( pxl );
    }
    else {
      obj_seeds.push_back( pxl );
    }
  }
  /* Running segmentation */
  Image< int > res( Segmentation::OrientedGeodesicStar( img, obj_seeds, bkg_seeds, -alpha, beta ) );

  /* Writing results. */
  Write( res, argv[ 2 ], argv[ 1 ] );

  return( 0 );
}
