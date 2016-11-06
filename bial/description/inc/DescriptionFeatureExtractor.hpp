#include "Color.hpp"
#include "DescriptionFeatureDetector.hpp"
#include "DescriptionFeatures.hpp"
#include "DescriptionParameterInterpreter.hpp"

#ifndef FEATUREEXTRACTOR_H
#define FEATUREEXTRACTOR_H

namespace Bial {

  static const int NIL = -1;
  static const int WHITE = 0;
  static const int GRAY = 1;
  static const int BLACK = 2;

  typedef Vector< std::tuple< int, int > > Curve;

  int Log( double value, double n );
  Image< int > Mbb( Image< int > img, Image< int > mask );
  Image< int > Mbb( Image< int > img );
  Curve ImageToCurve( Image< int > img, Image< int > mask );
  bool ValidContPoint( Image< int > bin, Adjacency L, Adjacency R, size_t p ); /* mover */
  Image< int > LabelContPixel( Image< int > img ); /* mover para segmentacion */
  double find_angle( int deltax, int deltay ); /* mover ? */
  Image< Color > RgbToHmmd( Image< Color > img );
  Image< Color > RgbToHsv( Image< Color > img );

  Adjacency FixAdj( const Adjacency &adj );
  Adjacency LeftSide( Adjacency adj );
  Adjacency RightSide( Adjacency adj );

  namespace AdjacencyType {
    Adjacency AdjacencyBox( int ncols, int nrows );
  }


  template< class I, class F >
  class FeatureExtractor {
protected:
    Vector < std::tuple < Image< I >, Image< int >> > detected;

public:
    FeatureExtractor( Vector < std::tuple < Image< I >, Image< int >> > detected );

    virtual void SetParameters( ParameterInterpreter *interpreter ) = 0;

    virtual std::string GetParameters( ParameterInterpreter *interpreter ) = 0;

    virtual Vector < Features < F >> Run( ) = 0;
  };

  template< class I, class F >
  FeatureExtractor< I, F >::FeatureExtractor( Vector < std::tuple < Image< I >, Image< int >> > detected ) {
    this->detected = detected;
  }
}


#endif
