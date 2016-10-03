/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2016/Feb/01 */
/* Content: Evaluation of edge detection algorithm accorging to Baddeley method.  */
/* Description: See Statistics.cpp and Statistics.hpp. Based on Euclidean distance of borders. */

#include "Adjacency.hpp"
#include "FileImage.hpp"
#include "Image.hpp"
#include "StatisticsBaddeley.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc != 3 ) {
    cout << "Usage: " << argv[ 0 ] << " <euclidean distance of source label> <euclidean distance of target label>" 
         << endl;
    return( 0 );
  }
  Image< int > src( Read< int >( argv[ 1 ] ) );
  Image< int > tgt( Read< int >( argv[ 2 ] ) );
  if( src.Dims( ) != tgt.Dims( ) ) {
    cout << "Error: Input image dimensions do not match." << endl;
    return( 0 );
  }
  
  cout << Statistics::Baddeley( src, tgt ) << endl;
  
  return( 0 );
}
