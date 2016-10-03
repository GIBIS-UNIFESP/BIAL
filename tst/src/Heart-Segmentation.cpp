/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 14/10/2014 */
/* Content: Heart segmentation test. */
/* Description: Segmentation of the heart in CT image. */

#include "FileImage.hpp"
#include "HeartSegmentation.hpp"
#include "Image.hpp"
#include "NiftiHeader.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc != 3 ) {
    cout << "usage: " << argv[ 0 ] << " <input> <output>" << endl;
    cout << "input: .............. original image." << endl;
    cout << "output: .............. segmented heart binary image." << endl;
    return( 0 );
  }
  /* Reading data contents */
  Image< float > scn( Read< float >( argv[ 1 ] ) );
  NiftiHeader nii( argv[ 1 ] );

  /* Getting image orientation */
  string orientation = nii.Orientation( );

  /* Executing and writing results. */
  Image< float > res( Heart::Segmentation( scn, orientation ) );
  Write( res, argv[ 2 ], argv[ 1 ] );

  return( 0 );
}
