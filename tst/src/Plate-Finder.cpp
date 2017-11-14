/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Author: Fábio Augusto Menocci Cappabianco */
/* Date: 2012/Jun/27 */
/* Version: 1.0.00 */
/* Content: Test file. */
/* Description: Test with Filteirng class. */

#include "ColorRGB.hpp"
#include "DrawCircle.hpp"
#include "DrawPoint.hpp"
#include "FileImage.hpp"
#include "FilteringGaussian.hpp"
#include "HoughCircle.hpp"
#include "Image.hpp"
#include "Matrix.hpp"

using namespace std;
using namespace Bial;

int BestCircle( const Vector< Circle > &circles, double imgRadius, Point imgCenter ) {
  double maxDistIC = imgRadius * 0.5, maxDiffIR = imgRadius * 0.5;
  Vector< double > diffToImgRadius( circles.size( ) );
  Vector< double > distToImgCtr( circles.size( ) );
  for( size_t i = 0; i < circles.size( ); ++i ) {
    Point center( { circles[ i ].center[ 0 ], circles[ i ].center[ 1 ] } );
    int radius = circles[ i ].radius;
    diffToImgRadius[ i ] = std::abs( radius - imgRadius ) / ( imgRadius * 2.0 / 0.9 );
    distToImgCtr[ i ] = std::sqrt( std::pow( center.coordinate[ 0 ] - imgCenter.coordinate[ 0 ], 2 ) +
                                   std::pow( center.coordinate[ 1 ] - imgCenter.coordinate[ 1 ], 2 ) ) /
                        ( imgRadius * 2.0 / 0.9 );
    maxDiffIR = std::max( diffToImgRadius[ i ], maxDistIC );
    maxDistIC = std::max( distToImgCtr[ i ], maxDistIC );
  }
  int bestCircle = -1;
  double bestProbability = 1.0;
  for( size_t i = 0; i < circles.size( ); ++i ) {
    diffToImgRadius[ i ] /= maxDiffIR;
    distToImgCtr[ i ] /= maxDistIC;
    double prob = ( diffToImgRadius[ i ] + distToImgCtr[ i ] ) / 2.0;
    if( prob <= bestProbability ) {
      bestCircle = i;
      bestProbability = prob;
    }
  }
  return( bestCircle );
}

int main( int argc, char **argv ) {
  if( argc != 7 ) {
    cout << "Usage: " << argv[ 0 ] <<
    " <input_file> <output_file> <canny_threshold> <detection_threshold> <canny_levels> <clean_edges>" << endl;
    return( 0 );
  }
  COMMENT( "Otsu's higher threshold.", 0 );
  double high_thres = atof( argv[ 3 ] );
  int detec_thres = atoi( argv[ 4 ] );
  int canny_levels = atoi( argv[ 5 ] );
  int clean_edges = atoi( argv[ 6 ] );

  Image< Color > src( Read< Color >( argv[ 1 ] ) );
  COMMENT( "Converting to gray.", 0 );
  Image< int > src_gray( ColorSpace::ARGBtoGraybyBrightness< int >( src ) );

  COMMENT( "Reduce the noise so we avoid false circle detection.", 0 );
  Image< int > smooth( Filtering::Gaussian( src_gray, 9.0, 2.0 ) );

  double imgRadius = std::min( src.size( 0 ), src.size( 1 ) ) / 2.0 * 0.9;
  Vector< Circle > circles( Hough::HoughCircles( smooth, 1.0, src_gray.size( 1 ) / 8, imgRadius * 0.70,
                                                 imgRadius * 1.25, high_thres, detec_thres, canny_levels, clean_edges ) );

  /* COMMENT( "Apply the Hough Transform to find the circles.", 0 ); */
  Point imgCenter( { src.size( 0 ) / 2.0f, src.size( 1 ) / 2.0f } );
  Image< int > res( src_gray.Dim( ) );
  int bestCircle = BestCircle( circles, imgRadius, imgCenter );
  Image< int > circles_img( src_gray.Dim( ) );

  std::cout << "Circles Found: " << circles.size( ) << std::endl;
  COMMENT( "Circles Found: " << circles.size( ), 0 );
  if( bestCircle != -1 ) {
    circles[ bestCircle ].filled = true;
    circles[ bestCircle ].Draw( res );
  }
  Write( res, argv[ 2 ] );
  /* Image< int >::Show( argv[ 2 ] ); */
  for( bestCircle = 0; bestCircle < static_cast< int >( circles.size( ) ); ++bestCircle ) {
    circles[ bestCircle ].filled = false;
    circles[ bestCircle ].Draw( circles_img );
    circles[ bestCircle ].Print( std::cout );
  }
  DEBUG_WRITE( circles_img, "circles", 0 );

  return( 0 );
}
