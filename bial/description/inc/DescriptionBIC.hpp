#include "DescriptionFeatureExtractor.hpp"

#ifndef BIC_H
#define BIC_H

namespace Bial {

  typedef Vector< Features < int > > BICfeature;

  class BIC : public FeatureExtractor< Color, int > {
private:
    size_t dim;

public:
    BIC( FeatureDetector< Color > *Fd );

    BIC( const Vector< std::tuple< Image< Color >, Vector< size_t > > > &detected );

    void SetParameters( ParameterInterpreter *interpreter );

    std::string GetParameters( ParameterInterpreter *interpreter );

    BICfeature Run( );
  };
}

#endif
