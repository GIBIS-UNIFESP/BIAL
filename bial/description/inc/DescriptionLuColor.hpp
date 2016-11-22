#include "DescriptionFeatureExtractor.hpp"

#ifndef LUCOLOR_H
#define LUCOLOR_H

namespace Bial {

  typedef Vector < Features < int >> LUCOLORfeature;

  class LUCOLOR : public FeatureExtractor< Color, int > {
private:
    size_t dim;
    size_t qtd_blocos;

public:
    LUCOLOR( FeatureDetector< Color > *Fd );

    LUCOLOR( Vector < std::tuple < Image< Color >, Image< int >> > detected );

    void SetParameters( ParameterInterpreter *interpreter );

    std::string GetParameters( ParameterInterpreter *interpreter );

    LUCOLORfeature Run( );
  };
}

#endif
