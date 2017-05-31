#include "DescriptionFeatureExtractor.hpp"

#ifndef BoxCounting_H
#define BoxCounting_H

namespace Bial {

  typedef Vector< Features < double > > BoxCountingfeature;

  class BoxCounting : public FeatureExtractor< int, double > {

public:
    BoxCounting( FeatureDetector< int > *Fd );

    BoxCounting( const Vector< std::tuple< Image< int >, Vector< size_t > > > &detected );

    void SetParameters( ParameterInterpreter *interpreter );

    std::string GetParameters( ParameterInterpreter *interpreter );

    BoxCountingfeature Run( );
  };

}

#endif
