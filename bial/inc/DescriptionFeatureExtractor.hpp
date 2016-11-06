#include "FeatureDetector.h"
#include "Features.h"
#include "ParameterInterpreter.h"

#ifndef FEATUREEXTRACTOR_H
#define FEATUREEXTRACTOR_H

namespace Bial {
  typedef Vector< std::tuple< int, int > > Curve;

  template< class I, class F >
  class FeatureExtractor {
protected:
    Vector < tuple < Image< I >, Image< int >> > detected;

    static int Log( double value, double n );
    static Image< int > Mbb( Image< int > img, Image< int > mask );
    static Curve ImageToCurve( Image< int > img, Image< int > mask );
    static bool ValidContPoint( Image< int > bin, Adjacency L, Adjacency R, int p ); /* mover */
    static Image< int > LabelContPixel( Image< int > img ); /* mover para segmentacion */
    static inline double find_angle( int deltax, int deltay ); /* mover ? */
    static Image< Color > RgbToHmmd( Image< Color > img );
    static Image< Color > RgbToHsv( Image< Color > img );

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
