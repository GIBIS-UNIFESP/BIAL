/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2013/Nov/01 */
/* Content: Test file. */
/* Description: Test with Segmentation class. */

#include "FileImage.hpp"
#include "Image.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc != 4 ) {
    cout << "Usage: " << argv[ 0 ] << " <object input image> <background input image> <seeds output image>" << endl;
    return( 0 );
  }
  /* Reading input data. */
  Image< int > obj( Read< int >( argv[ 1 ] ) );
  Image< int > bkg( Read< int >( argv[ 2 ] ) );
  if( obj.Dims( ) != bkg.Dims( ) ) {
    cout << "Object and background image dimensions do not match." << endl;
    return( 0 );
  }
  size_t dims = obj.Dims( );
  size_t obj_size = 0;
  for( size_t elm = 0; elm < obj.size( ); ++elm ) {
    if( obj( elm ) != 0 )
      ++obj_size;
  }
  size_t bkg_size = 0;
  for( size_t elm = 0; elm < bkg.size( ); ++elm ) {
    if( bkg( elm ) != 0 )
      ++bkg_size;
  }
  Image< int > res( dims + 1, obj_size + bkg_size );

  size_t seed = 0;
  for( size_t pxl = 0; pxl < obj.size( ); ++pxl ) {
    if( obj[ pxl ] != 0 ) {
      for( size_t dms = 0; dms < dims; ++dms ) {
        res[ seed++ ] = obj.Coordinate( pxl, dms );
      }
      res[ seed++ ] = 1;
    }
    if( bkg[ pxl ] != 0 ) {
      for( size_t dms = 0; dms < dims; ++dms ) {
        res[ seed++ ] = bkg.Coordinate( pxl, dms );
      }
      res[ seed++ ] = 0;
    }
  }
  /* Writing results. */
  Write( res, argv[ 3 ] );

  return( 0 );
}
