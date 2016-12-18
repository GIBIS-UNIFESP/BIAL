/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2012/Sep/21 */
/* Content: Test file. */
/* Description: Test with Brain and Feature classes. */

#include "AdjacencyRound.hpp"
#include "Feature.hpp"
#include "FileImage.hpp"
#include "FuzzyCMeans.hpp"
#include "Image.hpp"
#include "MedianFeature.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc < 4 ) {
    cout << "Usage: " << argv[ 0 ] << "<input image> <brain mask> <output image> [<m>] [<epsilon>] [<clusters>]" <<
    endl;
    cout << "\tm: Exponent of cmeans membership function. Default: 2" << endl;
    cout << "\tepsilon: Acceptable threshold of minimization function. Default: 0.1" << endl;
    cout << "\tclusters: Number of clusters. Default: 3" << endl;
    exit( -1 );
  }
 
  COMMENT( "Reading input data.", 0 );
  Image< int > scn( Read< int >( argv[ 1 ] ) );
  Image< int > mask( Read< int >( argv[ 2 ] ) );
  float m = 2.0f;
  if( argc > 4 )
    m = atoi( argv[ 4 ] );
  float epsilon = 0.1f;
  if( argc > 5 )
    epsilon = atof( argv[ 5 ] );
  size_t clusters = 3;
  if( argc > 6 )
    clusters = atoi( argv[ 6 ] );
  COMMENT( "Running cmeans.", 0 );
  Feature< int > feats = MedianFeature( scn, mask, AdjacencyType::HyperSpheric( 1.5, scn.Dims( ) ), 0.34 );
  FuzzyCMeans< int > fcm( feats, clusters, m, epsilon );
  Matrix< double > membership( fcm.Run( false ) );
  Image< int > result( mask );
  COMMENT( "Assigning final label.", 0 );
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
  COMMENT( "Writing result.", 0 );
  Write( result, argv[ 3 ], argv[ 1 ] );
  return( 0 );
}
