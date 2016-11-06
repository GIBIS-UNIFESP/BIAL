#include "DescriptionFeatureExtractor.hpp"

#ifndef GCH_H
#define GCH_H

namespace Bial {

  typedef Vector < Features < int >> GCHfeature;

  class GCH : public FeatureExtractor< Color, int > {
private:
    size_t dim;

public:
    GCH( FeatureDetector< Color > *Fd );

    GCH( Vector < std::tuple < Image< Color >, Image< int >> > detected );

    void SetParameters( ParameterInterpreter *interpreter );

    std::string GetParameters( ParameterInterpreter *interpreter );

    GCHfeature Run( );
  };
}
#endif
