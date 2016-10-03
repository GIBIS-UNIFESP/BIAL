/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Author: Fábio Augusto Menocci Cappabianco */
/* Date: 2012/Sep/21 */
/* Version: 1.0.00 */
/* Content: Test file. */
/* Description: Test with OPFClustering class. */

#include "Common.hpp"
#include "Feature.hpp"
#include "FileImage.hpp"
#include "Image.hpp"
#include "OPFSpectralClustering.hpp"
#include "SpatialFeature.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( ( argc < 3 ) || ( argc > 5 ) ) {
    cout << "Usage: " << argv[ 0 ] << " <input image> <output image> [<kmin>, <kmax>]" << endl;
    cout << "\tkmin, kmax: Default values = 0.01, 0.3. Suggested values: 0.0 < kmin <= kmax <= 1.0." << endl;
    exit( 1 );
  }
  COMMENT( "Fixed number sequence.", 0 );
  Common::Randomize( false );

  float kmin = 0.01;
  float kmax = 0.3;
  if( argc > 3 ) {
    kmin = atof( argv[ 3 ] );
  }
  if( argc > 4 ) {
    kmax = atof( argv[ 4 ] );
  }
  if( kmin > kmax ) {
    cout << "Error: kmin can not be greater than kmax. Given: kmin = " << kmin << ", kmax = " << kmax << "." << endl;
    return( 0 );
  }
  Image< int > img( Read< int >( argv[ 1 ] ) );
  Feature< int > feats( SpatialFeature( img ) );
  Image< int > label( img );
  label.Set( 0 );

  OPF::SpectralClustering( feats, kmin, kmax );
  for( size_t elm = 0; elm < feats.Elements( ); ++elm ) {
    label[ feats.Index( elm ) ] = feats.Label( elm );
  }
  for( size_t idx = 0; idx < label.size( ); ++idx ) {
    if( img[ idx ] != 0 ) {
      label[ idx ]++;
    }
  }
  Write( label, argv[ 2 ], argv[ 1 ] );

  return( 0 );
}
