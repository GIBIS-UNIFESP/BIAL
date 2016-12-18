/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2015/Aug/31 */
/* Content: Test file. */
/* Description: Triangle mesh cleaning algorithm. */

#include "DrawShape.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc != 3 ) {
    cout << "Usage: " << argv[ 0 ] << " <input stl file> <output stl file>" << endl;
    return( 0 );
  }
  COMMENT( "Opening file.", 2 );

  TriangleMesh *mesh = TriangleMesh::ReadSTLB( argv[ 1 ] );

  mesh->ExportSTLB( argv[ 2 ] );

}
