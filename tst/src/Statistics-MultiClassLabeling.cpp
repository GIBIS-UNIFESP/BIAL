/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Author: Fábio Augusto Menocci Cappabianco */
/* Date: 2013/Nov/14 */
/* Version: 1.0.00 */
/* Content: Test file. */
/* Description: Implementation of multiclass labeling from segmentation result. */

#include "AdjacencyRound.hpp"
#include "AdjacencyIterator.hpp"
#include "FileImage.hpp"
#include "Image.hpp"
#include "Vector.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc != 3 ) {
    cout << "Usage: " << argv[ 0 ] << " <input label image> <output multi-label image>" << endl;
    return( 0 );
  }
  Image< int > img( Read< int >( argv[ 1 ] ) );
  Image< int > res( img );

  /* Finding existing labels. */
  Vector< size_t > label( img.Maximum( ) + 1, 0 );
  for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {
    if( label[ img[ pxl ] ] == 0 ) {
      label[ img[ pxl ] ] = 1;
    }
  }
  /* setting new labels in increasing order from 1 to nlabels. */
  size_t nlabels = 1;
  for( size_t lbl = 0; lbl < label.size( ); ++lbl ) {
    if( label[ lbl ] != 0 ) {
      label[ lbl ] = nlabels;
    }
    ++nlabels;
  }
  /* computing multi-label image */
  Adjacency adj = AdjacencyType::Ellipsoid( 1.01 / img.PixelSize( 0 ), 1.01 / img.PixelSize( 1 ), 
                                            1.01 / img.PixelSize( 2 ) );
  res.Set( 0 );
  for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {
    Vector< size_t > used_labels( nlabels + 1, false );
    for( AdjacencyIterator idx = begin( adj, img, pxl ); *idx != img.size( ); ++idx ) {
      size_t adj_pxl = *idx;
      used_labels[ label[ img[ adj_pxl ] ] ] = true;
    }
    for( size_t lbl = 0; lbl < used_labels.size( ); ++lbl ) {
      if( used_labels[ lbl ] == true ) {
        res[ pxl ] += pow( 2, lbl - 1 );
      }
    }
  }
  Write( res, argv[ 2 ], argv[ 1 ] );

  return( 0 );
}
