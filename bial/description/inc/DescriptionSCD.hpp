#include "DescriptionFeatureExtractor.hpp"

#ifndef SCD_H
#define SCD_H

namespace Bial {

  typedef Vector< Features < int > > SCDfeature;

  class SCD : public FeatureExtractor< Color, int > {
private:
    size_t dim;

public:
    SCD( FeatureDetector< Color > *Fd );

    SCD( const Vector< std::tuple< Image< Color >, Vector< size_t > > > &detected );

    void SetParameters( ParameterInterpreter *interpreter );

    std::string GetParameters( ParameterInterpreter *interpreter );

    SCDfeature Run( );
  };
}
#endif
