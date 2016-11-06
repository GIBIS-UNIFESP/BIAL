#include "FeatureExtractor.h"

#ifndef SCD_H
#define SCD_H

namespace Bial {

  typedef Vector < Features < int >> SCDfeature;

  class SCD : public FeatureExtractor< Color, int > {
private:
    size_t dim;
    static Image< Color > RgbToHsv( Image< Color > img );

public:
    SCD( FeatureDetector< Color > *Fd );

    SCD( Vector < std::tuple < Image< Color >, Image< int >> > detected );

    void SetParameters( ParameterInterpreter *interpreter );

    std::string GetParameters( ParameterInterpreter *interpreter );

    SCDfeature Run( );
  };
}
#endif
