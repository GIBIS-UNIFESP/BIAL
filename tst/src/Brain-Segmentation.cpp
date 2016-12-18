/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 14/10/2014 */
/* Content: Brain segmentation test. */
/* Description: Segmentation of the brain in CT image. */

#include "BrainSkullStripping.hpp"
#include "FileImage.hpp"
#include "Image.hpp"
#include "NiftiHeader.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc != 3 ) {
    cout << "usage: " << argv[ 0 ] << " <input> <output>" << endl;
    cout << "input: .............. original image." << endl;
    cout << "output: .............. segmented brain binary image." << endl;
    return( 0 );
  }
  /* Reading data contents */
  Image< float > scn( Read< float >( argv[ 1 ] ) );
  NiftiHeader nii( argv[ 1 ] );

  /* Getting image orientation */
  string orientation = nii.Orientation( );

  /* Executing and writing results. */
  Image< float > res( Brain::SkullStripping( scn, orientation ) );
  Write( res, argv[ 2 ], argv[ 1 ] );

  return( 0 );
}
