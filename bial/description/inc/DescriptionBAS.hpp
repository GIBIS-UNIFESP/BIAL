#include "DescriptionFeatureExtractor.hpp"

#ifndef BAS_H
#define BAS_H

namespace Bial {

  typedef Vector< Features < double > > BASfeature;

  class BAS : public FeatureExtractor< int, double > {
private:

    size_t SAMPLES;

public:
    BAS( FeatureDetector< int > *Fd );

    BAS( const Vector< std::tuple< Image< int >, Vector< size_t > > > &detected );

    void SetParameters( ParameterInterpreter *interpreter );

    std::string GetParameters( ParameterInterpreter *interpreter );

    BASfeature Run( );
  };
}

#endif
