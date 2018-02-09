/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Author: Fábio Augusto Menocci Cappabianco */
/* Date: 2017/Mar/28 */
/* Version: 1.0.00 */
/* Content: Tests fourier transform. */
/* Description: Test with Bit class. */

#include "Fourier.hpp"
#include "FileImage.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char *argv[ ] ) {

  if( argc != 3 ) {
    cout << "Usage: " << argv[ 0 ] << " <input image> <output image>" << endl;
    return( 0 );
  }

  COMMENT( "Reading image.", 0 );
  Image< int > img( Read< int >( argv[ 1 ] ) );
  COMMENT( "Running FFT.", 0 );
  Image< bial_complex > c_img( img );
  Image< bial_complex > f_img( FFT( c_img ) );
  COMMENT( "Reading IFFT.", 0 );
  Image< bial_complex > r_img( FFT( f_img, false ) );
  Image< int > res_pad( r_img );
  Image< int > res( img.Dim( ) );
  ZeroUnPad( res_pad, res );
  
  Write( res, argv[ 2 ], argv[ 1 ] );
  
  return( 0 );
}
