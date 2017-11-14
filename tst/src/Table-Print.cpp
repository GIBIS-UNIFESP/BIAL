/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2015/May/22 */
/* Content: Test file. */
/* Description: Test with Table class. */

#include "Matrix.hpp"
#include "Table.hpp"
#include "Vector.hpp"

using namespace std;
using namespace Bial;

int main( int, char** ) {

  COMMENT( "Creating matrix.", 0 );
  Matrix< float > M1( 10, 10 );
  for( size_t i = 0; i < M1.size( 1 ); ++i ) {
    for( size_t j = 0; j < M1.size( 0 ); ++j ) {
      M1[ i * M1.size( 0 ) + j ] = ( i + 10 + j ) * 10;
    }
  }
  COMMENT( "Creating table 1.", 0 );
  Table tbl1( M1 );

  cout << "Table 1: " << endl;
  cout << tbl1 << endl;

  COMMENT( "Creating table 2.", 0 );
  Table tbl2( M1, true, true );

  cout << endl << "Table 2: " << endl;
  cout << tbl2 << endl;

  COMMENT( "Creating table 3.", 0 );
  Table tbl3;
  Vector< string > name = { "nome1", "nome2", "nome3", "buuuu" };
  Vector< int > data1 = { 1, 4, 7, 9 };
  Vector< float > data2 = { 4.9f, 1.4f, 3.3f, 4.2f };
  tbl3.PushBack( name );
  tbl3.PushBack( data1 );
  tbl3.PushBack( data2 );

  cout << endl << "Table 3: " << endl;
  cout << tbl3 << endl;

  return( 0 );
}
