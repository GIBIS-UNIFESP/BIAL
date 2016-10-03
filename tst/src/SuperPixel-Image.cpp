/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Author: Fábio Augusto Menocci Cappabianco */
/* Date: 2015/Sep/15 */
/* Version: 1.0.00 */
/* Content: Test file. */
/* Description: Test with superpixels. */

#include "Adjacency.hpp"
#include "Color.hpp"
#include "FileImage.hpp"
#include "Image.hpp"
#include "Superpixel.hpp"

using namespace Bial;
using namespace std;

int main( int argc, char *argv[ ] ) {
  
  if( argc < 4 ){
    cout << "usage: " << argv[ 0 ] << " <image> <superpixel_size> <compactness>" << endl;
    cout << "\t <superpixel_size>... The desired size of superpixels." << endl;
    cout << "\t <compactness>....... The relative importance between color similarity and spatial proximity." << endl;
    cout << "\t                      It can be in the range [1, 100]." << endl;
    exit( 0 );
  }

  string filename( argv[ 1 ] );
  string extension( File::ToLowerExtension( argv[ 1 ], 
                                            static_cast< size_t >( std::max( 0, static_cast< int >( filename.size( ) ) 
                                                                             - 8 ) ) ) );
  if( ( extension.rfind( ".scn" ) != std::string::npos ) || ( extension.rfind( ".nii" ) != std::string::npos ) ) {
    COMMENT( "Reading parameters.", 0 );
    Image< int > img( Read< int >( argv[ 1 ] ) );
    int sup_pixel_size = atoi( argv[ 2 ] );
    double compactness = atof( argv[ 3 ] );
    Image< int > label( Superpixel::SuperVoxel( img, sup_pixel_size, compactness ) );
    Write( label, "dat/label_slic.nii.gz", argv[ 1 ] );
    Write( img, "dat/border.nii.gz", argv[ 1 ] );
  }
  else {
    COMMENT( "Reading parameters.", 0 );
    Image< Color > img( Read< Color >( argv[ 1 ] ) );
    int sup_pixel_size = atoi( argv[ 2 ] );
    double compactness = atof( argv[ 3 ] );
    Image< int > label( Superpixel::SuperPixel( img, sup_pixel_size, compactness ) );
    Write( label, "dat/label_slic.pgm" );
    Write( img, "dat/spixels_slic.ppm" );
  }
  
  return( 0 );
}
