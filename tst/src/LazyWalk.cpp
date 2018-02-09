/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2015/Jun/09 */
/* Content: Test file. */
/* Description: Test with LazyWalk function. */

#include "EdgeLazyWalk.hpp"
#include "FileImage.hpp"
#include "Image.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc < 4 ) {
    cout << "Usage: " << argv[ 0 ] << " <input image> <output basename> <seed file> [<mask image>]" << endl;
    cout << "\t\t<output basename>: Basename completed with extensions for cost and predecessor images." << endl;
    cout <<
    "\t\t<seed file>: Text file containing the coordinates of the pixels, each dimension separated by space " <<
    "and one pixel per line. The file begins with a number that gives the number of dimensions." << endl;
    cout << "\t\t<mask image>: Binary image. It is different of zero where the algorithm should run." << endl;
    return( 0 );
  }
  COMMENT( "Reading input image.", 0 );
  Image< int > img( Read< int >( argv[ 1 ] ) );

  COMMENT( "Creating seeds.", 0 );
  Vector< bool > seed( img.Size( ) );
  seed.Set( false );
  IFile ifile;
  ifile.open( argv[ 3 ] );
  COMMENT( "Checking seed dimensions.", 1 );
  size_t seed_dims;
  ifile >> seed_dims;
  COMMENT( "seed dims: " << seed_dims, 3 );
  if( seed_dims != img.Dims( ) ) {
    BIAL_ERROR( "Seed dimensions and image dimensions do not match. Seed: " + to_string( seed_dims ) +
                ", image: " + to_string( img.Dims( ) ) + "." );
    return( 0 );
  }
  COMMENT( "Reading seeds.", 1 );
  do {
    size_t x, y, z;
    ifile >> x;
    if( ifile.eof( ) ) {
      break;
    }
    ifile >> y;
    if( seed_dims == 3 ) {
      if( ifile.eof( ) ) {
        break;
      }
      ifile >> z;
      COMMENT( x << ", " << y << ", " << z, 3 );
      seed( img.Position( x, y, z ) ) = true;
    }
    else {
      COMMENT( x << ", " << y, 3 );
      seed( img.Position( x, y ) ) = true;
    }
  } while( !ifile.eof( ) );
  ifile.close( );

  COMMENT( "Running LazyWalk.", 0 );
  Image< int > cost;
  Image< int > pred;
  if( argc == 5 ) {
    COMMENT( "Creating mask.", 0 );
    Image< int > msk( Read< int >( argv[ 4 ] ) );
    std::tie( cost, pred ) = Edge::LazyWalk( img, msk, seed );

    COMMENT( "Seting pixels for frendly displaying.", 0 );
    for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {
      if( msk[ pxl ] == 0.0f ) {
        cost[ pxl ] = 0;
        pred[ pxl ] = 0;
      }
      if( pred[ pxl ] < 0 ) {
        pred[ pxl ] = pxl;
      }
    }
  }
  else {
    std::tie( cost, pred ) = Edge::LazyWalk( img, seed );

    COMMENT( "Seting pixels for frendly displaying.", 0 );
    for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {
      if( pred[ pxl ] < 0 ) {
        pred[ pxl ] = pxl;
      }
    }
  }
  Write( cost, string( argv[ 2 ] ) + "-cst" + ( img.Dims( ) == 2 ? ".pgm" : ".nii.gz" ), argv[ 1 ] );
  Write( pred, string( argv[ 2 ] ) + "-prd" + ( img.Dims( ) == 2 ? ".pgm" : ".nii.gz" ), argv[ 1 ] );

  return( 0 );
}
