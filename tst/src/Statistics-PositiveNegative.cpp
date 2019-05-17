/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2019/Fev/08 */
/* Content: Test file. */
/* Description: Implementation of multiclass labeling from segmentation result. */

#include "FileImage.hpp"
#include "Image.hpp"
#include "SegmentationIntensityThreshold.hpp"
#include "StatisticsPosNeg.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( ( argc < 3 ) || ( argc > 4 ) ) {
    cout << "Usage: " << argv[ 0 ] << " <source label image> <target label image> [<label>]" << endl;
    cout << "\t\t<label>: label to be compared. 0 for comparing label by label. Default: 0." << endl;
    return( 0 );
  }
  Image< int > src( Read< int >( argv[ 1 ] ) );
  Image< int > tgt( Read< int >( argv[ 2 ] ) );
  if( src.Dims( ) != tgt.Dims( ) ) {
    cout << "Error: Input image dimensions do not match." << endl;
    return( 0 );
  }
  size_t label = 0;
  if( argc > 3 ) {
    label = atoi( argv[ 3 ] );
  }
  size_t tgt_max = tgt.Maximum( );
  /* for single label */
  if( label != 0 ) {
    cout << "Label: " << label << ":" << endl;
    Image< int > src_label( src );
    Image< int > tgt_label( tgt );
    Segmentation::IntensityThreshold( src_label, static_cast< int >( label ), static_cast< int >( label ) );
    Segmentation::IntensityThreshold( tgt_label, static_cast< int >( label ), static_cast< int >( label ) );
    size_t TP, TN, FP, FN;
    std::tie( TP, TN, FP, FN ) = Statistics::PositiveNegative( src_label, tgt_label );
    cout << "TP: " << TP << ", TN: " << TN << ", FP: " << FP << ", FN: " << FN << endl;
  }
  /* For multiple labels */
  else {
    for( size_t lbl = 0; lbl <= tgt_max; ++lbl ) {
      cout << "Label: " << lbl << ":" << endl;
      Image< int > src_label( src );
      Image< int > tgt_label( tgt );
      Segmentation::IntensityThreshold( src_label, static_cast< int >( lbl ), static_cast< int >( lbl ) );
      Segmentation::IntensityThreshold( tgt_label, static_cast< int >( lbl ), static_cast< int >( lbl ) );
      size_t TP, TN, FP, FN;
      std::tie( TP, TN, FP, FN ) = Statistics::PositiveNegative( src_label, tgt_label );
      cout << "TP: " << TP << ", TN: " << TN << ", FP: " << FP << ", FN: " << FN << endl;
    }
  }
  return( 0 );
}
