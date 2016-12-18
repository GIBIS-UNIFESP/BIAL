/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2013/Nov/14 */
/* Content: Test file. */
/* Description: Implementation of multiclass labeling from segmentation result. */

#include "FileImage.hpp"
#include "Image.hpp"
#include "SegmentationIntensityThreshold.hpp"
#include "StatisticsDice.hpp"
#include "StatisticsKappa.hpp"
#include "StatisticsJaccard.hpp"
#include "StatisticsObsAgree.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( ( argc < 3 ) || ( argc > 5 ) ) {
    cout << "Usage: " << argv[ 0 ] << " <source label image> <target label image> [<metric> [<label>]]" << endl;
    cout << "\t\t<metric>: 0 - Dice, 1 - Jaccard, 2 - Observed Agreement, 3 - Kappa, 4 - All. Default: 4." << endl;
    cout << "\t\t<label>: label to be compared. 0 for comparing label by label. Default: 0." << endl;
    return( 0 );
  }
  Image< int > src( Read< int >( argv[ 1 ] ) );
  Image< int > tgt( Read< int >( argv[ 2 ] ) );
  if( src.Dims( ) != tgt.Dims( ) ) {
    cout << "Error: Input image dimensions do not match." << endl;
    return( 0 );
  }
  size_t metric = 4;
  if( argc > 3 ) {
    metric = atoi( argv[ 3 ] );
  }
  if( metric > 4 ) {
    cout << "Error: Invalid metric." << endl;
    return( 0 );
  }
  size_t label = 0;
  if( argc > 4 ) {
    label = atoi( argv[ 4 ] );
  }
  size_t tgt_max = tgt.Maximum( );
  /* for single label */
  if( label != 0 ) {
    cout << "Label: " << label << ":" << endl;
    Image< int > src_label( src );
    Image< int > tgt_label( tgt );
    Segmentation::IntensityThreshold( src_label, static_cast< int >( label ), static_cast< int >( label ) );
    Segmentation::IntensityThreshold( tgt_label, static_cast< int >( label ), static_cast< int >( label ) );
    if( metric == 4 ) {
      cout << "Dice: " << Statistics::Dice( src_label, tgt_label ) << endl;
      cout << "Jaccard: " << Statistics::Jaccard( src_label, tgt_label ) << endl;
      cout << "Observed Agreement: " << Statistics::ObservedAgreement( src_label, tgt_label ) << endl;
      cout << "Kappa: " << Statistics::Kappa( src_label, tgt_label ) << endl;
    }
    else if( metric == 0 ) {
      cout << "Dice: " << Statistics::Dice( src_label, tgt_label ) << endl;
    }
    else if( metric == 1 ) {
      cout << "Jaccard: " << Statistics::Jaccard( src_label, tgt_label ) << endl;
    }
    else if( metric == 2 ) {
      cout << "Observed Agreement: " << Statistics::ObservedAgreement( src_label, tgt_label ) << endl;
    }
    else {
      cout << "Kappa: " << Statistics::Kappa( src_label, tgt_label ) << endl;
    }
  }
  /* For multiple labels */
  else {
    for( size_t lbl = 0; lbl <= tgt_max; ++lbl ) {
      cout << "Label: " << lbl << ":" << endl;
      Image< int > src_label( src );
      Image< int > tgt_label( tgt );
      Segmentation::IntensityThreshold( src_label, static_cast< int >( lbl ), static_cast< int >( lbl ) );
      Segmentation::IntensityThreshold( tgt_label, static_cast< int >( lbl ), static_cast< int >( lbl ) );
      if( metric == 4 ) {
        cout << "Dice: " << Statistics::Dice( src_label, tgt_label ) << endl;
        cout << "Jaccard: " << Statistics::Jaccard( src_label, tgt_label ) << endl;
        cout << "Observed Agreement: " << Statistics::ObservedAgreement( src_label, tgt_label ) << endl;
        cout << "Kappa: " << Statistics::Kappa( src_label, tgt_label ) << endl;
      }
      else if( metric == 0 ) {
        cout << "Dice: " << Statistics::Dice( src_label, tgt_label ) << endl;
      }
      else if( metric == 1 ) {
        cout << "Jaccard: " << Statistics::Jaccard( src_label, tgt_label ) << endl;
      }
      else if( metric == 2 ) {
        cout << "Observed Agreement: " << Statistics::ObservedAgreement( src_label, tgt_label ) << endl;
      }
      else {
        cout << "Kappa: " << Statistics::Kappa( src_label, tgt_label ) << endl;
      }
      if( lbl != tgt_max ) {
        cout << endl;
      }
    }
  }
  return( 0 );
}
