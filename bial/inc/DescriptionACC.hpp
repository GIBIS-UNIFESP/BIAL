#include "DescriptionFeatureExtractor.hpp"

#ifndef ACC_H
#define ACC_H

namespace Bial {

  typedef Vector < Features < int >> ACCfeature;

  class ACC : public FeatureExtractor< Color, int > {
private:
    size_t dim;
    size_t n_distances;
    Vector< size_t > distances;

public:
    ACC( FeatureDetector< Color > *Fd );

    ACC( Vector < std::tuple < Image< Color >, Image< int >> > detected );

    void SetParameters( ParameterInterpreter *interpreter );

    std::string GetParameters( ParameterInterpreter *interpreter );

    ACCfeature Run( );
  };
}
#endif
