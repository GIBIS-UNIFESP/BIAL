#ifndef TAR_H
#define TAR_H

#include "DescriptionFeatureExtractor.hpp"

#define NIL -1
#define WHITE 0
#define GRAY 1
#define BLACK 2

namespace Bial {

  typedef Vector< Features < double > > TARfeature;

  typedef Vector< std::tuple< int, int > > Curve;

  class TAR : public FeatureExtractor< int, double > {
private:

    size_t SAMPLES;

    static Vector< double > ComputeScale( const Curve &contour, size_t n, size_t ts );

    static Vector< Vector< double > > ComputeSignature( const Curve &contour, size_t n );

    static double ShapeComplexity( const Vector< Vector< double > > &tar, size_t samples );


public:
    TAR( FeatureDetector< int > *Fd );

    TAR( const Vector< std::tuple< Image< int >, Vector< size_t > > > &detected );

    void SetParameters( ParameterInterpreter *interpreter );

    std::string GetParameters( ParameterInterpreter *interpreter );

    TARfeature Run( );
  };
}

#endif
