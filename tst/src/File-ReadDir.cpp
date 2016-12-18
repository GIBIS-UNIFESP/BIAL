/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2012/Jun/29 */
/* Content: Test file. */
/* Description: Test with Matrix class. */

#include "File.hpp"
#include "Vector.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( ( argc < 2 ) || ( argc > 3 ) ) {
    cout << "Usage: <directory_name> [<recursive>]" << endl;
    cout << "\t\t<recursive>: 1 - true." << endl;
    return( 0 );
  }
  bool recursive = false;
  if( ( argc > 2 ) && ( atoi( argv[ 2 ] ) == 1 ) ) {
    recursive = true;
  }
  string dir_name( argv[ 1 ] );
  Vector< string > list = Directory::List( dir_name, recursive );

  cout << "Number of files found: " << list.size( ) << endl;
  for( size_t idx = 0; idx < list.size( ); ++idx ) {
    cout << list[ idx ] << endl;
  }
  return( 0 );
}
