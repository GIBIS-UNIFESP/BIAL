#include "FeatureExtractor.h"

#ifndef LAS_H
#define LAS_H

namespace Bial {

  typedef Vector < Features < int >> LASfeature;

  class LAS : public FeatureExtractor< int, int > {
private:
    size_t bins;

    static size_t QuantizeValue( int value );

public:
    LAS( FeatureDetector< int > *Fd );

    LAS( Vector < std::tuple < Image< int >, Image< int >> > detected );

    void SetParameters( ParameterInterpreter *interpreter );

    std::string GetParameters( ParameterInterpreter *interpreter );

    LASfeature Run( );
  };

}
#endif
