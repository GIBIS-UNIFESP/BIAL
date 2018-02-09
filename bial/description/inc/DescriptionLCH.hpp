#include "DescriptionFeatureExtractor.hpp"

#ifndef LCH_H
#define LCH_H

namespace Bial {

  typedef Vector< Features < int > > LCHfeature;

  class LCH : public FeatureExtractor< Color, int > {
private:
    size_t dim;
    size_t BINS;

public:
    LCH( FeatureDetector< Color > *Fd );

    LCH( const Vector< std::tuple< Image< Color >, Vector< size_t > > > &detected );

    void SetParameters( ParameterInterpreter *interpreter );

    std::string GetParameters( ParameterInterpreter *interpreter );

    LCHfeature Run( );
  };

}

#endif
