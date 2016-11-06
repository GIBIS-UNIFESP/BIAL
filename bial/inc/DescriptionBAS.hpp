#include "FeatureExtractor.h"

#ifndef BAS_H
#define BAS_H

namespace Bial {

  typedef Vector < Features < double >> BASfeature;

  class BAS : public FeatureExtractor< int, double > {
    enum {NIL = -1, WHITE = 0, GRAY = 1, BLACK = 2};
private:

    size_t SAMPLES;

public:
    BAS( FeatureDetector< int > *Fd );

    BAS( Vector < std::tuple < Image< int >, Image< int >> > detected );

    void SetParameters( ParameterInterpreter *interpreter );

    std::string GetParameters( ParameterInterpreter *interpreter );

    BASfeature Run( );
  };
}

#endif
