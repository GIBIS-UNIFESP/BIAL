/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2013/Nov/14 */
/* Content: Test file. */
/* Description: Implementation of multiclass labeling from segmentation result. */

#include "Image.hpp"
#include "FileImage.hpp"
#include "TransformEuclDist.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc != 4 ) {
    cout << "Usage: " << argv[ 0 ] << " <source label image> <target label image> <max_distance>" << endl;
    cout << "\t\t<max_distance>: Suggested: 2." << endl;
    return( 0 );
  }
  Image< int > src( Read< int >( argv[ 1 ] ) );
  Image< int > tgt( Read< int >( argv[ 2 ] ) );
  float thres = atof( argv[ 3 ] );
  if( src.Dims( ) != tgt.Dims( ) ) {
    cout << "Error: Input image dimensions do not match." << endl;
    return( 0 );
  }
  Image< float > src_dist( Transform::EDT( src ) );
  DEBUG_WRITE( src_dist, "src-edt", 0 );
  Image< float > tgt_dist( Transform::EDT( tgt ) );
  DEBUG_WRITE( tgt_dist, "tgt-edt", 0 );

  size_t TP = 0, TN = 0, FP = 0, FN = 0, EXT = 0;
  for( size_t pxl = 0; pxl < src.size( ); ++pxl ) {
    if( src[ pxl ] != 0 ) {
      if( tgt_dist[ pxl ] <= thres ) {
        ++TN;
      }
      else {
        ++FN;
      }
    }
    else if( tgt[ pxl ] != 0 ) {
      if( src_dist[ pxl ] <= thres ) {
        ++TP;
      }
      else {
        ++FP;
      }
    }
    else {
      ++EXT;
    }
  }
  cout << "TP: " << TP << ", FP: " << FP << ", TN: " << TN << ", FN: " << FN << ", EXT: " << EXT << endl;

  return( 0 );
}
