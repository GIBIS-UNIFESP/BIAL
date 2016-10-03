/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Author: Fábio Augusto Menocci Cappabianco */
/* Date: 2012/Sep/21 */
/* Version: 1.0.00 */
/* Content: Test file. */
/* Description: Test with Brain and Feature classes. */

#include "AdjacencyRound.hpp"
#include "ColorMedianFeature.hpp"
#include "Feature.hpp"
#include "FileImage.hpp"
#include "FuzzyCMeans.hpp"
#include "Image.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc < 3 ) {
    cout << "Usage: " << argv[ 0 ] << " <input color image> <output image> [<m>] [<epsilon>] [<clusters>]" <<
    endl;
    cout << "\tm: Exponent of cmeans membership function. Default: 2" << endl;
    cout << "\tepsilon: Acceptable threshold of minimization function. Default: 0.1" << endl;
    cout << "\tclusters: Number of clusters. Default: 3" << endl;
    exit( -1 );
  }
  Image< Color > img( Read< Color >( argv[ 1 ] ) );
  Adjacency adj_rel( AdjacencyType::Circular( 1.5 ) );
  Feature< int > feats = ColorMedianFeature< int >( img, adj_rel, 0.5 );

  float m = 2.0f;
  if( argc > 3 ) {
    m = atoi( argv[ 3 ] );
  }
  float epsilon = 0.1f;
  if( argc > 4 ) {
    epsilon = atof( argv[ 4 ] );
  }
  size_t clusters = 3;
  if( argc > 5 ) {
    clusters = atoi( argv[ 5 ] );
  }
  FuzzyCMeans< int > fcm( feats, clusters, m, epsilon );
  Matrix< double > membership( fcm.Run( false ) );
  Image< int > result( img.Dim( ), img.PixelSize( ) );
  for( size_t elm = 0; elm < membership.size( 1 ); ++elm ) {
    size_t best_clt = 0;
    for( size_t clt = 1; clt < clusters; ++clt ) {
      /* cout << "clt: " << clt << ", elm: " << elm << "." << endl; */
      if( membership( best_clt, elm ) < membership( clt, elm ) ) {
        best_clt = clt;
      }
    }
    result[ feats.Index( elm ) ] = best_clt + 1;
  }
  Write( result, argv[ 2 ], argv[ 1 ] );

  return( 0 );
}
