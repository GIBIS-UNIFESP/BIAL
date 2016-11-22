#ifndef TAR_H
#define TAR_H

#include "DescriptionFeatureExtractor.hpp"

#define NIL -1
#define WHITE 0
#define GRAY 1
#define BLACK 2

namespace Bial {

  typedef Vector < Features < double >> TARfeature;

  typedef Vector< std::tuple< int, int > > Curve;

  class TAR : public FeatureExtractor< int, double > {
private:

    size_t SAMPLES;

    static Vector< double > ComputeScale( Curve contour, int n, int ts );

    static Vector< Vector< double > > ComputeSignature( Curve contour, int n );

    static double ShapeComplexity( Vector< Vector< double > > tar, int samples );


public:
    TAR( FeatureDetector< int > *Fd );

    TAR( Vector < std::tuple < Image< int >, Image< int >> > detected );


    void SetParameters( ParameterInterpreter *interpreter );

    std::string GetParameters( ParameterInterpreter *interpreter );

    TARfeature Run( );
  };
}

#endif
