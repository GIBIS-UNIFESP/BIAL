#include "DescriptionFeatureExtractor.hpp"

#ifndef CCH_H
#define CCH_H

namespace Bial{
  typedef Vector < Features < int >> CCHfeature;

  class CCH : public FeatureExtractor< Color, int > {
  private:
    size_t dim;
    size_t grid;

  public:
    CCH( FeatureDetector< Color > *Fd );

    CCH( Vector < std::tuple < Image< Color >, Image< int >> > detected );

    void SetParameters( ParameterInterpreter *interpreter );

    std::string GetParameters( ParameterInterpreter *interpreter );

    CCHfeature Run( );
  };
}
#endif
