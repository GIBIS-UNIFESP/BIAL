/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Author: Fábio Augusto Menocci Cappabianco */
/* Date: 2012/Sep/13 */
/* Version: 1.0.00 */
/* Content: Test file. */
/* Description: Test with RandomQueue class. */

#include "RandomQueue.hpp"
#include "Vector.hpp"

using namespace std;
using namespace Bial;

int main( int, char** ) {
  RandomQueue queue( 5 );
  Vector< float > vec( 50, 0.0 );

  cout << "Input vector: " << endl;
  for( size_t it = 0; it < vec.size( ); ++it ) {
    vec[ it ] = static_cast< float >( rand( ) ) / static_cast< float >( RAND_MAX ) * 100.0;
    cout << setw( 8 ) << vec[ it ] << " ";
    if( ( it + 1 ) % 15 == 0 ) {
      cout << endl;
    }
  }
  cout << endl;

  cout << "Is full? " << queue.Full( ) << endl;
  cout << "Medians: " << endl;
  for( size_t it = 0; it < vec.size( ); ++it ) {
    cout << "Insert: " << setw( 8 ) << vec[ it ] << "." << endl;
    queue.Insert( vec[ it ] );
    cout << "Median: " << setw( 8 ) << queue.Median( );
    cout << ". Queue: ";
    queue.Print( cout );
  }
  cout << "Is full? " << queue.Full( ) << endl;
  while( !queue.Empty( ) ) {
    cout << "Removing: " << setw( 8 ) << queue.Remove( ) << "." << endl;
    if( !queue.Empty( ) ) {
      cout << "Median: " << setw( 8 ) << queue.Median( );
      cout << ". Queue: ";
      queue.Print( cout );
    }
  }
  cout << "Medians: " << endl;
  for( size_t it = 0; it < vec.size( ); ++it ) {
    cout << "Insert: " << setw( 8 ) << vec[ it ] << "." << endl;
    queue.Insert( vec[ it ] );
    cout << "Median: " << setw( 8 ) << queue.Median( );
    cout << ". Queue: ";
    queue.Print( cout );
  }
  return( 0 );
}
