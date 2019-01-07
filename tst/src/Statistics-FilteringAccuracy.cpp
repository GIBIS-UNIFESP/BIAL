/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2018/Nov/30 */
/* Content: Test file. */
/* Description: Implementation of multiclass labeling from segmentation result. */

#include "FileImage.hpp"
#include "Image.hpp"
#include "StatisticsEntropyDifference.hpp"
#include "StatisticsIQI.hpp"
#include "StatisticsMeanSquareError.hpp"
#include "StatisticsPSNR.hpp"
#include "StatisticsQILV.hpp"
#include "StatisticsSSIM.hpp"


using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( ( argc < 3 ) || ( argc > 5 ) ) {
    cout << "Usage: " << argv[ 0 ] << " <filtered image> <reference image> [<metric>]" << endl;
    cout << "\t\t<metric>: 0 - All, 1 - Entropy Difference, 2 - Image Quality Index, 3 - Mean Square Error, 4 - Peak Signal to Noise Ration, 5 - Quality Index based on Local Variance, 6 - Structural Similarity, Default: 0." << endl;
    return( 0 );
  }
  Image< int > src( Read< int >( argv[ 1 ] ) );
  Image< int > ref( Read< int >( argv[ 2 ] ) );
  if( src.Dims( ) != ref.Dims( ) ) {
    cout << "Error: Input image dimensions do not match." << endl;
    return( 0 );
  }
  size_t metric = 0;
  if( argc > 3 )
    metric = atoi( argv[ 3 ] );
  if( metric > 4 ) {
    cout << "Error: Invalid metric." << endl;
    return( 0 );
  }
  if( metric == 0 ) {
    cout << "ED: " << Statistics::EntropyDifference( src, ref );
    cout << "; IQI: " << Statistics::IQI( src, ref );
    cout << "; MSE: " << Statistics::MeanSquareError( src, ref );
    cout << "; PSNR: " << Statistics::PSNR( src, ref );
    cout << "; QILV: " << Statistics::QILV( src, ref );
    cout << "; SSIM: " << Statistics::SSIM( src, ref ) << endl;
  }
  if( metric == 1 )
    cout << "Entropy Difference: " << Statistics::EntropyDifference( src, ref ) << endl;
  if( metric == 2 )
    cout << "IQI: " << Statistics::IQI( src, ref ) << endl;
  if( metric == 3 )
    cout << "MSE: " << Statistics::MeanSquareError( src, ref ) << endl;
  if( metric == 4 )
    cout << "PSNR: " << Statistics::PSNR( src, ref ) << endl;
  if( metric == 5 )
    cout << "QILV: " << Statistics::QILV( src, ref ) << endl;
  if( metric == 6 )
    cout << "SSIM: " << Statistics::SSIM( src, ref ) << endl;
  return( 0 );
}
