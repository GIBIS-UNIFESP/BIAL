/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2015/Sep/15 */
/* Content: Test file. */
/* Description: Test with superpixels. */

#include "FileImage.hpp"
#include "Image.hpp"
#include "Matrix.hpp"
#include "SegmentationIntensityThreshold.hpp"
#include "StatisticsDice.hpp"
#include "Superpixel.hpp"

using namespace Bial;
using namespace std;

int main( int argc, char *argv[ ] ) {
  
  if( argc < 6 ){
    cout << "usage: " << argv[ 0 ] << " <brain image> <gt> <result> <supervoxel_size> <compactness>" << endl;
    cout << "\t <supervoxel_size>... The desired size of supervoxel." << endl;
    cout << "\t <compactness>....... The relative importance between color similarity and spatial proximity." << endl;
    cout << "\t                      It can be in the range [1, 100]." << endl;
    exit( 0 );
  }

  COMMENT( "Reading parameters.", 0 );
  Image< int > img( Read< int >( argv[ 1 ] ) );
  Image< int > gt( Read< int >( argv[ 2 ] ) );
  int sup_pixel_size = atoi( argv[ 4 ] );
  double compactness = atof( argv[ 5 ] );

  COMMENT( "Running slic.", 0 );
  Image< int > label( Superpixel::SuperVoxel( img, sup_pixel_size, compactness ) );

  COMMENT( "Computing the best label according to gt.", 0 );
  Matrix< size_t > gt_label( label.Maximum( ) + 1, 4 );
  for( size_t vxl = 0; vxl < label.size( ); ++vxl )
    ++gt_label( label[ vxl ], gt[ vxl ] );
  
  COMMENT( "Applying with the best label.", 0 );
  for( size_t vxl = 0; vxl < label.size( ); ++vxl ) {
    if( ( gt_label( label[ vxl ], 0 ) > gt_label( label[ vxl ], 1 ) ) &&
        ( gt_label( label[ vxl ], 0 ) > gt_label( label[ vxl ], 2 ) ) &&
        ( gt_label( label[ vxl ], 0 ) > gt_label( label[ vxl ], 3 ) ) )
      label[ vxl ] = 0;
    else if( ( gt_label( label[ vxl ], 1 ) > gt_label( label[ vxl ], 2 ) ) &&
             ( gt_label( label[ vxl ], 1 ) > gt_label( label[ vxl ], 3 ) ) )
      label[ vxl ] = 1;
    else if( gt_label( label[ vxl ], 2 ) > gt_label( label[ vxl ], 3 ) )
      label[ vxl ] = 2;
    else
      label[ vxl ] = 3;
  }

  COMMENT( "Writing result.", 0 );
  Write( label, argv[ 3 ], argv[ 1 ] );

  COMMENT( "Computing metrics.", 0 );
  cout << "Dice: ";
  for( int tissue = 1; tissue < 4; ++tissue ) {
    Image< int > tissue_img( label );
    Image< int > tissue_gth( gt );
    Segmentation::IntensityThreshold( tissue_img, tissue, tissue );
    Segmentation::IntensityThreshold( tissue_gth, tissue, tissue );
    float dice = Statistics::Dice( tissue_img, tissue_gth );
    cout << tissue << " : " << dice << ", ";
  }
  cout << endl;

  return( 0 );
}
