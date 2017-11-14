/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2012/Sep/21 */
/* Content: Test file. */
/* Description: Test with BucketQueue class. */

#include "GrowingBucketQueue.hpp"
#include "Vector.hpp"

using namespace std;
using namespace Bial;

int main( int, char** ) {
  size_t size = 10000;
  Vector< float > vec( size, 0.0 );
  GrowingBucketQueue queue( size, 0.01, false, false );

  cout << "Buckets: " << queue.Buckets( ) << endl;

  cout << "Input vector: " << endl;
  for( size_t it = 0; it < vec.size( ); ++it ) {
    vec[ it ] = static_cast< float >( rand( ) ) / static_cast< float >( RAND_MAX ) * 1000.0;
    cout << setw( 8 ) << vec[ it ] << " ";
    if( ( it + 1 ) % 15 == 0 ) {
      cout << endl;
    }
  }
  cout << endl;
  for( size_t it = 0; it < vec.size( ); ++it ) {
    queue.Insert( it, vec[ it ] );
  }
  cout << "Buckets: " << queue.Buckets( ) << endl;

  queue.Update( 0, vec[ 0 ], 1000.0 );
  vec[ 0 ] = 1000.0;
  cout << "Buckets: " << queue.Buckets( ) << endl;

  queue.Update( 0, vec[ 0 ], 1001.0 );
  vec[ 0 ] = 1001.0;
  cout << "Buckets: " << queue.Buckets( ) << endl;

  cout << "Sorted: " << endl;
  size_t it = 0;
  while( !queue.Empty( ) ) {
    int index = queue.Remove( );
    queue.Finished( index );
    cout << setw( 8 ) << vec[ index ] << " ";
    if( ( it + 1 ) % 15 == 0 ) {
      cout << endl;
    }
    it++;
  }
  cout << endl;

  return( 0 );
}
