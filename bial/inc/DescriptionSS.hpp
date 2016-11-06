#ifndef SS_H
#define SS_H

#include "FeatureExtractor.h"

#define INTERIOR 0
#define EXTERIOR 1
#define BOTH 2

#define WHITE 0
#define GRAY 1
#define BLACK 2

namespace Bial {

  typedef Vector < Features < double >> SSfeature;

  typedef Vector< tuple< int, int > > Curve;
  typedef Vector< tuple< double, double > > CurveDouble;


  class SS : public FeatureExtractor< int, double > {
private:

    size_t SAMPLES, MAXDIST, SIDE;

public:
    SS( FeatureDetector< int > *Fd );

    SS( Vector < std::tuple < Image< int >, Image< int >> > detected );

    void SetParameters( ParameterInterpreter *interpreter );

    std::string GetParameters( ParameterInterpreter *interpreter );

    SSfeature Run( );
  };

}
#endif
