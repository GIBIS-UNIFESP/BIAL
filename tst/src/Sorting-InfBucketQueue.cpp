/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Author: Fábio Augusto Menocci Cappabianco */
/* Date: 2012/Sep/21 */
/* Version: 1.0.00 */
/* Content: Test file. */
/* Description: Test with BucketQueue class. */

#include "InfBucketQueue.hpp"
#include "Vector.hpp"

using namespace std;
using namespace Bial;

int main( int, char** ) {
  size_t size = 10000;
  Vector< int > vec( size, 0 );
  InfBucketQueue queue( size, 1, numeric_limits< int >::max( ) );

  cout << "Input vector: " << endl;
  vec[ 0 ] = numeric_limits< int >::max( );
  cout << setw( 8 ) << vec[ 0 ] << " ";
  for( size_t it = 1; it < vec.size( ); ++it ) {
    vec[ it ] = static_cast< int >( rand( ) ) / ( static_cast< int >( RAND_MAX ) / 1000 );
    cout << setw( 8 ) << vec[ it ] << " ";
    if( ( it + 1 ) % 15 == 0 ) {
      cout << endl;
    }
  }
  cout << endl;
  for( size_t it = 0; it < vec.size( ); ++it ) {
    queue.Insert( it, static_cast< llint >( vec[ it ] ) );
  }
  cout << "Buckets: " << queue.Buckets( ) << endl;

  queue.Update( 1, static_cast< llint >( vec[ 1 ] ), static_cast< llint >( 1000 ) );
  vec[ 1 ] = 1000;
  cout << "Buckets: " << queue.Buckets( ) << endl;

  queue.Update( 2, static_cast< llint >( vec[ 2 ] ), static_cast< llint >( numeric_limits< int >::max( ) ) );
  vec[ 2 ] = numeric_limits< int >::max( );
  cout << "Buckets: " << queue.Buckets( ) << endl;

  cout << "Sorted: " << endl;
  size_t it = 0;
  while( queue.Elements( ) != 0 ) {
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
