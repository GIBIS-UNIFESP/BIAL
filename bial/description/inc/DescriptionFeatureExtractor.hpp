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
  Image< int > Mbb( const Image< int > &img, const Image< int > &mask );
  Image< int > Mbb( const Image< int > &img );
  Curve ImageToCurve( const Image< int > &img, const Vector< size_t > &mask );
  bool ValidContPoint( const Image< int > &bin, const Adjacency &L, const Adjacency &R, size_t p ); /* mover */
  Image< int > LabelContPixel( const Image< int > &img ); /* mover para segmentacion */
  double find_angle( int deltax, int deltay ); /* mover ? */
  Image< Color > RgbToHmmd( const Image< Color > &img );
  Image< Color > RgbToHsv( const Image< Color > &img );

  Adjacency FixAdj( const Adjacency &adj );
  Adjacency LeftSide( const Adjacency &adj );
  Adjacency RightSide( const Adjacency &adj );

  namespace AdjacencyType {
    Adjacency AdjacencyBox( int ncols, int nrows );
  }


  template< class I, class F >
  class FeatureExtractor {
protected:
    const Vector< std::tuple< Image< I >, Vector< size_t > > > &detected;

public:
    FeatureExtractor( const Vector< std::tuple< Image< I >, Vector< size_t > > > &detected );

    virtual void SetParameters( ParameterInterpreter *interpreter ) = 0;

    virtual std::string GetParameters( ParameterInterpreter *interpreter ) = 0;

    virtual Vector< Features < F > > Run( ) = 0;
  };

  template< class I, class F >
  FeatureExtractor< I, F >::FeatureExtractor( const Vector< std::tuple< Image< I >, Vector< size_t > > > &detected ) :
    detected( detected ) {
  }

}

#endif
