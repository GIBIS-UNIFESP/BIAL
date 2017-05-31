#ifndef SS_H
#define SS_H

#include "DescriptionFeatureExtractor.hpp"

#define INTERIOR 0
#define EXTERIOR 1
#define BOTH 2

#define WHITE 0
#define GRAY 1
#define BLACK 2

namespace Bial {

  typedef Vector< Features < double > > SSfeature;

  typedef Vector< std::tuple< int, int > > Curve;
  typedef Vector< std::tuple< double, double > > CurveDouble;


  class SS : public FeatureExtractor< int, double > {
private:

    size_t SAMPLES, MAXDIST, SIDE;

public:
    SS( FeatureDetector< int > *Fd );

    SS( const Vector< std::tuple< Image< int >, Vector< size_t > > > &detected );

    void SetParameters( ParameterInterpreter *interpreter );

    std::string GetParameters( ParameterInterpreter *interpreter );

    SSfeature Run( );
  };

}
#endif
