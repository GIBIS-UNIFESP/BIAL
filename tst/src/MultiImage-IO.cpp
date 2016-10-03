/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2016/Sep/29 */
/* Content: IO tests with MultiImage. */
/* Description: */

#include "ColorHSI.hpp"
#include "FileImage.hpp"
#include "MultiImage.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  COMMENT( "Reading int image and printing results.", 0 );
  MultiImage mimg;
  cout << "Initial type: " << to_string( static_cast< int >( mimg.Type( ) ) ) << endl;
  mimg = Read< int >( "res/cat1.pgm" );
  cout << "Image 1 type: " << to_string( static_cast< int >( mimg.Type( ) ) ) << endl;
  Image< int > &ref_img( mimg.IntImage( ) );
  cout << "Image 1 dims: " << ref_img.Dim( ) << endl;
  COMMENT( "Reading float image and printing results.", 0 );
  mimg = Read< float >( "res/0.nii.gz" );
  cout << "Image 2 type: " << to_string( static_cast< int >( mimg.Type( ) ) ) << endl;
  Image< float > &ref_img2( mimg.FltImage( ) );
  cout << "Image 2 dims: " << ref_img2.Dim( ) << endl;
  COMMENT( "Reading color image and printing results.", 0 );
  mimg = Read< Color >( "res/579.ppm" );
  cout << "Image 3 type: " << to_string( static_cast< int >( mimg.Type( ) ) ) << endl;
  Image< Color > &ref_img3( mimg.ClrImage( ) );
  cout << "Image 3 dims: " << ref_img3.Dim( ) << endl;
  COMMENT( "Reading real color image and printing results.", 0 );
  mimg = ColorSpace::ARGBtoAHSI( ref_img3 );
  cout << "Image 4 type: " << to_string( static_cast< int >( mimg.Type( ) ) ) << endl;
  Image< RealColor > &ref_img4( mimg.RclImage( ) );
  cout << "Image 4 dims: " << ref_img4.Dim( ) << endl;
  return( 0 );
}
