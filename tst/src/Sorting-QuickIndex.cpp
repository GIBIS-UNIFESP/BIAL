/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2012/Sep/21 */
/* Content: Test file. */
/* Description: Test with Sorting class. */

#include "SortingSort.hpp"
#include "Vector.hpp"

using namespace std;
using namespace Bial;

int main( int, char** ) {

  Vector< float > data1( 100 );
  Vector< float > data2( 100 );
  for( size_t idx = 0; idx < data1.size( ); ++idx ) {
    data1[ idx ] = rand( );
    data2[ idx ] = rand( );
  }
  cout << "data1: " << endl;
  for( size_t idx = 0; idx < data1.size( ); ++idx ) {
    cout << data1[ idx ] << " ";
  }
  cout << endl << endl;

  cout << "data2: " << endl;
  for( size_t idx = 0; idx < data2.size( ); ++idx ) {
    cout << data2[ idx ] << " ";
  }
  cout << endl << endl;

  cout << "After sorting: " << endl << endl;

  Vector< size_t > order = Sorting::Sort( data1, true );

  Sorting::Sort( data2, order );

  cout << "data1: " << endl;
  for( size_t idx = 0; idx < data1.size( ); ++idx ) {
    cout << data1[ idx ] << " ";
  }
  cout << endl << endl;

  cout << "order: " << endl;
  for( size_t idx = 0; idx < order.size( ); ++idx ) {
    cout << order[ idx ] << " ";
  }
  cout << endl << endl;

  cout << "data2: " << endl;
  for( size_t idx = 0; idx < data2.size( ); ++idx ) {
    cout << data2[ idx ] << " ";
  }
  cout << endl;

  return( 0 );
}
