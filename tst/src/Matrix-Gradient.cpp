/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2015/Jun/09 */
/* Content: Test file. */
/* Description: Test with Matrix class to read from file. */

#include "AdjacencyRound.hpp"
#include "Edge.hpp"
#include "FilteringMedian.hpp"
#include "GradientMorphological.hpp"
#include "Histogram.hpp"
#include "Image.hpp"
#include "IntensityGlobals.hpp"
#include "Signal.hpp"
#include "SignalAccumulate.hpp"
#include "SignalNormalize.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc != 3 ) {
    cout << "Usage: " << argv[ 0 ] << " <input matrix> <output matrix>" << endl;
    return( 0 );
  }
  COMMENT( "Reading input Matrix.", 0 );
  Matrix< float > mat( argv[ 1 ] );

  Image< float > img( mat );

  img = Filtering::Median( img, 1.5 );

  Image< float > msk( img );
  for( size_t pxl = 0; pxl < msk.size( ); ++pxl ) {
    if( msk[ pxl ] <= -0.0f ) {
      msk[ pxl ] = 0;
    }
    else {
      msk[ pxl ] = 1;
    }
  }
  Adjacency spheric( AdjacencyType::HyperSpheric( 1.0, img.Dims( ) ) );
  COMMENT( "Computing morphological gradient: ", 0 );
  Image< float > res( Gradient::Morphological( img, msk, spheric ) );

  COMMENT( "Computing histogram to remove outliers.", 0 );
  Signal< float > hist( SignalType::Histogram( res, 0.1 ) );
  SignalOp::Normalize( hist );
  SignalOp::Accumulate( hist );
  size_t max_bin = 0;
  size_t min_bin = 0;
  COMMENT( "Locating outilier. Histogram size: " << hist.size( ), 0 );
  for( ; max_bin < hist.size( ); ++max_bin ) {
    if( hist[ min_bin ] < 0.01 ) {
      ++min_bin;
    }
    if( hist[ max_bin ] > 0.99 ) {
      break;
    }
  }
  COMMENT( "Eliminating small intensities.", 0 );
  for( size_t pxl = 0; pxl < res.size( ); ++pxl ) {
    if( res[ pxl ] < hist.Data( min_bin ) ) {
      res[ pxl ] = hist.Data( min_bin );
    }
    if( res[ pxl ] > hist.Data( max_bin ) ) {
      res[ pxl ] = hist.Data( max_bin );
    }
  }
  if( res.Minimum( ) < 0.0f ) {
    res -= res.Minimum( );
  }
  COMMENT( "Complement.", 0 );
  Intensity::Complement( res );

  /* cout << res.DataMatrix( ); */

  res *= 255.0f / res.Maximum( );
  res.Write( argv[ 2 ] );

  return( 0 );
}
